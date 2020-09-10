import numpy as np
#
dt=$simulation.time.dt*$simulation.time.reportSteps
with open(udfDirectory()+"/angle.dat","w") as f:
	for i in range(totalRecord()+1):
		jump(i)
		t=dt*i
		u3x=np.array($simulationResult.pendulum[].orientation.u3.x)
		u3z=np.array($simulationResult.pendulum[].orientation.u3.z)
		theta=np.arctan2(-u3x,u3z)
		theta=np.rad2deg(theta)
		data=str(t)+" "
		for i in theta:
			data+=str(i)
			data+=" " 
		f.write(data+"\n")

