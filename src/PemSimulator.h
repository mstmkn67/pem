#ifndef _PEM_SIMULATOR_H_
#define _PEM_SIMULATOR_H_

#include "udfmanager.h"
#include "Timer.h"
#include <cmath>
#include <vector>
#include <sstream>
using namespace std;
//
#include "Pendulum.h"
#include "Body.h"
#include "ExternalField.h"
#include "Torque.h"
#include "BoundaryCondition.h"

class PemSimulator{
public:
	PemSimulator(UDFManager* in,UDFManager* out);
	virtual ~PemSimulator();
	virtual void update();
protected:
	virtual void input();
	virtual void output();
	virtual void nan_check(double x,char c='0');
private:
	vector<Pendulum*> pendulum;
	vector<Body*> body;
	ExternalField* mField; // magnetic field
	ExternalField* gField; // gravity field
	vector<Torque*> torque;
	BoundaryCondition* bc;
	//virtual void field_test();
	double time;
	//
	UDFManager *in,*out;
	Timer* timer;
};

#endif // _PEM_SIMULATOR_H_
