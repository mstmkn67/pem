#include "udf/gourmain.h"
#include "PemSimulator.h"

void udfHeaderCheck()
{
	string version("1.3"),engine("pem");
	cout << "**************************************************************" << endl;
	cout <<  "              " <<  engine << "  " << version << "           " << endl;
	cout << "                                        Masato MAKINO         " << endl;
	cout << "**************************************************************" << endl;
	cout <<  endl;
}

void error_massage(){
	cout << "usage: pem -I input_udf [-O output_udf] " << endl;
}


int gourmain(UDFManager* in,UDFManager* out){
	udfHeaderCheck();
	PemSimulator* sim=new PemSimulator(in,out);
	sim->update();
	delete sim;
	return 0;
}
