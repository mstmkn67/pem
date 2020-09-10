import numpy as np

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
