#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>


class Triangle{
public:
    Point3 a,b,c;

    Triangle(){}
    Triangle(Point3 aa, Point3 bb, Point3 cc){
        a=aa;
        b=bb;
        c=cc;
    }
};
