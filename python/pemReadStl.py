from stl import mesh
from MMesh5 import reduce_vertex
import numpy as np

def read_stl(fileName):
    smesh=mesh.Mesh.from_file(fileName)
    volume, cog, inertia= smesh.get_mass_properties()
    vertex=[]
    face=[]
    for i in smesh.vectors:
        for j in i:
            a=np.array(j)-np.array(cog)
            vertex.append(j)
    vertex,face=reduce_vertex(vertex)
    return vertex,face

if __name__ == '__main__':
    file_name="cube.stl"
    vertex,face=read_stl(file_name)
    print(vertex)
    print(face)
