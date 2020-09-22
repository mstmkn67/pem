import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.special import ellipk,ellipj


def const_k(theta0,alpha):
    return np.sin(0.5*(theta0-alpha))

def period(omega,k):
    return 4*ellipk(k*k)/omega

def angle(t,omega,k,T,alpha):
    u=omega*(t+0.25*T)
    sn=k*ellipj(u,k*k)[0]
    return np.rad2deg(2*np.arcsin(sn)+alpha)


theta0,alpha=np.deg2rad(60),np.deg2rad(30)
p,B,I=1.0,1.0,2.0
omega=np.sqrt(p*B/I)
fig=plt.figure(figsize=(12,9))
ax=plt.subplot(111)
df=pd.read_csv("angle.dat",sep=' ', header=None)
ax.plot(df.loc[::10,0],df.loc[::10,1],label="simulation",linestyle="",marker='o',color="red")
k=const_k(theta0,alpha)
T=period(omega,k)
theta=angle(df.loc[:,0].values,omega,k,T,alpha)
ax.plot(df.loc[:,0],theta,label="analytic solution",linestyle="-",color="red")
ax.legend()
ax.set_xlabel("Time")
ax.set_ylabel("angle")
ax.set_title("Analytic solutions(line) and simulation results(symbols)")
plt.tight_layout()
plt.savefig('./time_angle.png')
plt.show()

