import numpy as np

def draw_dipoles(udf,r,d,mag,color):
    for i,j in zip(r,d):
        p,v=np.array(i),np.array(j)
        s=np.sqrt(v@v)
        att=color+[0.2*s,0.2*s,mag]
        udf.arrow(p.tolist(),(p+v).tolist(),att)

def draw_frame(udf,smin,max,color=[1,1,1,1]):
	size=[max[0]-smin[0],max[1]-smin[1],max[2]-smin[2]]
	r0=[smin[0]        ,smin[1]        ,smin[2]]
	r1=[smin[0]+size[0],smin[1]        ,smin[2]]
	r2=[smin[0]+size[0],smin[1]+size[1],smin[2]]
	r3=[smin[0]        ,smin[1]+size[1],smin[2]]
	r4=[smin[0]        ,smin[1]        ,smin[2]+size[2]]
	r5=[smin[0]+size[0],smin[1]        ,smin[2]+size[2]]
	r6=[smin[0]+size[0],smin[1]+size[1],smin[2]+size[2]]
	r7=[smin[0]        ,smin[1]+size[1],smin[2]+size[2]]
	udf.polyline([r0,r1,r2,r3,r0],color)
	udf.polyline([r4,r5,r6,r7,r4],color)
	udf.line(r0,r4,color)
	udf.line(r1,r5,color)
	udf.line(r2,r6,color)
	udf.line(r3,r7,color)

def draw_cuboid(udf,r,color=[1,1,1,1]):
	udf.polygon([r[0],r[1],r[2],r[3],r[0]],color)
	udf.polygon([r[4],r[5],r[6],r[7],r[4]],color)
	udf.polygon([r[0],r[1],r[5],r[4],r[0]],color)
	udf.polygon([r[1],r[2],r[6],r[5],r[1]],color)
	udf.polygon([r[2],r[3],r[7],r[6],r[2]],color)
	udf.polygon([r[3],r[0],r[4],r[7],r[3]],color)

def show_sphere(udf,gpos,lpos,frame,a,color):
    p=gpos+lpos@frame
    udf.sphere(p.tolist(),color+[a])

def show_cylinder(udf,gpos,lpos,frame,a,h,d,color):
    d/=np.sqrt(d@d)
    #p=gpos+lpos@frame #center
    p1=gpos+(lpos+h*d)@frame
    p2=gpos+(lpos-h*d)@frame
    udf.cylinder(p1.tolist(),p2.tolist(),color+[-a])

def show_ellipsoid(udf,gpos,lpos,frame,a,b,c,da,db,dc,color):
    p=gpos+lpos@frame
    udf.ellipsoid1(p.tolist(),color+[a,b,c]+(da@frame).tolist()+(dc@frame).tolist())

def show_cuboid(udf,gpos,lpos,frame,a,b,c,da,db,dc,color):
    da/=np.sqrt(da@da)
    db/=np.sqrt(db@db)
    dc/=np.sqrt(dc@dc)
    pos=[]
    pos.append((gpos+(lpos-a*da-b*db-c*dc)@frame).tolist())
    pos.append((gpos+(lpos+a*da-b*db-c*dc)@frame).tolist())
    pos.append((gpos+(lpos+a*da+b*db-c*dc)@frame).tolist())
    pos.append((gpos+(lpos-a*da+b*db-c*dc)@frame).tolist())
    pos.append((gpos+(lpos-a*da-b*db+c*dc)@frame).tolist())
    pos.append((gpos+(lpos+a*da-b*db+c*dc)@frame).tolist())
    pos.append((gpos+(lpos+a*da+b*db+c*dc)@frame).tolist())
    pos.append((gpos+(lpos-a*da+b*db+c*dc)@frame).tolist())
    draw_cuboid(udf,pos,color)

def show_line(udf,gpos,lpos,frame,l,d,color):
    p1=gpos+(lpos+l*d)@frame
    p2=gpos+(lpos-l*d)@frame
    udf.line(p1.tolist(),p2.tolist(),color)

def show_point(udf,gpos,lpos,color):
    udf.point((gpos+lpos).tolist(),color)

def show_mesh(udf,gpos,lpos,frame,vertex,face,color):
    vpos=[]
    for i in vertex:
        p=gpos+(lpos+i)@frame
        vpos.append(p.tolist())
    for i in face:
        udf.polygon([vpos[i[0]],vpos[i[1]],vpos[i[2]]],color)

def show_mesh_normal(udf,gpos,lpos,frame,vertex,face,color):
    vpos=[]
    for i in vertex:
        p=gpos+(lpos+i)@frame
        vpos.append(p)
    for i in face:
        tri=[vpos[i[0]].tolist(),vpos[i[1]].tolist(),vpos[i[2]].tolist()]
        udf.polygon(tri,color)
        r=(vpos[i[0]]+vpos[i[1]]+vpos[i[2]])/3.0
        dr01,dr02=vpos[i[1]]-vpos[i[0]],vpos[i[2]]-vpos[i[0]]
        n=np.cross(dr01,dr02)
        n/=np.sqrt(n@n)
        udf.line(r.tolist(),(r+n).tolist(),1)



####################
def show_body(udf,body_id,gpos,frame,normal=False):
    bloc=udf.getLocation("Body",body_id)
    n=udf.size(bloc+".shape[]")
    nc=udf.size(bloc+".color[]")
    color=[1,0,0,1]
    if nc==n:
        colors=udf.get(bloc+".color[]")
    elif nc>1:
        color=udf.get(bloc+".color[0]")
    for i in range(n):
        sloc=bloc+".shape[{}]".format(i)
        lpos=np.array(udf.get(sloc+".center"))
        if nc==n:
            color=colors[i]
        s=udf.get(sloc+".shape")
        if s=="sphere":
            a=udf.get(sloc+".sphere.radius")
            show_sphere(udf,gpos,lpos,frame,a,color)
        elif s=="cylinder":
            a=udf.get(sloc+".cylinder.radius")
            h=udf.get(sloc+".cylinder.length")
            d=np.array(udf.get(sloc+".cylinder.d"))
            show_cylinder(udf,gpos,lpos,frame,a,h,d,color)
        elif s=="ellipsoid":
            a=udf.get(sloc+".ellipsoid.a")
            b=udf.get(sloc+".ellipsoid.b")
            c=udf.get(sloc+".ellipsoid.c")
            da=np.array(udf.get(sloc+".ellipsoid.da"))
            db=np.array(udf.get(sloc+".ellipsoid.db"))
            dc=np.cross(da,db)
            show_ellipsoid(udf,gpos,lpos,frame,a,b,c,da,db,dc,color)
        elif s=="cuboid":
            a=udf.get(sloc+".cuboid.a")
            b=udf.get(sloc+".cuboid.b")
            c=udf.get(sloc+".cuboid.c")
            da=np.array(udf.get(sloc+".cuboid.da"))
            db=np.array(udf.get(sloc+".cuboid.db"))
            dc=np.cross(da,db)
            show_cuboid(udf,gpos,lpos,frame,a,b,c,da,db,dc,color)
        elif s=="line":
            l=udf.get(sloc+".line.l")
            d=np.array(udf.get(sloc+".line.d"))
            show_line(udf,gpos,lpos,frame,l,d,color)
        elif s=="point":
            show_point(udf,gpos,lpos,color)
        elif s=="mesh":
            vertex=np.array(udf.get(sloc+".mesh.vertex[].position"))
            face=udf.get(sloc+".mesh.face[].vertex[]")
            if normal:
                show_mesh_normal(udf,gpos,lpos,frame,vertex,face,color)
            else:
                show_mesh(udf,gpos,lpos,frame,vertex,face,color)
