from math import cos,sin,pi
dy=3
thetas=[30,60,90,120,150]
$simulation.pendulum[]=[]
for i,theta in enumerate(thetas):
	$simulation.pendulum[i].body=0
	$simulation.pendulum[i].position=[0,i*dy,0]
	theta2=theta*pi/180
	s,c=sin(theta2),cos(theta2)
	$simulation.pendulum[i].orientation=[[c,0,s],[0,1,0],[-s,0,c]]
	$simulation.pendulum[i].angularVelocity=[0,0,0]
	$simulation.pendulum[i].constraint.flag="false"



