#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#define EPS 0.000000001
#define MAX 2147483647.0
#define MIN -2147483648.0

#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<queue>
#include<stack>

#define START_V 0
#define END_V 1
#define SPLIT_V 2
#define MERGE_V 3
#define REGULAR_V 4

using namespace std;

struct Point{
    double x,y;
    int id;

    Point(){}
     Point(double xx, double yy){
        this->x=xx;
        this->y=yy;
        this->id=-1;
    }
    Point(double xx, double yy, int index){
        this->x=xx;
        this->y=yy;
        this->id=index;
    }
    Point(const Point &p){
        this->x=p.x;
        this->y=p.y;
        this->id=p.id;
    }

    bool operator()(const Point &p1, const Point &p2) const
    {
        return ((p1.y==p2.y)?(p1.x>p2.x):(p1.y<p2.y));
    }
    bool operator>(const Point &p1)
    {
        return ((this->y==p1.y)?(p1.x>this->x):(p1.y<this->y));
    }
    bool operator<(const Point &p1)
    {
        return ((this->y==p1.y)?(p1.x<this->x):(p1.y>this->y));
    }
};

struct Edge{
    Point a,b;
    int id;
    double key;

    Edge(){}
    Edge(Point aa, Point bb){
        this->a=aa;
        this->b=bb;
        this->id=-1;
        this->key=0;
    }
    Edge(Point aa, Point bb, int index){
        this->a=aa;
        this->b=bb;
        this->id=index;
        this->key=0;
    }
    Edge(Point aa, Point bb, double curY){
        this->a=aa;
        this->b=bb;
        this->id=-1;
        setKey(curY);
    }
    Edge(Point aa, Point bb, int index, double curY){
        this->a=aa;
        this->b=bb;
        this->id=index;
        setKey(curY);
    }
    Edge(Edge &edg, int index, double curY){
        this->a=edg.a;
        this->b=edg.b;
        this->id=index;
        setKey(curY);
    }
    Edge(double curY){
        this->key=curY;
    }
    void setKey(double curY){
        if(a.y == b.y)
        {
            this->key= (a.x<b.x)?a.x:b.x;
        }
        else
        {
            this->key= (curY-a.y)*(b.x - a.x)/(b.y - a.y)+ a.x;
        }
    }

    bool operator<(const Edge &edge)const{
        return (this->key<edge.key);
    }
};

vector<Point> V;
vector<Edge> E;
vector<Edge> D;
vector<int> LRS; // 0=left; 1=right;

void input(){
    FILE *file;
    int totalV;

    file=fopen("E:\\Learn Sth\\L-4,T-2\\Computational Geometry\\MPGL\\input3.txt","r");//for Desktop
    fscanf(file,"%d",&totalV);
    double xx,yy;
    int topP=0, botP=0;
    for(int i=0; i<totalV; i++)
    {
        fscanf(file,"%lf%lf",&xx,&yy);
        V.push_back(Point(xx,yy,i));
        printf("%lf %lf\n", xx, yy);
        LRS.push_back(0); // 0=left;
        if(V[i]>V[topP]){
            topP=i;
        }
        if(V[i]<V[botP]){
            botP=i;
        }
    }
    int i=topP;
    for(int i=topP; i!=botP; i++){
        LRS[i%totalV]=1; //1=rightside;
    }
    // Edges
    for(int i=0; i<totalV; i++)
    {
        E.push_back(Edge(V[i%totalV],V[(i+1)%totalV],i));
    }

    fclose(file);
}



bool isDInside(Point pl, Point i, Point ts){
    double val = ((pl.x-i.x)*(ts.y-i.y) - (ts.x-i.x)*(pl.y-i.y));
    if(val < 0) return false;
    else        return true;
}


bool comp(Point &p1, Point &p2)
{
    return ((p1.y==p2.y)?(p1.x<p2.x):(p1.y>p2.y));
}

void triangulateMP(){

    sort(V.begin(), V.end(), comp);
    stack<Point> stck;
    Point topStck,popLast;

    stk.push(V[0]);
    stk.push(V[1]);
    int total=V.size()-1;
    for (int i=2; i<total; i++)
    {
        //printf("\ncurrent point %d\n",sortedV[j].index);
        topStck = stck.top();
        if(LRS[V[j].id]!=LRS[topStck.id])
        {
            //printf("different chain %d,%d\n",sortedV[j].index,stackTop.index);
            while(!stck.empty())
            {
                topStck=stck.top();
                stck.pop();
                //printf("popped %d\n",stackTop.index);
                if(!stck.empty())
                {
                    D.push_back(Edge(V[i],topStck));
                    //printf("added diagonal %d to %d\n",sortedV[j].index,stackTop.index);
                }
            }
            stck.push(V[i-1]);
            //printf("pushing %d\n",sortedV[j-1]);
            stck.push(V[i]);
            //printf("pushing %d\n",sortedV[j]);
        }

        else
        {
            //printf("same chain %d,%d\n",sortedV[j].index,stackTop.index);
            //printf("popping top\n");
            popLast=stck.top();
            stck.pop();
            //printf("popped %d\n",lastPopped.index);
            if(!stck.empty())
            {
                topStck=stck.top();
            }

            while(!stck.empty() && isDInside(popLast, V[i], topStck))
            {
                stck.pop();
                //printf("popped %d\n",stackTop.index);
                D.push_back(Edge(V[i],topStck));
                //printf("adding diagonal %d to %d\n",sortedV[j].index,stackTop.index);
                popLast=topStck;
                if(!stck.empty())
                {
                    topStck=stck.top();
                }
            }

            stck.push(popLast);
            //printf("pushing %d\n",lastPopp.index);
            stck.push(V[i]);
            //printf("pushing %d\n",sortedV[j].index);
        }
    }
    if(!stck.empty())
    {
        stck.pop();
    }
    while(!stck.empty())
    {
        topStck=stck.top();
        stck.pop();
        if(!stck.empty())
        {
            D.push_back(Edge(V[V.size()-1],topStck));
        }
    }
}

void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

   // Draw a Red 1x1 Square centered at origin
   int i;

   glBegin(GL_LINE_LOOP);
   glColor3f(1.0f, 0.0f, 0.0f);
   for(i=0;i<V.size();i++)
			glVertex2f(V[i].x,V[i].y);
   glEnd();

   glColor3f(0, 1, 0);
   glBegin(GL_LINES);
   for(i=0;i<D.size();i++)
   {
       glVertex2f(D[i].a.x,D[i].a.y);
       glVertex2f(D[i].b.x,D[i].b.y);
   }
   glEnd();

   glColor3f(1, 1, 1);
	glBegin(GL_LINES);{
		//Y axis
		glVertex2f(0, -100);
		glVertex2f(0,  100);

		glColor3f(1, 1, 0);
		//X axis
		glVertex2f(-100, 0);
		glVertex2f( 100, 0);
	}glEnd();

   glFlush();  // Render now
}


int main(int argc, char **argv)
{

   input();
   triangulateMP();

   glutInit(&argc, argv);                 // Initialize GLUT
   glutCreateWindow("Monotone Partitioning"); // Create a window with the given title
   glutInitWindowSize(320, 320);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
   glLoadIdentity();                 // Reset Projection matrix
   gluOrtho2D(-50.0, 50.0, -50.0, 50.0); // Set clipping area's left, right, bottom, top
   //glutDisplayFunc(display); // Register display callback handler for window re-paint

   glutDisplayFunc(display); // Register display callback handler for window re-paint

   glutMainLoop();           // Enter the event-processing loop

	return 0;
}
