#include "Body.h"

Body::Body(const vector<Vector3d>& dc,const vector<Vector3d>& di)
:dCenter(dc),dipole(di){
    int n=dCenter.size();
    dCenterL.resize(n);
    dipoleL.resize(n);
    update();
}

Body::~Body(){

}

void Body::update(){
    for(int i=0;i<dCenter.size();i++){
        dCenterL[i]=position
                    +dCenter[i].x*frame.u1
                    +dCenter[i].y*frame.u2
                    +dCenter[i].z*frame.u3;
        dipoleL[i]=dipole[i].x*frame.u1
                  +dipole[i].y*frame.u2
                  +dipole[i].z*frame.u3;
    }
}

SteadyMovingBody::SteadyMovingBody(const Vector3d& p,const Frame& f,const Vector3d& v,double* t,
                                   const vector<Vector3d>& dc,const vector<Vector3d>& di)
:Body(dc,di),position0(p),velocity(v),time(t){
    position=position0;
    frame=f;
}

SteadyMovingBody::~SteadyMovingBody(){
}

void SteadyMovingBody::update(){
    position=position0+velocity*(*time);
    Body::update();
}

RotationBody::RotationBody(
    const Vector3d& p,const Frame& f,
    const Vector3d& rc, double t,double av,
    const Vector3d& am1,const Vector3d& am2,double _dt,
    const vector<Vector3d>& dc,const vector<Vector3d>& di)
:Body(dc,di),rotationCenter(rc),theta(t),angularVelocity(av),
amplitude1(am1),amplitude2(am2),dt(_dt)
{
    position=p;
    frame=f;
    omega=amplitude1^amplitude2;
    omega/=omega.length();
    omega=angularVelocity*omega;
}

RotationBody::~RotationBody(){

}

void RotationBody::update(){
    theta+=angularVelocity*dt;
    position=rotationCenter+amplitude1*cos(theta)+amplitude2*sin(theta);
    frame.u1+=(omega^frame.u1)*dt;
    frame.u2+=(omega^frame.u2)*dt;
    frame.u3+=(omega^frame.u3)*dt;
    frame.update();
    Body::update();
}
