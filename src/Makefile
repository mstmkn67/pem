CC = g++
CFLAG = -c -O3 -I $(PF_FILES)/include
CCFLAG = -o pem  -L $(PF_FILES)/lib/linux_64 -lplatform -llapack
OBJECT= Vector3d.o  \
	Timer.o gourmain.o cmdlineopts.o LapackFunctions.o \
	Pendulum.o Body.o Frame.o ExternalField.o \
	Torque.o BoundaryCondition.o \
	PemSimulator.o pem_driver.o

all :$(OBJECT)
	$(CC) $(OBJECT) $(CCFLAG)

Vector3d.o : Vector3d.cpp Vector3d.h
	$(CC) Vector3d.cpp $(CFLAG)

Timer.o : Timer.cpp Timer.h
	$(CC) Timer.cpp $(CFLAG)

gourmain.o : udf/gourmain.cpp udf/gourmain.h
	$(CC) udf/gourmain.cpp $(CFLAG)

cmdlineopts.o : udf/cmdlineopts.cpp udf/cmdlineopts.h
	$(CC) udf/cmdlineopts.cpp $(CFLAG)

LapackFunctions.o : solver/LapackFunctions.h solver/LapackFunctions.cpp
	$(CC) solver/LapackFunctions.cpp $(CFLAG)

Pendulum.o : Pendulum.cpp Pendulum.h
	$(CC) Pendulum.cpp $(CFLAG)

Body.o : Body.cpp Body.h
	$(CC) Body.cpp $(CFLAG)

Frame.o : Frame.cpp Frame.h
	$(CC) Frame.cpp $(CFLAG)

ExternalField.o : ExternalField.cpp ExternalField.h
	$(CC) ExternalField.cpp $(CFLAG)

Torque.o : Torque.cpp Torque.h
	$(CC) Torque.cpp $(CFLAG)

BoundaryCondition.o : BoundaryCondition.cpp BoundaryCondition.h
	$(CC) BoundaryCondition.cpp $(CFLAG)

PemSimulator.o : PemSimulator.cpp PemSimulator.h
	$(CC) PemSimulator.cpp $(CFLAG)

pem_driver.o : pem_driver.cpp
	$(CC) pem_driver.cpp $(CFLAG)

clean : 
	rm -f *.o pem

install :
	strip pem
	cp pem ../bin
