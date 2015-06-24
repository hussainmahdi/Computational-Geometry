#include <GL/glut.h>

class Edge{
public:
    Point3 startP;
    Point3 endP;

    int type;
    Edge(){type=0;}
};
