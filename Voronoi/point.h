#include <GL/glut.h>

class Point3{

public:
    double x,y,z;
    Point3(){
        x=0; y=0; z=0;
    }
    Point3(double xx, double yy, double zz){
        x=xx;
        y=yy;
        z=zz;
    }
};
