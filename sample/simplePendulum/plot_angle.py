import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.special import ellipk,ellipj

def const_k_linear(g,l,theta0):
	E=0.5*g*theta0**2/l
	k2=0.5*l*E/g
	return np.sqrt(k2)

def const_k_exact(g,l,theta0):
	E=g*(1.0-np.cos(theta0))/l
	k2=0.5*l*E/g
	return np.sqrt(k2)

def period_linear(l,g):
	return 2*np.pi*np.sqrt(l/g)

def angle_linear(t,k,T,l,g):
	return np.rad2deg(2*k*np.sin(np.sqrt(g/l)*(t+0.25*T)))

def period_exact(l,g,k):
	return 4*np.sqrt(l/g)*ellipk(k*k)

def angle_exact(t,k,T,l,g):
	u=np.sqrt(g/l)*(t+0.25*T)
	sn=k*ellipj(u,k*k)[0]
	return np.rad2deg(2*np.arcsin(sn))

colors=['red', 'black', 'blue', 'brown', 'green']

l,g=5.0,1.0
theta0s=[30,60,90,120,150]
fig=plt.figure(figsize=(12,9))
ax=plt.subplot(211)
df=pd.read_csv("angle.dat",sep=' ', header=None)
for i,(theta0,color) in enumerate(zip(theta0s,colors),1):
	ax.plot(df.loc[::10,0],df.loc[::10,i],label=str(theta0)+" simulation",linestyle="",marker='o',color=color)
	k=const_k_linear(g,l,np.deg2rad(theta0))
	T=period_linear(l,g)
	theta=angle_linear(df.loc[:,0].values,k,T,l,g)
	ax.plot(df.loc[:,0],theta,label=str(theta0)+" linear solution",linestyle="-",color=color)
ax.legend()
ax.set_xlabel("Time")
ax.set_ylabel("angle")
#
ax=plt.subplot(212)
for i,(theta0,color) in enumerate(zip(theta0s,colors),1):
	ax.plot(df.loc[::10,0],df.loc[::10,i],label=str(theta0)+" simulation",linestyle="",marker='o',color=color)
	k=const_k_exact(g,l,np.deg2rad(theta0))
	T=period_exact(l,g,k)
	theta=angle_exact(df.loc[:,0].values,k,T,l,g)
	ax.plot(df.loc[:,0],theta,label=str(theta0)+" exact solution",linestyle="-",color=color)
ax.legend()
ax.set_xlabel("Time")
ax.set_ylabel("angle")
plt.savefig('./time_angle.png')
plt.show()

