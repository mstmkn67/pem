import numpy as np
from UDFManager import UDFManager
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

udf=UDFManager("pendulum_o.udf")
pos=[]
n=udf.totalRecord()
for i in range(n):
    udf.jump(i)
    p=np.array(udf.get("simulationResult.pendulum[0].magnet[0].position"))
    pos.append(p)
pos=np.array(pos)
#
fig = plt.figure(figsize=(9,5))
ax = fig.add_subplot(111, projection='3d')
ax.plot(pos[:,0],pos[:,1],pos[:,2],color="blue")
ax.scatter(pos[::10,0],pos[::10,1],pos[::10,2],color="black")
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.set_xlim(-4,4)
ax.set_ylim(-4,4)
ax.set_zlim(-12,2)
ax.set_title("Bob trajectory")
#
plt.tight_layout()
plt.savefig('./trajectory.png')
plt.show()
