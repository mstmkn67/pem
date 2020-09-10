#ifndef _BOUNDARY_CONDITION_H_
#define _BOUNDARY_CONDITION_H_

#include "Vector3d.h"

class BoundaryCondition{
public:
    BoundaryCondition();
    virtual ~BoundaryCondition();
	virtual Vector3d difPosition(const Vector3d& r0,const Vector3d& r1);
	virtual Vector3d getPosition(const Vector3d& r);
};

class PeriodicBoundaryCondition:public BoundaryCondition{
public:
    PeriodicBoundaryCondition(const Vector3d& min,const Vector3d& size);
    virtual ~PeriodicBoundaryCondition();
	virtual Vector3d difPosition(const Vector3d& r0,const Vector3d& r1);
	virtual Vector3d getPosition(const Vector3d& r);
private:
    Vector3d min;
    Vector3d size;
};

#endif // _BOUNDARY_CONDITION_H_
