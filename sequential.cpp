/******************************************************************* 
TDP3471 - Distributed and Parallel Computing
Farshid Tavakolizadeh
Email: email@farshid.ws
Project Page: https://farshid.ws/projects.php?id=118
Some copyrights goes to DuFei (http://goo.gl/zat8on)
Year: 2012
********************************************************************/

/// COMPILE: g++ con.cpp -o con -lgraph -lrt
/// RUN: ./con number_of_points

#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<vector>
#include <algorithm>
#include <time.h>
#include <unistd.h>
using namespace std;

int POINTS;

typedef struct
{
    float x;
    float y;
} POINT;
vector<POINT> p;
int n=0;

void Initial();
void GenPoints();
void DispPoint(int x0,int y0,int color);
void DispLine(int x0,int y0,int color);
void SortPoint();
int CreateConvexHull();
void DispPolygon(int num,int x0,int y0,int color);
void Close();
timespec diff(timespec start, timespec end);

main(int argc, char* argv[])
{
    int number;
    timespec time1, time2;
    POINTS = atoi(argv[1]);
    p.resize(POINTS);


    Initial();
    setcolor(15);
    line(0,13,640,13);

    GenPoints();
    DispPoint(0,0,15); // white

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    SortPoint();
    number=CreateConvexHull();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printf("Points: %d \t Boundary Points: %d \t ",POINTS,number);
    printf("Time: %ld s + %.9ld ns",diff(time1,time2).tv_sec,diff(time1,time2).tv_nsec);

    //sleep(3);
    DispPolygon(number,0,0,12); // light red

    Close();
}

void Initial()
{
    int driver=DETECT,mode=0;
    initgraph(&driver,&mode,(char*)"");
    setbkcolor(8); // white
}

void GenPoints()
{
    // the max value will be times 0.1
    const int maxX = 6400-100, maxY = 4800-300; // -300 to create space above grap
    vector<int> xx(maxX);
    for(int i=0; i<maxX; i++)
        xx[i] = i+50; // start from x=50

    vector<int> yy(maxY);
    for(int j=0; j<maxY; j++)
        yy[j] = j+50; // start from y=50

    vector<POINT> seeds(maxX*maxY);
    int s = 0;
    for(int i=0; i<maxX; i++)
    {
        for(int j=0; j<maxY; j++)
        {
            seeds[s].x = xx[i];
            seeds[s].y = yy[j];
            s++;
        }
    }

    random_shuffle ( seeds.begin(), seeds.end() );

    for(int i=0; i<POINTS; i++)
    {
        p[i].x = seeds[i].x;
        p[i].y = seeds[i].y;
    }
    n = POINTS-1;
}

void DispPoint(int x0,int y0,int color)
{
    int i;
    setcolor(color);
    for(i=0; i<n; i++)
        circle(0.1*p[i].x+x0,480-(0.1*p[i].y+y0),1);
}

void SortPoint()
{
    POINT *min,temp;
    int i,j;
    min=&p[0];
    for(i=1; i<n; i++)
        if(p[i].y<min->y)
            min=&p[i];
    temp=*min;
    *min=p[0];
    p[0]=temp;
    for(i=2; i<n; i++)
    {
        j=i-1;
        temp=p[i];
        while(j>0&&((temp.x-p[0].x)*(p[j].y-p[0].y)-(p[j].x-p[0].x)*(temp.y-p[0].y))>0)
        {
            p[j+1]=p[j];
            j--;
        }
        p[j+1]=temp;
    }
    p[n]=p[0];
}

void DispLine(int x0,int y0,int color)
{int i;
 setcolor(color);
 for(i=1;i<n;i++)
   {
    moveto(0.1*p[0].x+x0,480-0.1*p[0].y-y0);
    lineto(0.1*p[i].x+x0,480-0.1*p[i].y-y0);
   }
}

void DispPolygon(int num,int x0,int y0,int color)
{
 int i;
 setcolor(color);
 for(i=0;i<num;i++)
   {
    moveto(0.1*p[i].x+x0,480-0.1*p[i].y-y0);
    lineto(0.1*p[i+1].x+x0,480-0.1*p[i+1].y-y0);
   }
}

int CreateConvexHull()
{
    int j,Num,k,t;
    Num=n;
    for(k=3; k<=Num; k++)
    {
        j=2;
        for(j=2; j<=k-1; j++)
        {
            if(((p[k-j+1].x-p[k-j].x)*(p[k-j].y-p[0].y)-(p[k-j].x-p[0].x)*(p[k-j+1].y-p[k-j].y))*((p[k-j+1].x-p[k-j].x)*(p[k].y-p[k-j+1].y)-(p[k].x-p[k-j+1].x)*(p[k-j+1].y-p[k-j].y))>0)
            {
                for(t=k-1; t<Num; t++)
                    p[t]=p[t+1];
                Num--;
                k=k-1;
                j=j-1;
            }
        }
    }
    return Num;
}

void Close()
{
    //sleep(40);
    getche();
    closegraph();
}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
