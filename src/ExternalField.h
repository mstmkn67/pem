#ifndef _EXTERNAL_FIELD_H_
#define _EXTERNAL_FIELD_H_

#include "Vector3d.h"

class ExternalField{
public:
    ExternalField(){};
    virtual ~ExternalField(){};
	virtual Vector3d getField(const Vector3d& position){
		return Vector3d(0,0,0);
	};
private:
};

class ConstantField:public ExternalField{
public:
	ConstantField(const Vector3d& m):field(m){};
	virtual ~ConstantField(){};
	virtual Vector3d getField(const Vector3d& position){
		return field;
	}
private:
	Vector3d field;
};

class EllipticRotationField:public ExternalField{
public:
	EllipticRotationField(double angular_velocity_x,const Vector3d& Bx,
                          double angular_velocity_y,const Vector3d& By,double* time);
	virtual ~EllipticRotationField();
	virtual Vector3d getField(const Vector3d& position);
private:
	double ax,ay;
	Vector3d Bx;
	Vector3d By;
    double* time;
};

#endif // _EXTERMAL_FIELD_H_
