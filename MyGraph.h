/*
 * Graph.h
 *
 *  Created on: Mar 3, 2015
 *      Author: jessie
 */

#ifndef MYGRAPH_H_
#define MYGRAPH_H_

#include <iostream>
#include <queue>
#include "fplan.h"

using namespace std;
#define INFI 1000
#define MAX 1000
#define H 1
#define V 0

typedef struct MatrixGraph
{
	int numOfModules;
	int numOfEdges;
	int edge[MAX][MAX];
}matrixgraph;
/*
typedef struct Module
{
	char name;
	int width, height;
	int x,y,rx,ry;//X,Y means leftdown corner, X_,Y_ means rightup corner
	int degree;
}module;
*/

int getNum(char v);

void initGraph(matrixgraph *C, int m);

void initiateMod(module *m, char value,int w, int h, queue<module> *M);

void initiateMod(module *m, char value,int w, int h, vector<module> *M);

void addModule(matrixgraph *C);

void addModule(matrixgraph *C1, matrixgraph *C2, int type);

void setRelation(matrixgraph *C, int i, int j);

bool getRelation(matrixgraph *C, int i, int j);



//void moduleMerge(bst *Tv1, queue<module> * Gamma, graph *ch1, graph *ch2);

#endif /* MYGRAPH_H_ */
