/*
 * Graph.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jessie
 */

#include <iostream>
#include <queue>
#include "MyGraph.h"
using namespace std;


/*typedef struct Graph
{
	int numOfModules;
	//int numOfEdges;
	int edge[MAX][MAX];
}graph;*/

int getNum(char v)
{
		return v - 97;
}



void initGraph(matrixgraph *C, int m)
{
	//C->numOfEdges = 0;
	C->numOfModules = m;
    int i,j;
	for(i = 0;i < m;i++)
		for(j=0;j<m;j++)
		{
			if(i>=j)
				C->edge[i][j] = 0;
			else
				C->edge[i][j] = 0;
		}
}

void initiateMod(char value,int w, int h, vector<module> *M)
{
    module m;
	m.name = value;
	m.width = w;
	m.height = h;
	m.x = 0;
	m.y = 0;
	m.degree = 0;
	M->push_back(m);
}

void initiateMod(module *m, char value,int w, int h, queue<module> * M)
{
	m->name = value;
	m->width = w;
	m->height = h;
	m->x = 0;
	m->y = 0;
	m->degree = 0;
	M->push(*m);
}

void addModule(matrixgraph *C)//no relation
{
	C->numOfModules++;
	int m = C->numOfModules - 1;
	for(int i=0;i<=m;i++)
	{
		if(i==m)
			C->edge[m][i] = 0;
		else
			C->edge[i][m] = 0;
	}
}

void addModule(matrixgraph *C1, matrixgraph *C2, int type)//set relation
{
	int i,j,m1,m2;
	m1 = C1->numOfModules;
	m2 = C2->numOfModules;
    C1->numOfModules = m1 + m2;
	for(i=0;i<m1;i++)
	{
		for(j=0;j<m2;j++)
		{
			C1->edge[i][m1+j]=type?0:1;//configure relation based on type,0 is left-right, 1is bottom-top
			C1->edge[m1+j][i]=0;
		}
	}
	for(i=0;i<m2-1;i++)
	{
		for(j=i+1;j<m2;j++)
		{
            
            C1->edge[i+m1][j+m1] = C2->edge[i][j];
		}y
	}
}

/*void moduleMerge(bst *Tv, queue<module> * Gamma, graph *ch1, graph *ch2)
{
	addModule(ch1, ch2);
	Tv->insertHead('t', 0, 0, ch1);//starting construct t tree.
	while(!Gamma->empty())
	{
		Tv->insert_(&(Gamma->front()), ch1);
		Gamma->pop();
	}
}*/



/*
void getCoordinate(module m, int x, int y)
{
	m.X = x;
	m.Y = y;
	m.X_ = x + m.width;
	m.Y_ = y + m.heights;
}
*/


void setRelation(matrixgraph *C, int i, int j)
{
	C->edge[i][j] = 1;
	//C->numOfEdges++;
}

bool getRelation(matrixgraph *C, int i, int j)
{
    i = i<0?(abs(i)-97-97):i;
    j = j<0?(abs(j)-97-97):j;
	return C->edge[i][j];
}



