#ifndef _BODY_H_
#define _BODY_H_

#include "Vector3d.h"
#include "Frame.h"
#include <vector>
using namespace std;

class Body{
public:
    Body(const vector<Vector3d>& dcenter,const vector<Vector3d>& dipole);
    virtual ~Body();
    virtual void update();
    Vector3d position;
    Frame frame;
    //
    vector<Vector3d> dCenter; //particle fixed system
    vector<Vector3d> dipole; //particle fixed system
    vector<Vector3d> dCenterL; //laboratory system
    vector<Vector3d> dipoleL; //laboratory system
private:

};

class SteadyMovingBody:public Body{
public:
    SteadyMovingBody(const Vector3d& position0,const Frame& frame,
    const Vector3d& velocity,double* time,
    const vector<Vector3d>& dcenter,const vector<Vector3d>& dipole);
    virtual ~SteadyMovingBody();
    virtual void update();
private:
    Vector3d position0; // center of rotation
    Vector3d velocity;
    double* time;
};

class RotationBody:public Body{
public:
    RotationBody(const Vector3d& position,const Frame& frame,
                const Vector3d& rotationCenter,
                double theta,double angularVelocity,
                const Vector3d& amplitude1,const Vector3d& amplitude2,double dt,
                const vector<Vector3d>& dcenter,const vector<Vector3d>& dipole);
    virtual ~RotationBody();
    virtual void update();
private:
    Vector3d rotationCenter;
    double theta;// angle
	double angularVelocity;//angular velocity
	Vector3d amplitude1,amplitude2;
    Vector3d omega;
    //double* time;
    double dt;
};

#endif //_BODY_H_

