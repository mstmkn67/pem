import numpy as np

def outField(udf,name,no,sizeMin,sizeMax):
    nx,ny,nz=11,11,11
    dx=(sizeMax[0]-sizeMin[0])/(nx-1),
    dy=(sizeMax[1]-sizeMin[1])/(ny-1)
    dz=(sizeMax[2]-sizeMin[2])/(nz-1)
    file=open(udf.udfDirectory()+"/"+name+"%d.vtk"%(no),"w")
    file.write("# vtk DataFile Version 2.0\n")
    file.write("fem_field\n")
    file.write("ASCII\n")
    file.write("DATASET STRUCTURED_POINTS\n")
    file.write("DIMENSIONS %d %d %d\n"%(nx,ny,nz))
    file.write("ORIGIN %f %f %f\n"%(sizeMin[0],sizeMin[1],sizeMin[2]))
    file.write("SPACING %f %f %f\n\n"%(dx,dy,dz))
    file.write("POINT_DATA %d\n"%(nx*ny*nz))
    file.write("VECTORS gravity_field float\n")
    g=udf.get("simulationResult.gravity")
    for k in range(nz):
        for j in range(ny):
            for i in range(nx):
                file.write("{} {} {}\n".format(g[0],g[1],g[2]))
    file.write("VECTORS magnetic_field float\n")
    m=udf.get("simulationResult.magnetiField")
    for k in range(nz):
        for j in range(ny):
            for i in range(nx):
                file.write("{} {} {}\n".format(m[0],m[1],m[2]))
    file.close()

def outMagnet(udf,name,no,mn):
    position,dipole=[],[]
    ### pendulum
    ids=udf.get("simulation.pendulum[].body")
    poss=udf.get("simulation.pendulum[].position")
    frames=udf.get("simulationResult.pendulum[].orientation")
    for i,j,k in zip(ids,poss,frames):
        bloc=udf.getLocation("Body",i)
        c=udf.get(bloc+".magnet[].center")
        d=udf.get(bloc+".magnet[].dipole")
        for l,m in zip(c,d):
            position.append(j+l@k)
            dipole.append(m@k)
    ### body
    ids=udf.get("simulation.body[].body")
    poss=udf.get("simulationResult.body[].position")
    frames=udf.get("simulationResult.body[].orientation")
    for i,j,k in zip(ids,poss,frames):
        bloc=udf.getLocation("Body",i)
        c=udf.get(bloc+".magnet[].center")
        d=udf.get(bloc+".magnet[].dipole")
        for l,m in zip(c,d):
            position.append(j+l@k)
            dipole.append(m@k)
    ### vtk
    n=len(position)
    file=open(udf.udfDirectory()+"/"+name+"%d.vtk"%(no),"w")
    file.write("# vtk DataFile Version 2.0\n")
    file.write("fem_dipole\n")
    file.write("ASCII\n")
    file.write("DATASET UNSTRUCTURED_GRID\n")
    file.write("POINTS %d float\n"%(n))
    for i in position:
        file.write("%f %f %f\n"%(i[0],i[1],i[2]))
    file.write("\n")
    file.write("CELLS %d %d\n"%(n,2*n))
    for i in range(n):
        file.write("%d %d\n"%(1,i))
    file.write("\n")
    file.write("CELL_TYPES %d\n"%(n))
    for i in range(n):
        file.write("%d\n"%(1))
    file.write("\n")
    file.write("POINT_DATA %d\n"%(n))
    file.write("VECTORS dipole float\n")
    for i in dipole:
        file.write("%f %f %f\n"%(i[0],i[1],i[2]))

def outPendulum(udf,name,no):
    position=[]
    ### pendulum
    ids=udf.get("simulation.pendulum[].body")
    poss=udf.get("simulation.pendulum[].position")
    frames=udf.get("simulationResult.pendulum[].orientation")
    for i,j,k in zip(ids,poss,frames):
        bloc=udf.getLocation("Body",i)
        c=udf.get(bloc+".analysis.massCenter")
        position.append(j+c@k)

def outBody(udf,name,no):
    position=[]
    ### body
    ids=udf.get("simulation.body[].body")
    poss=udf.get("simulationResult.body[].position")
    frames=udf.get("simulationResult.body[].orientation")
    for i,j,k in zip(ids,poss,frames):
        bloc=udf.getLocation("Body",i)
        c=udf.get(bloc+".analysis.massCenter")
        position.append(j+c@k)

def outMesh(udf,name,no):
    print(1)


def outShape(udf,name,no):
    print(1)
