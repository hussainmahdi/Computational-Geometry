#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<algorithm>
#include<GL/glut.h>

#define pi 2*acos(0)
using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;


struct Point{
    double x, y;
};

vector<Point> convexHull;
vector<Point> points;

bool cmp(Point a, Point b){
    return (a.x<b.x || (a.x==b.x && a.y<b.y));
}

double findArea(Point A, Point B, Point C){
    return (A.x*C.y)+(C.x*B.y)+(B.x*A.y)-(C.x*A.y)-(C.y*B.x)-(B.y*A.x);
}

vector<Point> rightSidePoints(Point A, Point B){

    vector<Point> rightPoints;
    Point C;
    for(int i=0; i<points.size(); i++){
        C = points[i];
        if((C.x==A.x && C.y==A.y) || (C.x==B.x && C.y==B.y)) continue;
        if(findArea(A,B,C) > 0)
            rightPoints.push_back(C);
    }
    return rightPoints;
}

void quickHull(Point A, Point B, vector<Point> pointsAB){

    if(pointsAB.size()==0) return;

    //cout << "farthest point " << C.x << " " << C.y << endl;

    double highestDist=0.0;
    double area=0;
    Point C;
    Point farthestPoint;
    for(int i=0; i<pointsAB.size(); i++){
        C = pointsAB[i];
        //if((C.x==A.x && C.y==A.y) || (C.x==B.x && C.y==B.y)) continue;
        area = findArea(A,B,C);
        if(highestDist<=area){
            highestDist = area;
            farthestPoint = C;
        }
    }

    cout << "Farthest Point" << " " << farthestPoint.x << " " << farthestPoint.y<<endl;
    vector<Point> pointsAC = rightSidePoints(A,farthestPoint);
    vector<Point> pointsCB = rightSidePoints(farthestPoint,B);
    quickHull(A,C, pointsAC);
    convexHull.push_back(farthestPoint);
    quickHull(C,B, pointsCB);

    return;
}

void findConvexHull(){

    Point A = points[0];
    Point B = points[points.size()-1];

    convexHull.push_back(A);

    vector<Point> pointsAB = rightSidePoints(A,B);
    vector<Point> pointsBA = rightSidePoints(B,A);

    quickHull(A,B, pointsAB);
    convexHull.push_back(B);
    quickHull(B,A, pointsBA);

    return;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);//grey

		glBegin(GL_LINES); {
            for(i=-8; i<=8; i++)
            {
                if(i==0) continue;
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10, 90, 0);

                glVertex3f(-90, i*10, 0);
                glVertex3f(90, i*10, 0);
            }

		}glEnd();
	}
}

void drawPoints(){

    glPointSize(1.0);
    glBegin(GL_POINTS); {

        for(int i=0; i<points.size(); i++)
        {
            glColor3d( 0.6, 0.6, 0.6 );
            glVertex3d(points[i].x, points[i].y, 0);
        }

    }glEnd();
}

void drawLines(){

    glBegin(GL_LINES); {

        for(int i=0; i<convexHull.size()-1; i++)
        {
            glColor3d( 0.6, 0.6, 0.6 );
            glVertex3d(10*convexHull[i].x, 10*convexHull[i].y, 0);
            glVertex3d(10*convexHull[i+1].x, 10*convexHull[i+1].y, 0);
        }

    }glEnd();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	glColor3f(0.6,0.6,0.6);


    drawPoints();
    drawLines();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=1.0;


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:

			break;
		case GLUT_KEY_PAGE_DOWN:

			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Convex Hull");

	Point spoint;
    for(int i=0; i<7; i++){

        //srand(time(NULL));
        scanf("%lf %lf", &spoint.x, &spoint.y);

//        spoint.x = rand()%100 + 1;
//        //srand(time(spoint.x));
//        spoint.y = rand()%100 + 1;

        points.push_back(spoint);
    }
    sort(points.begin(), points.end(), cmp);
    for(int i=0; i<points.size(); i++)
            printf("%lf", points[i].x);
    findConvexHull();
    printf("-------\n");
    for(int i=0; i<points.size(); i++)
            printf("%lf", points[i].x);
    //sort(convexHull.begin(), convexHull.end(), cmp);

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
