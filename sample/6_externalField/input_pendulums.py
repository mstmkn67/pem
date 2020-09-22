from math import cos,sin,pi
import numpy as np

theta0=np.deg2rad(60)
alpha=np.deg2rad(30)
#
s,c=sin(alpha),cos(alpha)
$simulation.magnetiField.constantField.B=[c,s,0.0]
#
$simulation.pendulum[]=[]
s,c=sin(theta0),cos(theta0)
$simulation.pendulum[i].body=i
$simulation.pendulum[i].position=[0,0,0]
$simulation.pendulum[i].orientation=[[c,s,0],[-s,c,0],[0,0,1]]
$simulation.pendulum[i].angularVelocity=[0,0,0]
$simulation.pendulum[i].constraint.flag="false"



