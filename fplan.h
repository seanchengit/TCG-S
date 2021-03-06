//---------------------------------------------------------------------------
#ifndef fplanH
#define fplanH
//---------------------------------------------------------------------------
#include <LEDA/graph/graph_iterator.h>
#include <LEDA/core/list.h>
#include <LEDA/graphics/window.h>
#include <LEDA/core/array.h>
#include <vector>
#include <math.h>
#include <time.h>
//#include "fplan.h"
#include "stdio.h"
//#include "show_graph.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;

struct Pin{
  int mod;
  int net;
  int x,y;    // relative position
  int ax,ay;  // absolute position
  Pin(int x_=-1,int y_=-1){ x=x_,y=y_; }
};
typedef vector<Pin> Pins;
typedef Pin* Pin_p;
typedef vector<Pin_p> Net;
typedef vector<Net > Nets;


enum Module_Type { MT_Hard, MT_Reclinear, MT_Soft, MT_Buffer };


struct Module{
  int id;
  char name[20];
  int width,height;
  int x,y; //the location of bottom-left corner 
  int area;
  Pins pins;
  Module_Type type;
};
typedef vector<Module> Modules;

typedef struct Module_Info{
  unsigned char name;
  int x,y;
  int rx,ry;
  int width, height;
  //'0' denote 0 degree, '1' denote 90 degree
  //'2' denote 180 degree, '3' denote 270 degree 
  int degree;   
}module;

typedef vector<Module_Info> Modules_Info;

class FloorPlan{
  public:
    FloorPlan() {};
    ~FloorPlan() {};
    
    void read(char*);
    void show_modules();
    void show_modules_pins();
    virtual void packing() {}

    // information
    //virtual float wire_c2c();
    int size() { return modules_N; }
    int getTotalArea() { return TotalArea; }
    int getArea() { return Area; }
    int getWidth() { return Width; }
    int getHeight() { return Height; }
    int getDeadSpace() { return DeadSpace; }
    int getWireLength() { return Wire; }
    
    Modules modules;
    Module  root_module;
    Modules_Info modules_info;    
    Nets network;

    /*
    added by XUAN CHEN 26.3.2015
    */

    void setWidth(int x){Width = x;}
    void setHeight(int x){Height = x;}
    void setArea(){Area = Width*Height;}
  protected:
    map<string,int> net_table;

    int Area, Width, Height, Wire;
    float Cost, Ratio; 
    int TotalArea;
    int DeadSpace;
    int modules_N;    
    
  private:
    void read_dimension(Module&);
    void read_IO_list(Module&,bool parent);
    void read_network();
    void create_network();   
};


//void error(char *msg,char *msg2="");
float rand_01();      
//---------------------------------------------------------------------------
#endif
