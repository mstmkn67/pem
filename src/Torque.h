#ifndef _TORQUE_H_
#define _TORQUE_H_

#include "Pendulum.h"
#include "Body.h"
#include "ExternalField.h"
#include "BoundaryCondition.h"
#include <vector>
using namespace std;

class Torque{
public:
    Torque(const vector<Pendulum*>& pendulum,const vector<Body*>& body);
    virtual ~Torque();
    virtual void update();
protected:
    vector<Pendulum*> pendulum;
    vector<Body*> body;
private:
};

class GravityTorque:public Torque{
public:
    GravityTorque(const vector<Pendulum*>& pendulum,const vector<Body*>& body,
                 ExternalField* field);
    virtual ~GravityTorque();
    virtual void update();
private:
    ExternalField* field;
};

class ExternalMagneticTorque:public Torque{
public:
    ExternalMagneticTorque(const vector<Pendulum*>& pendulum,const vector<Body*>& body,
                           ExternalField* field);
    virtual ~ExternalMagneticTorque();
    virtual void update();
private:
    ExternalField* field;
};

class MagneticInteractionTorque:public Torque{
public:
    MagneticInteractionTorque(const vector<Pendulum*>& pendulum,const vector<Body*>& body,
                              double permeability);
    virtual ~MagneticInteractionTorque();
    virtual void update();
    static void setBoundaryCondition(BoundaryCondition* bc);
protected:
    virtual Vector3d get_dUdR(const Vector3d& R,const Vector3d& p,const Vector3d& pp);
    virtual Vector3d get_dUdp(const Vector3d& R,const Vector3d& pp);
private:
    double permeability;
    double pe_4pi,pe3_4pi;
    static BoundaryCondition* bc;
};

class PseudoFrictionTorque:public Torque{
public:
    PseudoFrictionTorque(const vector<Pendulum*>& pendulum,const vector<Body*>& body,
                         double coefficient);
    virtual ~PseudoFrictionTorque();
    virtual void update();

private:
    double coefficient;
};

#endif // _TORQUE_H_

