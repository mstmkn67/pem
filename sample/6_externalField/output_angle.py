import numpy as np
#
dt=$simulation.time.dt*$simulation.time.reportSteps
with open(udfDirectory()+"/angle.dat","w") as f:
    for i in range(totalRecord()+1):
        jump(i)
        t=dt*i
        u1x=np.array($simulationResult.pendulum[0].orientation.u1.x)
        u1y=np.array($simulationResult.pendulum[0].orientation.u1.y)
        theta=np.arctan2(u1y,u1x)
        theta=np.rad2deg(theta)
        print("{} {}".format(t,theta),file=f)


