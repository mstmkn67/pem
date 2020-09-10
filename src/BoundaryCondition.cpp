#include "BoundaryCondition.h"

BoundaryCondition::BoundaryCondition(){

}

BoundaryCondition::~BoundaryCondition(){

}

Vector3d BoundaryCondition::difPosition(const Vector3d& r0,const Vector3d& r1){
	return r1-r0;
}
Vector3d BoundaryCondition::getPosition(const Vector3d& r){
	return r;
}
//

PeriodicBoundaryCondition::PeriodicBoundaryCondition(
	const Vector3d& sm,const Vector3d& s
):min(sm),size(s){
}

PeriodicBoundaryCondition::~PeriodicBoundaryCondition(){

}

Vector3d PeriodicBoundaryCondition::difPosition(const Vector3d& r0,const Vector3d& r1){
	Vector3d r=r1-r0;
	return Vector3d(r.x-floor(r.x/size.x+0.5)*size.x,
	                r.y-floor(r.y/size.y+0.5)*size.y,
                  r.z-floor(r.z/size.z+0.5)*size.z);
}

Vector3d PeriodicBoundaryCondition::getPosition(const Vector3d& r){
	return Vector3d(r.x-floor((r.x-min.x)/size.x)*size.x, 
		              r.y-floor((r.y-min.y)/size.y)*size.y,
		              r.z-floor((r.z-min.z)/size.z)*size.z );
}