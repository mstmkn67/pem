import numpy as np

s=1./np.sqrt(2.)
frame=np.array([[s,s,0.0],
                [-s,s,0.0],
                [0.0,0.0,1.0]])

a=np.array([1.0,0.0,0.0])
print(a@frame)
