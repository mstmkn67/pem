#include "Frame.h"

Frame::Frame(){
    u1.set(1.0,0.0,0.0);
    u2.set(0.0,1.0,0.0);
    u3.set(0.0,0.0,1.0);
}

Frame::Frame(const Vector3d& _u1=Vector3d(1,0,0),const Vector3d& _u2=Vector3d(0,1,0))
:u1(_u1),u2(_u2){
    u3=u1^u2;
    //correct1();
    correct2();
}

Frame::~Frame(){
}

Vector3d Frame::operator()(int i)const{
    if(i==0)return u1;
    if(i==1)return u2;
    if(i==2)return u3;
}

void Frame::update(){
    //correct1();
    correct2();
}

void Frame::correct1(){
    u1/=u1.length();
    u2-=(u2*u1)*u1;
    u2/=u2.length();
    u3=u1^u2;
}

void Frame::correct2(){
    u1=2*u1/(1+u1.length2());
    u2=2*u2/(1+u2.length2());
    u3=2*u3/(1+u3.length2());
    Vector3d u1t=u1,u2t=u2,u3t=u3;
    u1=(u1t-(u1t*u2t)*u2t)/(1-0.5*(u1t*u2t));
    u2=(u2t-(u2t*u3t)*u3t)/(1-0.5*(u2t*u3t));
    u3=(u3t-(u3t*u1t)*u1t)/(1-0.5*(u3t*u1t));
}