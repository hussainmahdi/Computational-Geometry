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
vector<int> helper;
set<Edge> bst;
set<Edge>::iterator it;
pair<set<Edge>::iterator,bool> ret;
vector<set<Edge>::iterator> ePointer;

void input(){
    FILE *file;
    int totalV;
    double xx,yy;

    file=fopen("E:\\Learn Sth\\L-4,T-2\\Computational Geometry\\MPGL\\input.txt","r");//for Desktop
    fscanf(file,"%d",&totalV);
    for(int i=0; i<totalV; i++)
    {
        fscanf(file,"%lf%lf",&xx,&yy);
        V.push_back(Point(xx,yy,i));
        //printf("%lf %lf\n", xx, yy);
        helper.push_back(-1);
    }
    // Edges
    for(int i=0; i<totalV; i++)
    {
        E.push_back(Edge(V[i%totalV],V[(i+1)%totalV],i));
        ePointer.push_back(it);
    }

    fclose(file);
}

double cross(Point n1, Point curId, Point n2){
    //(n1.x-curId.x)  (n1.y-curId.y)
    //(n2.x-curId.x)  (n2.y-curId.y)
    return ((n1.x-curId.x)*(n2.y-curId.y) - (n2.x-curId.x)*(n1.y-curId.y));
}

int findVType(int curId){

    //n1 = neighbour 1
    //n2 = neighbour 2
    int n1,n2;
    if(curId>0) n1 = curId-1;
    else        n1 = V.size()-1;

    if(curId<(V.size()-1)) n2 = curId+1;
    else                   n2 = 0;

    int vType = REGULAR_V;
    if(V[curId]>V[n1] && V[curId]>V[n2])
    {
        if(cross(V[n1], V[curId], V[n2])<0)  vType=START_V;
        else                                 vType=SPLIT_V;
    }
    else if(V[curId]<V[n1] && V[curId]<V[n2])
    {
        if(cross(V[n1], V[curId], V[n2])<0)  vType=END_V;
        else                                 vType=MERGE_V;
    }
    return vType;
}

void handleV(int curId, int vType, double curY){
    //n1 = neighbour 1
    //n2 = neighbour 2
    int n1,n2;
    if(curId>0) n1 = curId-1;
    else        n2 = V.size()-1;
    if(curId<(V.size()-1)) n2 = curId+1;
    else                   n2 = 0;

    if(vType==REGULAR_V)
    {
        if(V[curId].y<V[n2].y)
        {
            if(findVType(helper[n2])== MERGE_V)
            {
                D.push_back(Edge(V[curId],V[helper[n2]]));
            }
            //delete
            bst.erase(ePointer[n2]);
            it=bst.insert(Edge(E[curId], curId, curY)).first;
            ePointer[curId]=it;
            helper[curId]=curId;
        }
        else
        {
            it = bst.lower_bound(Edge(V[curId].x));
            if(findVType(helper[(*it).id])== MERGE_V)
            {
                D.push_back(Edge(V[curId],V[helper[(*it).id]]));
            }
            helper[(*it).id]=curId;
        }
    }
    else if(vType==START_V)
    {
        printf("START_V\n");
        it=bst.insert(Edge(E[curId],curId,curY)).first;
        ePointer[curId]=it;
        helper[curId]=curId;
    }
    else if(vType==END_V)
    {
        printf("END_V\n");
        if(findVType(helper[n2])== MERGE_V)
        {
            D.push_back(Edge(V[curId],V[helper[n2]]));
        }
        bst.erase(ePointer[n2]);
    }
    else if(vType==SPLIT_V)
    {
        printf("SPLIT_V\n");

        Edge edg(V[curId].x);  // confusion
        it = bst.lower_bound(edg);
        D.push_back(Edge(V[curId],V[helper[(*it).id]]));
        helper[(*it).id]=curId;

        it=bst.insert(Edge(E[curId],curId,curY)).first;
        ePointer[curId]=it;

        helper[curId]=curId;

    }
    else if(vType==MERGE_V)
    {
        printf("MERGE_V\n");
        if(findVType(helper[n2])== MERGE_V)
        {
            D.push_back(Edge(V[curId],V[helper[n2]]));
        }
        //delete
        bst.erase(ePointer[n2]);

        it=bst.lower_bound(Edge(V[curId].x));
        if(findVType(helper[(*it).id]) == MERGE_V)
        {
            //printf("helper of MERGE_V found\n");
            D.push_back(Edge(V[curId],V[helper[(*it).id]]));
        }
        helper[(*it).id]=curId;
    }

}

bool comp(Point &p1, Point &p2)
{
    return ((p1.y==p2.y)?(p1.x<p2.x):(p1.y>p2.y));
}

void makeMonotone(){
    priority_queue<Point,vector<Point>,Point> pq;
    //priority_queue<Point> pq;
    printf("before pushing into pq");

    for(int i=0; i<V.size(); i++)
    printf("id=%d %lf %lf\n",V[i].id, V[i].x, V[i].y);

    for(int i=0; i<V.size(); i++)
            pq.push(V[i]);

//    printf("After pushing into pq");
//    while(!pq.empty()){
//        Point v=pq.top();
//        pq.pop();
//
//        printf("id=%d %lf %lf\n",v.id, v.x, v.y);
//    }

    while(!pq.empty()){
        Point v=pq.top();
        pq.pop();

        printf("curID = %d\n",v.id);

        int vType=findVType(v.id);
        if(vType==0) printf("id=%d, Start\n",v.id);
        else if(vType==1) printf("id=%d, End\n",v.id);
        else if(vType==2) printf("id=%d, Split\n",v.id);
        else if(vType==3) printf("id=%d, Merge\n",v.id);
        else if(vType==4) printf("id=%d, Regular\n",v.id);
        handleV(v.id, vType, v.y);
        printf("END of Vertex %d\n\n",v.id);
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
   makeMonotone();

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
