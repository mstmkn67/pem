#ifndef _PENDULUM_H_
#define _PENDULUM_H_

#include "Vector3d.h"
#include "Tensor3x3.h"
#include "Frame.h"
#include "Body.h"
#include "solver/LapackFunctions.h"
#include <vector>
#include <cmath>
using namespace std;

class Pendulum:public Body{
public:
    Pendulum(const Vector3d& massCenter,double masss,const Tensor3x3& inertiaMoment,const vector<Vector3d>& dcenter,const vector<Vector3d>& dipole,
             bool constraint=false, const Vector3d& rotationAxis=Vector3d(1.0,0.0,0.0));
    virtual ~Pendulum();
    virtual void torqueReset();
    virtual void initial();
    virtual void updateEuler();
    virtual void updateRungeKutta1();
    virtual void updateRungeKutta2();
    virtual void updateRungeKutta3();
    virtual void updateRungeKutta4();
    //
    static void setDt(double _dt){dt=_dt;};
    //
    double mass;
    Vector3d massCenter;
    //
    Vector3d torque;
    Vector3d angularVelocity;
    Vector3d angularMomentum;
    //Frame frame;
    //
    //vector<Vector3d> dCenter;
    //vector<Vector3d> dipole;
protected:
    virtual Tensor3x3 inverse2();
    virtual void angularVelocityConstraint();
private:
    static double dt;
    Tensor3x3 inertiaMoment;
    Tensor3x3 inverseInertiaMoment;
    //
    bool constraint;
    Vector3d rotationAxis;
    //parameters for Runge Kutta method
    Vector3d Ln,dL1,dL2,dL3,dL4;
    Frame fn,df1,df2,df3,df4;
};


#endif // _PENDULUM_H_
