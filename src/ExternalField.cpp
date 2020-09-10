#include "ExternalField.h"

EllipticRotationField::EllipticRotationField(
    double _ax,const Vector3d& _Bx,double _ay,const Vector3d& _By,double* t)
    :ax(_ax),ay(_ay),Bx(_Bx),By(_By),time(t){
}

EllipticRotationField::~EllipticRotationField(){
}

Vector3d EllipticRotationField::getField(const Vector3d& position){
   	double tt=*time;
	return Bx*cos(tt*ax)+By*sin(tt*ay); 
}