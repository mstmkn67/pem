#include "Torque.h"

Torque::Torque(const vector<Pendulum*>& p,const vector<Body*>& b)
:pendulum(p),body(b){

}

Torque::~Torque(){

}

void Torque::update(){

}

//
GravityTorque::GravityTorque(const vector<Pendulum*>& p,const vector<Body*>& b,
                             ExternalField* ef)
:Torque(p,b),field(ef){

}

GravityTorque::~GravityTorque(){

}

void GravityTorque::update(){
    for(auto p:pendulum){
        const Vector3d rc=p->massCenter;
        double mass=p->mass;
        Vector3d g=field->getField(rc);
        Vector3d t=rc.x*(p->frame.u1^g);
        t+=rc.y*(p->frame.u2^g);
        t+=rc.z*(p->frame.u3^g);
        //cout << rc << " " ;
        p->torque+=mass*t;
    }
}

//
ExternalMagneticTorque::ExternalMagneticTorque(const vector<Pendulum*>& p,const vector<Body*>& b,
                             ExternalField* ef)
:Torque(p,b),field(ef){

}

ExternalMagneticTorque::~ExternalMagneticTorque(){

}

void ExternalMagneticTorque::update(){
    for(auto p:pendulum){
        Vector3d t;
        int n=p->dipole.size();
        for(int i=0;i<n;i++){
            Vector3d& r=p->dCenterL[i];
            Vector3d& d=p->dipole[i];
            Vector3d b=field->getField(r);
            t+=d.x*(p->frame.u1^b);
            t+=d.y*(p->frame.u2^b);
            t+=d.z*(p->frame.u3^b);
        }
        p->torque+=t;
    }
}
//

BoundaryCondition* MagneticInteractionTorque::bc=0;

MagneticInteractionTorque::MagneticInteractionTorque(
    const vector<Pendulum*>& p,const vector<Body*>& b,double pe)
:Torque(p,b),permeability(pe){
    pe_4pi=0.25*permeability/M_PI;
    pe3_4pi=0.75*permeability/M_PI;
}

MagneticInteractionTorque::~MagneticInteractionTorque(){

}

Vector3d MagneticInteractionTorque::get_dUdR(const Vector3d& R,const Vector3d& p,const Vector3d& pp){
    double Rp=R*p,Rpp=R*pp,R2=R.length2(),R5=R2*R2*sqrt(R2);
    Vector3d a=(-5*Rp*Rpp/R2)*R;
    a+=(pp*R)*p+(R*p)*pp+(p*pp)*R;
    return pe3_4pi/R5*a;
}

Vector3d MagneticInteractionTorque::get_dUdp(const Vector3d& R,const Vector3d& pp){
    double Rpp=R*pp,R2=R.length2(),R3=R2*sqrt(R2);
    Vector3d a=(3*Rpp/R2)*R-pp;
    return pe_4pi/R3*a;
}

void MagneticInteractionTorque::update(){
    vector<Pendulum*>::iterator p=pendulum.begin();
    for(auto a:pendulum){
        int np=a->dipoleL.size();
        for(auto b:pendulum){
            if(a==b)continue;
            int nq=b->dipoleL.size();
            for(int ip=0;ip<np;ip++){
                for(int iq=0;iq<nq;iq++){
                    //Vector3d R=b->dCenterL[iq]-a->dCenterL[ip];
                    Vector3d R=bc->difPosition(a->dCenterL[ip],b->dCenterL[iq]);
                    Vector3d dUdR=get_dUdR(R,a->dipoleL[ip],b->dipoleL[iq]);
                    Vector3d dUdp=get_dUdp(R,b->dipoleL[iq]);
                    a->torque-=(a->dCenterL[ip]-a->position)^dUdR;
                    a->torque+=(a->dipoleL[ip])^dUdp;
                }
            }
        }
        for(auto b:body){
            int nb=b->dipoleL.size();
            for(int ip=0;ip<np;ip++){
                for(int ib=0;ib<nb;ib++){
                    //Vector3d R=b->dCenterL[ib]-a->dCenterL[ip];
                    Vector3d R=bc->difPosition(a->dCenterL[ip],b->dCenterL[ib]);
                    Vector3d dUdR=get_dUdR(R,a->dipoleL[ip],b->dipoleL[ib]);
                    Vector3d dUdp=get_dUdp(R,b->dipoleL[ib]);
                    a->torque-=(a->dCenterL[ip]-a->position)^dUdR;
                    a->torque+=(a->dipoleL[ip])^dUdp;                       
                }
            }
        }
    }
}

void MagneticInteractionTorque::setBoundaryCondition(BoundaryCondition* b){
    bc=b;
}

//
PseudoFrictionTorque::PseudoFrictionTorque(
    const vector<Pendulum*>& p,const vector<Body*>& b,double c
):Torque(p,b),coefficient(c){

}

PseudoFrictionTorque::~PseudoFrictionTorque(){

}

void PseudoFrictionTorque::update(){
    for(auto p:pendulum){
        p->torque-=coefficient*p->angularVelocity;
    }
}