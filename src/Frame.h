#ifndef _FRAME_H_
#define _FRAME_H_

#include "Vector3d.h"
#include <cmath>
using namespace std;

class Frame{
public:
    Frame();
    Frame(const Vector3d& u1,const Vector3d& u2);
    virtual ~Frame();
    //
    virtual void update();
    virtual Vector3d operator()(int i)const;
    Vector3d u1,u2,u3;
protected:
    virtual void correct1();
    virtual void correct2();
private:
};

#endif // _FRAME_H_
