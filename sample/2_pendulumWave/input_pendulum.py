from math import cos,sin,pi
import numpy as np
from scipy.special import ellipk
import pemInertiaMoment as pim

def const_k_exact(theta0):
	k2=0.5*(1-np.cos(theta0))
	return np.sqrt(k2)

def length_linear(g,T):
	return g*(0.5*T/pi)**2

def legnth_exact(g,T,k):
	return g*T**2/16./ellipk(k*k)**2


dy=0.1
theta0=np.deg2rad(150)
k=const_k_exact(theta0)
#period
gamma=20.0
N,Np=20,15
Ts=[gamma/i for i in range(N,N+15)]
#
g=10.0
##
$body[]=[]
for i,T in enumerate(Ts):
	$body[i].id=i
	l=length_linear(g,T)
	#l=legnth_exact(g,T,k)
	#shape[0]
	$body[i].shape[0].center=[0,0,-l]
	$body[i].shape[0].mass=1.0
	$body[i].shape[0].shape="point"
	#shape[1] dummy
	$body[i].shape[1].center=[0,0,-l]
	$body[i].shape[1].mass=0.0
	$body[i].shape[1].shape="sphere"
	$body[i].shape[1].sphere.radius=0.03
	#shape[2] dummy
	$body[i].shape[2].center=[0,0,-0.5*l]
	$body[i].shape[2].mass=0.0
	$body[i].shape[2].shape="line"
	$body[i].shape[2].line.l=0.5*l
	$body[i].shape[2].line.d=[0,0,1]
	#color
	$body[i].color[0]=[1,1,1,1]
	if i%2==0:
		$body[i].color[1]=[1,0,0,1]
	else:
		$body[i].color[1]=[0,0,1,1]
	$body[i].color[2]=[0,0,0,1]
##
for i in range(len(Ts)):
	pim.analyzer(_udf_,i)
##
$simulation.pendulum[]=[]
s,c=sin(theta0),cos(theta0)
for i,theta in enumerate(Ts):
	$simulation.pendulum[i].body=i
	$simulation.pendulum[i].position=[0,i*dy,0]
	$simulation.pendulum[i].orientation=[[c,0,s],[0,1,0],[-s,0,c]]
	$simulation.pendulum[i].angularVelocity=[0,0,0]
	$simulation.pendulum[i].constraint.flag="false"



