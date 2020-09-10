#include "Pendulum.h"

#define EPS 1e-8

double Pendulum::dt=1000;

Pendulum::Pendulum(const Vector3d& mc,double m,const Tensor3x3& im,const vector<Vector3d>& c,const vector<Vector3d>& d,
                    bool con, const Vector3d& ra)
:Body(c,d),massCenter(mc),mass(m),inertiaMoment(im),constraint(con),rotationAxis(ra){
    if(inertiaMoment.det()!=0.0){
        inverseInertiaMoment=inertiaMoment.invert();
    }else{
        inverseInertiaMoment=inverse2();
    }
    rotationAxis/=rotationAxis.length();
}

Pendulum::~Pendulum(){
    
}

Tensor3x3 Pendulum::inverse2(){
    double c[]={inertiaMoment.x.x,inertiaMoment.x.y,inertiaMoment.x.z,
                inertiaMoment.y.x,inertiaMoment.y.y,inertiaMoment.y.z,
                inertiaMoment.z.x,inertiaMoment.z.y,inertiaMoment.z.z};
    double w[3];
    int info=lap::get_eigenvalues_and_vectors_symmetric(3,c,w);
    Vector3d vec[3];
    for(int i=0;i<3;i++){
        vec[i].set(c[3*i+0],c[3*i+1],c[3*i+2]);
    }
    Tensor3x3 inv;
    for(int i=0;i<3;i++){
        if(abs(w[i])>EPS){
            inv+=dyad(vec[i],vec[i])/w[i];
        }
    }
    return inv;
}

void Pendulum::torqueReset(){
    torque.set(0.0,0.0,0.0);
}

void Pendulum::angularVelocityConstraint(){
    angularVelocity=(angularVelocity*rotationAxis)*rotationAxis;
}

void Pendulum::initial(){
    Body::update();
    angularMomentum=inertiaMoment*angularVelocity;
}

void Pendulum::updateEuler(){
    angularMomentum+=torque*dt;
    angularVelocity.set(0,0,0);
    for(int i=0;i<3;i++){
        double uiL=frame(i)*angularMomentum;
        for(int j=0;j<3;j++){
            angularVelocity+=inverseInertiaMoment(j,i)*uiL*frame(j);
        }
    }
    if(constraint)angularVelocityConstraint();
    frame.u1+=(angularVelocity^frame.u1)*dt;
    frame.u2+=(angularVelocity^frame.u2)*dt;
    frame.u3+=(angularVelocity^frame.u3)*dt;
    Body::update();
}

void Pendulum::updateRungeKutta1(){
    Ln=angularMomentum;
    fn=frame;
    //
    dL1=torque*dt;
    df1.u1=(angularVelocity^frame.u1)*dt;
    df1.u2=(angularVelocity^frame.u2)*dt;
    df1.u3=(angularVelocity^frame.u3)*dt;
    angularMomentum=Ln+0.5*dL1;
    frame.u1=fn.u1+0.5*df1.u1;
    frame.u2=fn.u2+0.5*df1.u2;
    frame.u3=fn.u3+0.5*df1.u3;
    angularVelocity.set(0,0,0);
    for(int i=0;i<3;i++){
        double uiL=frame(i)*angularMomentum;
        for(int j=0;j<3;j++){
            angularVelocity+=inverseInertiaMoment(j,i)*uiL*frame(j);
        }
    }
    if(constraint)angularVelocityConstraint();
    Body::update();
}

void Pendulum::updateRungeKutta2(){
    dL2=torque*dt;
    df2.u1=(angularVelocity^frame.u1)*dt;
    df2.u2=(angularVelocity^frame.u2)*dt;
    df2.u3=(angularVelocity^frame.u3)*dt;
    angularMomentum=Ln+0.5*dL2;
    frame.u1=fn.u1+0.5*df2.u1;
    frame.u2=fn.u2+0.5*df2.u2;
    frame.u3=fn.u3+0.5*df2.u3;
    angularVelocity.set(0,0,0);
    for(int i=0;i<3;i++){
        double uiL=frame(i)*angularMomentum;
        for(int j=0;j<3;j++){
            angularVelocity+=inverseInertiaMoment(j,i)*uiL*frame(j);
        }
    }
    if(constraint)angularVelocityConstraint();
    Body::update(); 
}

void Pendulum::updateRungeKutta3(){
    dL3=torque*dt;
    df3.u1=(angularVelocity^frame.u1)*dt;
    df3.u2=(angularVelocity^frame.u2)*dt;
    df3.u3=(angularVelocity^frame.u3)*dt;
    angularMomentum=Ln+dL3;
    frame.u1=fn.u1+df3.u1;
    frame.u2=fn.u2+df3.u2;
    frame.u3=fn.u3+df3.u3;
    angularVelocity.set(0,0,0);   
    for(int i=0;i<3;i++){
        double uiL=frame(i)*angularMomentum;
        for(int j=0;j<3;j++){
            angularVelocity+=inverseInertiaMoment(j,i)*uiL*frame(j);
        }
    }
    if(constraint)angularVelocityConstraint();
    Body::update();
}

void Pendulum::updateRungeKutta4(){
    dL4=torque*dt;
    df4.u1=(angularVelocity^frame.u1)*dt;
    df4.u2=(angularVelocity^frame.u2)*dt;
    df4.u3=(angularVelocity^frame.u3)*dt;
    //
    angularMomentum=Ln+(dL1+2*dL2+2*dL3+dL4)/6;
    frame.u1=fn.u1+(df1.u1+2*df2.u1+2*df3.u1+df4.u1)/6;
    frame.u2=fn.u2+(df1.u2+2*df2.u2+2*df3.u2+df4.u2)/6;
    frame.u3=fn.u3+(df1.u3+2*df2.u3+2*df3.u3+df4.u3)/6;
    angularVelocity.set(0,0,0);   
    for(int i=0;i<3;i++){
        double uiL=frame(i)*angularMomentum;
        for(int j=0;j<3;j++){
            angularVelocity+=inverseInertiaMoment(j,i)*uiL*frame(j);
        }
    }
    if(constraint)angularVelocityConstraint();
    Body::update();
}

#undef EPS
