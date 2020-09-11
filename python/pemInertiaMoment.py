import numpy as np
from UDFManager import *
import copy

def analyzer(udf,ibody):
    bloc=Location("body[]",[ibody])
    n=udf.size(bloc.str()+".shape[]")
    sloc=Location(bloc.str()+".shape[0]")
    IM=np.zeros((3,3))
    COM=np.zeros(3)
    MM=0.0
    for i in range(n):
        lpos=np.array(udf.get(sloc.str()+".center"))
        mass=np.array(udf.get(sloc.str()+".mass"))
        s=udf.get(sloc.str()+".shape")
        ii=np.zeros((3,3))
        if s=="sphere":
            a=udf.get(sloc.str()+".sphere.radius")
            ii=sphere(mass,a)
        elif s=="cylinder":
            a=udf.get(sloc.str()+".cylinder.radius")
            h=udf.get(sloc.str()+".cylinder.length")
            d=np.array(udf.get(sloc.str()+".cylinder.d"))
            ii=cylinder(mass,a,h,d)
        elif s=="ellipsoid":
            a=udf.get(sloc.str()+".ellipsoid.a")
            b=udf.get(sloc.str()+".ellipsoid.b")
            c=udf.get(sloc.str()+".ellipsoid.c")
            da=np.array(udf.get(sloc.str()+".ellipsoid.da"))
            db=np.array(udf.get(sloc.str()+".ellipsoid.db"))
            dc=np.cross(da,db)
            ii=ellipsoid(mass,a,b,c,da,db)
        elif s=="cuboid":
            a=udf.get(sloc.str()+".cuboid.a")
            b=udf.get(sloc.str()+".cuboid.b")
            c=udf.get(sloc.str()+".cuboid.c")
            da=np.array(udf.get(sloc.str()+".cuboid.da"))
            db=np.array(udf.get(sloc.str()+".cuboid.db"))
            dc=np.cross(da,db)
            ii=cuboid(mass,a,b,c,da,db)
        elif s=="line":
            l=udf.get(sloc.str()+".line.l")
            d=np.array(udf.get(sloc.str()+".line.d"))
            ii=line(mass,l,d)
        elif s=="point":
            ii=point(mass)
        elif s=="mesh":
            vertex=np.array(udf.get(sloc.str()+".mesh.vertex[].position"))
            face=udf.get(sloc.str()+".mesh.face[].vertex[]")
            ii=mesh(mass,vertex,face)
        IM+=translate(lpos,mass,ii)
        COM+=mass*lpos
        MM+=mass
        sloc.next() 
    udf.put(MM,bloc.str()+".analysis.mass")
    COM/=MM
    udf.put(COM.tolist(),bloc.str()+".analysis.massCenter")
    ii=[IM[0][0],IM[1][1],IM[2][2],IM[1][0],IM[2][0],IM[2][1]]
    udf.put(ii,bloc.str()+".analysis.inertiaMoment")

def sphere(m=1.0,a=1.0):
	return 0.4*m*a*a*np.eye(3)

def cylinder(m=1.0,a=1.0,h=3.0,d=np.array([0.0,0.0,1.0])):
	d=np.array(d)
	dd=np.outer(d,d)
	I=(0.25*a*a+h*h/48.)*(np.eye(3)-dd)
	I+=0.5*a*a*dd
	return m*I

def ellipsoid(m=1.0,a=1.0,b=2.0,c=3.0,da=np.array([1.0,0.0,0.0]),db=np.array([0.0,1.0,0.0])):
	da,db=np.array(da),np.array(db)
	dc=np.cross(da,db)
	I =(b*b+c*c)*np.outer(da,da)
	I+=(a*a+c*c)*np.outer(db,db)
	I+=(b*b+a*a)*np.outer(dc,dc)
	return 0.2*m*I

def cuboid(m=1.0,a=1.0,b=2.0,c=3.0,da=np.array([1.0,0.0,0.0]),db=np.array([0.0,1.0,0.0])):
	da,db=np.array(da),np.array(db)
	dc=np.cross(da,db)
	I =(b*b+c*c)*np.outer(da,da)
	I+=(a*a+c*c)*np.outer(db,db)
	I+=(b*b+a*a)*np.outer(dc,dc)
	return m*I/3.

def line(m=1.0,l=1.0,d=np.array([1.0,0.0,0.0])):
	return m*l*l/48*(np.eye(3)-np.outer(d,d))

def point(m=1.0):
	return np.zeros((3,3))

def mesh(m,vertex,face):
	vertex=np.array(vertex)
	volume=0.0
	I=np.zeros((3,3))
	for i in face:
		r0,r1,r2=vertex[i[0]],vertex[i[1]],vertex[i[2]]
		r=(r0+r1+r2)/3.0
		l2=r@r
		dr01,dr02=r1-r0,r2-r0
		n=np.cross(dr01,dr02)
		s=np.sqrt(n@n)
		n/=s
		s*=0.5
		volume+=r@n*s
		I+=r@n*(l2*np.eye(3)-np.outer(r,r))*s
	volume/=3
	I*=0.2*m/volume
	return I

def translate(r,m,I): #Steiner's formula
	return I+m*(r@r*np.eye(3)-np.outer(r,r))
