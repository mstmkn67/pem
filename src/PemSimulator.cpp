#include "PemSimulator.h"

#include <climits>


PemSimulator::PemSimulator(UDFManager* _in,UDFManager* _out)
:in(_in),out(_out){
	timer=new Timer;
}

PemSimulator::~PemSimulator(){
  for(auto p:pendulum){
    delete p;
  }
  for(auto b:body){
    delete b;
  }
  for(auto t:torque){
    delete t;
  }
  delete mField;
  delete gField;
  delete bc;
	delete timer;
}

void PemSimulator::update(){
  time=0;
  input();
  //
  for(auto p:pendulum)p->initial();
  for(auto b:body)b->update();
  //
  output();
  //
  int simulationSteps=in->i("simulation.time.simulationSteps");
  int reportSteps=in->i("simulation.time.reportSteps");
  double dt=in->d("simulation.time.dt");
  string integrator=in->s("simulation.integrator");
  for(int i=1;i<=simulationSteps;i++){
    time=i*dt;
    if(integrator=="4thOrderRungeKutta"){
      //RungeKutta1
      for(auto p:pendulum)p->torqueReset();
      for(auto t:torque)t->update();
      for(auto p:pendulum)p->updateRungeKutta1();
      //RungeKutta2
      time+=0.5*dt;
      for(auto p:pendulum)p->torqueReset();
      for(auto t:torque)t->update();
      for(auto p:pendulum)p->updateRungeKutta2();
      //RungeKutta3
      for(auto p:pendulum)p->torqueReset();
      for(auto t:torque)t->update();
      for(auto p:pendulum)p->updateRungeKutta3();
      //RungeKutta4
      time+=0.5*dt;
      for(auto p:pendulum)p->torqueReset();
      for(auto t:torque)t->update();
      for(auto p:pendulum)p->updateRungeKutta4();
    }else if(integrator=="Euler"){//Euler
      for(auto p:pendulum)p->torqueReset();
      for(auto t:torque)t->update();
      for(auto p:pendulum)p->updateEuler();
    }else{
      cout << "integrator type, " << integrator << "is not implemented" << endl;
      exit(1);
    }
    for(auto p:pendulum)p->frame.update();
    for(auto b:body)b->update();
    if(i%reportSteps==0){
      cout << "report at step " << i << endl;
      output();
    }
  }
}

void PemSimulator::input(){
  /////////// pendulum
  Location ploc("simulation.pendulum[]");
  for(int i=0;i<in->size(ploc);i++){
    ploc.next();
    int id=in->i(ploc.sub("body"));
    Tensor3x3 I;
    Location pb=in->getLocation("Body",id);
    pb.up();
    double mass=in->d(pb.sub("analysis.mass"));
    Vector3d com(in->d(pb.sub("analysis.massCenter.x")),
                 in->d(pb.sub("analysis.massCenter.y")),
                 in->d(pb.sub("analysis.massCenter.z")));
    I.x.x=in->d(pb.sub("analysis.inertiaMoment.xx"));
    I.y.y=in->d(pb.sub("analysis.inertiaMoment.yy"));
    I.z.z=in->d(pb.sub("analysis.inertiaMoment.zz"));
    I.y.x=I.x.y=in->d(pb.sub("analysis.inertiaMoment.yx"));
    I.z.x=I.x.z=in->d(pb.sub("analysis.inertiaMoment.zx"));
    I.z.y=I.y.z=in->d(pb.sub("analysis.inertiaMoment.zy"));
    Location pbm=pb.sub("magnet[]");
    vector<Vector3d> center,dipole;
    for(int j=0;j<in->size(pbm);j++){
      pbm.next();
      Vector3d c(in->d(pbm.sub("position.x")),in->d(pbm.sub("position.y")),in->d(pbm.sub("position.z")));
      Vector3d d(in->d(pbm.sub("dipole.x")),in->d(pbm.sub("dipole.y")),in->d(pbm.sub("dipole.z")));
      center.push_back(c);
      dipole.push_back(d);
    }
    bool cflag=false;Vector3d ra;
    if(in->s(ploc.sub("constraint.flag"))=="true"){
      cflag=true;
      ra.set(in->d(ploc.sub("constraint.rotationAxis.x")),
             in->d(ploc.sub("constraint.rotationAxis.y")),
             in->d(ploc.sub("constraint.rotationAxis.z")));
    }
    Pendulum* pen=new Pendulum(com,mass,I,center,dipole,cflag,ra);
    pen->position.set(in->d(ploc.sub("position.x")),in->d(ploc.sub("position.y")),in->d(ploc.sub("position.z")));
    pen->frame.u1.set(in->d(ploc.sub("orientation.u1.x")),in->d(ploc.sub("orientation.u1.y")),in->d(ploc.sub("orientation.u1.z")));
    pen->frame.u2.set(in->d(ploc.sub("orientation.u2.x")),in->d(ploc.sub("orientation.u2.y")),in->d(ploc.sub("orientation.u2.z")));
    pen->frame.u3.set(in->d(ploc.sub("orientation.u3.x")),in->d(ploc.sub("orientation.u3.y")),in->d(ploc.sub("orientation.u3.z")));
    pen->angularVelocity.set(in->d(ploc.sub("angularVelocity.x")),in->d(ploc.sub("angularVelocity.y")),in->d(ploc.sub("angularVelocity.z")));
    pendulum.push_back(pen);
  }
  double dt=in->d("simulation.time.dt");
  Pendulum::setDt(dt);
  /////////// Body
  Location bloc("simulation.body[]");
  for(int i=0;i<in->size(bloc);i++){
    bloc.next();
    int id=in->i(bloc.sub("body"));
    Location bb=in->getLocation("Body",id);
    bb.up();
    Location bbm=bb.sub("magnet[]");
    vector<Vector3d> center,dipole;
    for(int j=0;j<in->size(bbm);j++){
      bbm.next();
      Vector3d c(in->d(bbm.sub("position.x")),in->d(bbm.sub("position.y")),in->d(bbm.sub("position.z")));
      Vector3d d(in->d(bbm.sub("dipole.x")),in->d(bbm.sub("dipole.y")),in->d(bbm.sub("dipole.z")));
      center.push_back(c);
      dipole.push_back(d);
    }
    Vector3d pos(in->d(bloc.sub("position.x")),in->d(bloc.sub("position.y")),in->d(bloc.sub("position.z")));
    Vector3d u1(in->d(bloc.sub("orientation.u1.x")),in->d(bloc.sub("orientation.u1.y")),in->d(bloc.sub("orientation.u1.z")));
    Vector3d u2(in->d(bloc.sub("orientation.u2.x")),in->d(bloc.sub("orientation.u2.y")),in->d(bloc.sub("orientation.u2.z")));
    Frame f(u1,u2);
    string type=in->s(bloc.sub("type"));
    if(type=="fixed"){
      Body* b=new Body(center,dipole);
      b->position=pos;
      b->frame=f;
      body.push_back(b);
    }else if(type=="constantVelocity"){
      Vector3d v(in->d(bloc.sub("constantVelocity.velocity.x")),
                 in->d(bloc.sub("constantVelocity.velocity.y")),
                 in->d(bloc.sub("constantVelocity.velocity.z")));
      Body* b=new SteadyMovingBody(pos,f,v,&time,center,dipole);
      body.push_back(b);
    }else if(type=="ellipticRotation"){
      Vector3d rc(in->d(bloc.sub("ellipticRotation.center.x")),
                  in->d(bloc.sub("ellipticRotation.center.y")),
                  in->d(bloc.sub("ellipticRotation.center.z")));
      double itheta=in->d(bloc.sub("ellipticRotation.initialPhase"));
      double av=in->d(bloc.sub("ellipticRotation.angularVelocity"));
      Vector3d r1(in->d(bloc.sub("ellipticRotation.r1.x")),
                  in->d(bloc.sub("ellipticRotation.r1.y")),
                  in->d(bloc.sub("ellipticRotation.r1.z")));
      Vector3d r2(in->d(bloc.sub("ellipticRotation.r2.x")),
                  in->d(bloc.sub("ellipticRotation.r2.y")),
                  in->d(bloc.sub("ellipticRotation.r2.z")));
      double dt=in->d("simulation.time.dt");
      Body* b=new RotationBody(pos,f,rc,itheta,av,r1,r2,dt,center,dipole);
      body.push_back(b);
    }else{
      cout << bloc << ".type is incorrect. " << type << endl;
      exit(1);
    }
  }
  /////////// gravity field
  Vector3d g(in->d("simulation.gravity.x"),in->d("simulation.gravity.y"),in->d("simulation.gravity.z"));
  gField=new ConstantField(g);
  torque.push_back(new GravityTorque(pendulum,body,gField));
  /////////// magnetic field
  string ftype=in->s("simulation.magnetiField.fieldType");
  if(ftype=="false" || ftype==""){
    mField=new ExternalField;
  }else if(ftype=="constantField"){
    Vector3d b(in->d("simulation.magnetiField.constantField.B.x"),
               in->d("simulation.magnetiField.constantField.B.y"),
               in->d("simulation.magnetiField.constantField.B.z"));
    mField=new ConstantField(b);
    torque.push_back(new ExternalMagneticTorque(pendulum,body,mField));
  }else if(ftype=="ellipticRotationField"){
    double a1=in->d("simulation.magnetiField.ellipticRotationField.angularVelocity");
    Vector3d b1(in->d("simulation.magnetiField.ellipticRotationField.B1.x"),
                in->d("simulation.magnetiField.ellipticRotationField.B1.y"),
                in->d("simulation.magnetiField.ellipticRotationField.B1.z"));
    //double a2=in->d("simulation.magnetiField.ellipticRotationField.angularVelocity1");
    Vector3d b2(in->d("simulation.magnetiField.ellipticRotationField.B2.x"),
                in->d("simulation.magnetiField.ellipticRotationField.B2.y"),
                in->d("simulation.magnetiField.ellipticRotationField.B2.z"));
    mField=new EllipticRotationField(a1,b1,a1,b2,&time);
    torque.push_back(new ExternalMagneticTorque(pendulum,body,mField));
  }else{
    cout << "Magnetic field is incorrect. " << ftype << endl;
    exit(1);
  }
  ////////// magnetic interation
  if(pendulum.size()+body.size()>1){
    double permeability=in->d("simulation.freeSpacePermeability");
    torque.push_back(new MagneticInteractionTorque(pendulum,body,permeability));
  }
  ////////// psedo friction torque
  string f=in->s("simulation.PseudoFrictionTorque.flag");
  if(f=="true"){
    double c=in->d("simulation.PseudoFrictionTorque.coefficient");
    torque.push_back(new PseudoFrictionTorque(pendulum,body,c));
  }
  /////////// boundary condition ////////////
  string bs=in->s("simulation.periodicBoundaryCondition");
  if(bs=="true"){
    Vector3d smin(in->d("simulation.systemSize.min.x"),
                  in->d("simulation.systemSize.min.y"),
                  in->d("simulation.systemSize.min.z"));
    Vector3d smax(in->d("simulation.systemSize.max.x"),
                  in->d("simulation.systemSize.max.y"),
                  in->d("simulation.systemSize.max.z"));
    bc=new PeriodicBoundaryCondition(smin,smax-smin);
  }else{
    bc=new BoundaryCondition();
  }
  MagneticInteractionTorque::setBoundaryCondition(bc);
}

void PemSimulator::output(){
  out->newRecord();
  Location ploc("simulationResult.pendulum[]");
  for(auto  p:pendulum){
    ploc.next(); 
    Frame& f=p->frame;
    out->put(ploc.sub("orientation.u1.x"),f.u1.x);out->put(ploc.sub("orientation.u1.y"),f.u1.y);
    out->put(ploc.sub("orientation.u1.z"),f.u1.z);
    out->put(ploc.sub("orientation.u2.x"),f.u2.x);out->put(ploc.sub("orientation.u2.y"),f.u2.y);
    out->put(ploc.sub("orientation.u2.z"),f.u2.z);
    out->put(ploc.sub("orientation.u3.x"),f.u3.x);out->put(ploc.sub("orientation.u3.y"),f.u3.y);
    out->put(ploc.sub("orientation.u3.z"),f.u3.z);
    out->put(ploc.sub("angularVelocity.x"),p->angularVelocity.x);
    out->put(ploc.sub("angularVelocity.y"),p->angularVelocity.y);
    out->put(ploc.sub("angularVelocity.z"),p->angularVelocity.z);
    out->put(ploc.sub("angularMomentum.x"),p->angularMomentum.x);
    out->put(ploc.sub("angularMomentum.y"),p->angularMomentum.y);
    out->put(ploc.sub("angularMomentum.z"),p->angularMomentum.z);
    out->put(ploc.sub("torque.x"),p->torque.x);
    out->put(ploc.sub("torque.y"),p->torque.y);
    out->put(ploc.sub("torque.z"),p->torque.z);
    Location mloc(ploc.sub("magnet[]"));
    for(auto c:p->dCenterL){
      mloc.next();
      out->put(mloc.sub("position.x"),c.x);
      out->put(mloc.sub("position.y"),c.y);
      out->put(mloc.sub("position.z"),c.z);
    }
    Location dloc(ploc.sub("magnet[]"));
    for(auto d:p->dipoleL){
      dloc.next();
      out->put(dloc.sub("dipole.x"),d.x);
      out->put(dloc.sub("dipole.y"),d.y);
      out->put(dloc.sub("dipole.z"),d.z);
    }
  }
  Location bloc("simulationResult.body[]");
  for(auto b:body){
    bloc.next();
    out->put(bloc.sub("position.x"),b->position.x);
    out->put(bloc.sub("position.y"),b->position.y);
    out->put(bloc.sub("position.z"),b->position.z);
    Frame& f=b->frame;
    out->put(bloc.sub("orientation.u1.x"),f.u1.x);out->put(bloc.sub("orientation.u1.y"),f.u1.y);
    out->put(bloc.sub("orientation.u1.z"),f.u1.z);
    out->put(bloc.sub("orientation.u2.x"),f.u2.x);out->put(bloc.sub("orientation.u2.y"),f.u2.y);
    out->put(bloc.sub("orientation.u2.z"),f.u2.z);
    out->put(bloc.sub("orientation.u3.x"),f.u3.x);out->put(bloc.sub("orientation.u3.y"),f.u3.y);
    out->put(bloc.sub("orientation.u3.z"),f.u3.z);
    Location mloc(bloc.sub("magnet[]"));
    for(auto c:b->dCenterL){
      mloc.next();
      out->put(mloc.sub("position.x"),c.x);
      out->put(mloc.sub("position.y"),c.y);
      out->put(mloc.sub("position.z"),c.z);
    }
    Location dloc(bloc.sub("magnet[]"));
    for(auto d:b->dipoleL){
      dloc.next();
      out->put(dloc.sub("dipole.x"),d.x);
      out->put(dloc.sub("dipole.y"),d.y);
      out->put(dloc.sub("dipole.z"),d.z);
    }
  }
  Vector3d g=gField->getField(Vector3d(0,0,0));
  out->put("simulationResult.gravity.x",g.x);
  out->put("simulationResult.gravity.y",g.y);
  out->put("simulationResult.gravity.z",g.z);
  Vector3d b=mField->getField(Vector3d(0,0,0));
  out->put("simulationResult.magnetiField.x",b.x);
  out->put("simulationResult.magnetiField.y",b.y);
  out->put("simulationResult.magnetiField.z",b.z);
  out->put("simulationResult.cpuTime",timer->get());
	out->write();
}

void PemSimulator::nan_check(double x,char c){
  if( isnan(x) == true ){ 
    cout  << "Not a Number is found: " << c << endl;
    exit(1);
  }
}
