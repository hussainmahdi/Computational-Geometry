#include <iostream>
#include <cstdio>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <time.h>

#include "point.h"
#include "triangle.h"
#include "edge.h"

#define pi 2*acos(0)
#define PIXEL_X 30
#define PIXEL_Y 30
#define INC 0.05
#define SITES 50
#define MAX_P 25.0
#define MIN_P 5.0
#define SC 0
#define SF 1

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

vector<Point3> sites;


Point3 vertices[3*SITES];
Point3 infinites[3*SITES];
Edge dtEdges[SITES][SITES];
Edge vrEdges[SITES][SITES];

int nSites;
int nInfinites=0, nVertices=0;

void drawPoints() {

    glPushMatrix();{
        glPointSize(5.0f);
        glBegin(GL_POINTS);{
            glColor3f(1,0,0);
            for(int i= 0; i<nSites; i++){
                glVertex3f(SF*sites[i].x, SF*sites[i].y, 0);
            }
            glColor3f(1,1,1);
            for(int i= 0; i< nVertices; i++){
                glVertex3f(SF*vertices[i].x,  SF*vertices[i].y, 0);
            }
        }glEnd();
    }glPopMatrix();
}

void drawEdges(){

    glBegin(GL_LINES);{
        glColor3f(0,0,1);
        for(int i=0; i<nSites-1; i++)
        for(int j=i+1; j<nSites; j++){
            glLineWidth(3.0f);
            glColor3f(1,1,1);

            if(vrEdges[i][j].type >= 1){// 1 or 2
               // if(vrEdges[i][j].idx == 2){//2
                glVertex3f(SF*vrEdges[i][j].startP.x, SF*vrEdges[i][j].startP.y, 0);
                    //cout<<vrEdges[i][j].start->X<<" "<< vrEdges[i][j].start->Y<<" 0"<<endl;//?
                glVertex3f(SF*vrEdges[i][j].endP.x, SF*vrEdges[i][j].endP.y, 0);
                       // cout<<vrEdges[i][j].end->X<<" "<<vrEdges[i][j].end->Y<<" 0"<<endl;//?
               // }
            }
            glLineWidth(3.0f);
            glColor3f(0.5,0,1.0);
            if(dtEdges[i][j].type == 1){//
                glVertex3f(dtEdges[i][j].startP.x, dtEdges[i][j].startP.y, 0);
                    //cout<<d_edges[i][j].start->X<<" "<< d_edges[i][j].start->Y<<" 0"<<endl;//?
                glVertex3f(dtEdges[i][j].endP.x, dtEdges[i][j].endP.y, 0);
                   // cout<<d_edges[i][j].end->X<<" "<<d_edges[i][j].end->Y<<" 0"<<endl;//?
            }
        }

    }glEnd();
}

void drawAxes() {
	if(drawaxes==1)
	{
		//glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawGrid() {
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to x-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
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
			cameraHeight -= 5.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 5.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.3;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.3;
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
	//gluLookAt(1000*cos(cameraAngle), 1000*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);

    gluLookAt(500,500,2000,	500,500,0,	0,1,0);
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	glColor3f(1,0,0);

    drawEdges();
    drawPoints();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	angle+=0.5;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=1500.0;
	cameraAngle=4.0;
	angle=0;

    nSites=rand()%SITES+1;
    nInfinites=0;
    nVertices=0;

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
	gluPerspective(100,	1,	1,	10000.0);
	//field of view in the y (vertically)
	//aspect ratio that determines the field of view in the x direction (horizontally)
	//near distance
	//far distance
}

int isSameSide(Point3 p, Point3 q, Point3 r){

    int side = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (side == 0)      return 0; // colinear
    return (side > 0) ? 1 : 2;    // clock or counterclock wise
}


void setEdge(int i, int j, int k){

    //cout<< "OK...\n";
    if(vrEdges[i][j].type==0)
    {
        //printf("%d\n", vrEdges[i][j].type);
        vrEdges[i][j].startP = vertices[nVertices];
        vrEdges[i][j].type++;

        infinites[nInfinites].x = (sites[i].x + sites[j].x) / 2.0;
        infinites[nInfinites].y = (sites[i].y + sites[j].y) / 2.0;
        if(isSameSide(sites[i], sites[j], vertices[nVertices]) == isSameSide(sites[i], sites[j], sites[k]))
        {
            infinites[nInfinites].x = vertices[nVertices].x +
                                      200*(infinites[nInfinites].x - vertices[nVertices].x);
            infinites[nInfinites].y = vertices[nVertices].y +
                                      200*(infinites[nInfinites].y - vertices[nVertices].y);
            vrEdges[i][j].endP = infinites[nInfinites] ;
        }
        else
        {
            infinites[nInfinites].x = infinites[nInfinites].x +
                                      200*(vertices[nVertices].x - infinites[nInfinites].x);
            infinites[nInfinites].y = infinites[nInfinites].y +
                                      200*(vertices[nVertices].y - infinites[nInfinites].y);
            vrEdges[i][j].endP = infinites[nInfinites] ;
        }
        nInfinites++;
    }
    else{

        printf("%d\n", vrEdges[i][j].type);
        vrEdges[i][j].endP = vertices[nVertices];
        vrEdges[i][j].type++;//2
    }
}


void makeVoronoi(int i, int j, int k){

    double a,b,c,d,e,f,g;

    a = sites[j].x-sites[i].x;
    b = sites[j].y-sites[i].y;
    c = sites[k].x-sites[i].x;
    d = sites[k].y-sites[i].y;
    e = a*(sites[i].x + sites[j].x) + b*(sites[i].y + sites[j].y);
    f = c*(sites[i].x + sites[k].x) + d*(sites[i].y + sites[k].y);
    g = 2*(a*(sites[k].y - sites[j].y) - b*(sites[k].x - sites[j].x));

    cout << "OK ...\n";
    if(g==0){;}
    /*else*/
    else
    {
        cout<< "OK ... \n";
        vertices[nVertices].x = (d*e - b*f)/g;
        cout<< "OK ... \n";
        vertices[nVertices].y = (a*f - c*e)/g;
        //printf("v_point\[%d\]\t%f\t%f\n", vi,v_points[vi].x,v_points[vi].y);
        //set up vonoroi vrEdges
        setEdge(i,j,k);
        setEdge(j,i,k);
        setEdge(i,k,j);
        setEdge(k,i,j);
        setEdge(j,k,i);
        setEdge(k,j,i);

        nVertices++;
    }
}

void makeDlTriangle(){

    double xn, yn, zn;
    int flag;
    for(int i=0; i<nSites-2; i++)
    for(int j=i+1; j<nSites; j++)
    for(int k=i+1; k<nSites; k++)
    if(j!=k){
        xn=(sites[j].y-sites[i].y)*(sites[k].z-sites[i].z) -
           (sites[k].y-sites[i].y)*(sites[j].z-sites[i].z);
        yn=(sites[k].x-sites[i].x)*(sites[j].z-sites[i].z) -
           (sites[j].x-sites[i].x)*(sites[k].z-sites[i].z);
        zn=(sites[j].x-sites[i].x)*(sites[k].y-sites[i].y) -
           (sites[k].x-sites[i].x)*(sites[j].y-sites[i].y);

        if(flag = (zn<0))
            for(int m=0; m<nSites; m++)
                flag=flag&&
                    (((sites[m].x-sites[i].x)*xn +
                     (sites[m].y-sites[i].y)*yn +
                     (sites[m].z-sites[i].z)*zn) <= 0);

        if(flag) {
            //Triangle triangle(sites[i], sites[j], sites[k]);
            //triangles.push_back(triangle);
            cout << i << " " << j << " " << k <<endl;

            makeVoronoi(i,j,k);
//            cout << sites[i].x << " " << sites[i].y << " " << sites[i].z <<endl;
//            cout << sites[j].x << " " << sites[j].y << " " << sites[k].z <<endl;
//            cout << sites[k].x << " " << sites[k].y << " " << sites[k].z <<endl;

            dtEdges[i][j].type =1;
            dtEdges[i][j].startP = sites[i];
            dtEdges[i][j].endP = sites[j];

            dtEdges[j][k].type =1;
            dtEdges[j][k].startP = sites[j];
            dtEdges[j][k].endP = sites[k];

            dtEdges[i][k].type =1;
            dtEdges[i][k].startP = sites[i];
            dtEdges[i][k].endP = sites[k];

        }
    }
}

bool comp(Point3 a, Point3 b){
    return ((a.y==b.y)?(a.x<b.x):(a.y>b.y));
}
void createSites(){
    Point3 site;
    FILE *file;
    file = fopen ("E:\\Learn Sth\\L-4,T-2\\Computational Geometry\\Voronoi\\input1.txt","r");
    if(file==NULL)cout<<"No file created\n";

    fscanf(file,"%d",&nSites); cout<<nSites<<endl;

    for(int i=0; i<nSites; ++i){
        fscanf (file, "%lf %lf", &site.x,&site.y);
        site.x*=20;
        site.y*=20;
        site.z = site.x*site.x + site.y*site.y;
        sites.push_back(site);
    }


//    srand(time(NULL));
//	if(nSites<8) nSites=15;
//    cout << "Creating sites...\n";
//	for(int i=0; i<nSites; ++i)
//    {
//        site.x= MIN_P + static_cast <double> (rand()) / static_cast <double> (RAND_MAX/(MAX_P-MIN_P));
//        site.y= MIN_P + static_cast <double> (rand()) / static_cast <double> (RAND_MAX/(MAX_P-MIN_P));
//        site.z= site.x*site.x + site.y*site.y;
//
//        cout << site.x << " " << site.y << " " << site.z << endl;
//        sites.push_back(site);
//    }

    cout<<nSites<<" sites created"<<endl;
    //sort(sites.begin(), sites.end(), comp);
}

int main(int argc, char *argv[])
{
    createSites();
    makeDlTriangle();
    //makeVoronoi();

    glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Voronoi Diagram");

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
