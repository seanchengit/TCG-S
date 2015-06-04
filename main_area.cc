//---------------------------------------------------------------------------
#include <LEDA/graph/graph_iterator.h>
#include <LEDA/core/list.h>
#include <LEDA/graphics/window.h>
#include <LEDA/core/array.h>
#include <vector>
#include <math.h>
#include <time.h>
#include "fplan.h"
#include "stdio.h"
#include "show_graph.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include "clo_red.h"
#include "save_rect_line.h"
#include "RBT.cc"
#include "MyGraph.h"
/*
#include <iostream>             
#include <cstring>
#include <cmath>
#include <time.h>
//#ifdef BSD
#include <sys/time.h>
#include <sys/resource.h>
//#endif
//#include <LEDA/string.h>
#include <LEDA/core/string.h>
#include "fplan.h"
#include "clo_red.h"
#include "save_rect_line.h"
*/
//#include "show_graph.h"
//---------------------------------------------------------------------------

const float init_avg = 200;
int hill_climb_stage = 4;
float avg_ratio=200;
float Random_Floorplanning(Clo_Red_Graph &fp,int times);
//float lamda=1.3;
double init_time_s;
double seconds();

double mean(vector<double> &chain){
  double sum=0;
  for(int i=0; i < chain.size();i++)
     sum+= chain[i];
  return sum/chain.size();
}

double std_var(vector<double> &chain){
  double m = mean(chain);
  double sum=0;
  double var;
  int N= chain.size();

  for(int i=0; i < N;i++)
     sum += (chain[i]-m)*(chain[i]-m);

  var = sqrt(sum/(N-1));
//  printf("  m=%.4f ,v=%.4f\n",m,var);

  return var;
}

//int ite=0;

/* 
   Simulated Annealing B*Tree Floorplanning
   P: probability to accept in avg. delta cost (0~1)
   r: temperature decreasing rate (0~1)
   t: terminating temperature
    conv_rate: converge condition which is maximal fail rate (0~1)
   k: factor of the number of permutation in one temperature
*/

int max_rx(Clo_Red_Graph &fp)
{
    int i,temp=0;
    for(i=0;i<fp.modules_info.size();i++)
    {
        temp = temp < (fp.modules_info[i].rx)?fp.modules_info[i].rx:temp;
    }
    return temp;
}

int max_ry(Clo_Red_Graph &fp)
{
    int i,temp=0;
    for(i=0;i<fp.modules_info.size();i++)
    {
        temp = temp<(fp.modules_info[i].ry)?fp.modules_info[i].ry:temp;
    }
    return temp;
}

void RB_Floorplanning_merge(Clo_Red_Graph &fp1, Clo_Red_Graph &fp2,  bst &Tv1, matrixgraph *Ch1, matrixgraph *Ch2, int type, float P, float r, float Term_T, float conv_rate, int k)
{
    int i,len1, len2;
    setRelation(Ch1, 0, 5); setRelation(Ch1, 1, 6);
    setRelation(Ch1, 1, 3); setRelation(Ch1, 1, 4);
    setRelation(Ch1, 1, 5); setRelation(Ch1, 2, 3);
    setRelation(Ch1, 2, 4); setRelation(Ch1, 2, 6);
    setRelation(Ch1, 2, 5); setRelation(Ch1, 3, 5);
    setRelation(Ch1, 4, 5);
    Tv1.insertHead('s', 0, 0, Ch1);
    i = 0;
    while(i < fp1.modules_info.size())
    {
        Tv1.insert_(&(fp1.modules_info[i]), Ch1);
        i++;
    }
/*
    fp1.setWidth(max_rx(fp1));
    fp1.setHeight(max_ry(fp1));
    fp1.setArea();
*/
   
    setRelation(Ch2, 0, 5); setRelation(Ch2, 1, 6);
    setRelation(Ch2, 1, 3); setRelation(Ch2, 1, 4);
    setRelation(Ch2, 1, 5); setRelation(Ch2, 2, 3);
    setRelation(Ch2, 2, 4); setRelation(Ch2, 2, 6);
    setRelation(Ch2, 2, 5); setRelation(Ch2, 3, 5);
    setRelation(Ch2, 4, 5);
 
    addModule(Ch1, Ch2, type);
    i = 0;
    len1 = fp1.modules_info.size();
    len2 = fp2.modules_info.size();
    while(i < len2)
    {
        fp2.modules_info[i].name+=len1;
        fp1.modules_info.push_back(fp2.modules_info[i]);
        Tv1.insert_(&(fp1.modules_info.back()), Ch1);
        i++;
    }
    
    fp1.setWidth(max_rx(fp1));
    fp1.setHeight(max_ry(fp1));
    fp1.setArea();

}

void RB_Floorplanning(Clo_Red_Graph &fp, bst &Ts, bst &Tv, matrixgraph *Ch,  float P, float r, float Term_T, float conv_rate, int k)
{
    int i=0;
    setRelation(Ch, 0, 5); setRelation(Ch, 1, 6);
    setRelation(Ch, 1, 3); setRelation(Ch, 1, 4);
    setRelation(Ch, 1, 5); setRelation(Ch, 2, 3);
    setRelation(Ch, 2, 4); setRelation(Ch, 2, 6);
    setRelation(Ch, 2, 5); setRelation(Ch, 3, 5);
    setRelation(Ch, 4, 5);
    Ts.insertHead('s', 0, 0, Ch);
   // Tv.insertHead('t', 0, 0, Ch);
    while(i<fp.modules_info.size())
    {
        Tv.insert_(&(fp.modules_info[i]), Ch);
        Ts.insert_(&(fp.modules_info[i]), Ch);
       // Tv.insert_(&(fp.modules_info[i]), Ch);
        i++;
    }
    fp.setWidth(max_rx(fp));
    fp.setHeight(max_ry(fp));
    fp.setArea();
}

void SA_Floorplanning(Clo_Red_Graph &fp,float P,float r,float Term_T,
	   	      float conv_rate,int k)
{
  Solution Best,E;
  int MT,uphill,reject;
  float pre_cost,cost,d_cost,reject_rate;
  int N = k * fp.size();
  float T, Actual_T=1;
  float avg=init_avg;
  float max_reject_rate = 1;
  ofstream fs("/tmp/debug");
  double rf= Random_Floorplanning(fp, k);
  double estimate_avg =  rf / avg_ratio;
  //cout << "Random Average Delta Cost = " << rf << endl;
  //cout << "Estimate Average Delta Cost = " << estimate_avg << endl;

  if(hill_climb_stage==0)
    avg = estimate_avg;

  T = avg / log(P);  
  
  // get inital solution
  fp.get_solution(Best);
  pre_cost = Best.Cost;
  E = Best;

  fs.precision(4);
  
  int good_num=0,bad_num=0;
  float total_cost=0;
  int count=0;

  do{
      count++;
      MT=uphill=reject=0;
      total_cost = 0;

      fs   << "Iteration " << count << ", T=" << Actual_T << endl;
      cout << "Iteration " << count << ", T=" << Actual_T << endl;

     vector<double> chain;
     for(; uphill < N && MT < 2*N; MT++){

         if(Actual_T==1)
            fp.perturbate1();
         else if(Actual_T > 0.8)
            fp.perturbate2();
         else
            fp.perturbate3();

         //fp.Test();
         fp.contour_packing();
         //fp.compute_wire_length();
         //fp.compute_cost(alpha, beta, Best);
         cost = fp.get_cost();
         d_cost = cost - pre_cost;
         float p = exp(d_cost/T);

         chain.push_back(cost);
 
         if(d_cost <=0 || rand_01() < p ){
           pre_cost = cost;
           total_cost += d_cost;

           if(d_cost != 0)
       	     fs << "    total= " << total_cost << endl;

           if(d_cost > 0){ 
             uphill++;
             fs << " bad= " << d_cost << ": \t\t" << p <<  endl;
              bad_num++;
           }

 	   if(d_cost < 0){
  	     fs << " good= " << d_cost << endl;
             good_num++;
           }
           fp.get_solution(E);

           // keep best solution
           if(cost < Best.Cost){
             fp.get_solution(Best);
             cout << "   ==>  Cost= " <<  Best.Cost << endl;
             assert(Best.Cost >= fp.getTotalArea());
           }
         }
         else{
           reject++;
           fp.recover(E);
         }
     }//for_MT
     T = r*T;
     //cout<<"decreasing T="<<T<<endl;
     //double sv = std_var(chain);
     //float r_t = exp(lamda*T/sv);
     //T = r_t*T;

     // After Hill-Climbing, start to use normal SA
     if(count == hill_climb_stage){
       T = estimate_avg/log(P);
       Actual_T = exp(estimate_avg/T);
     }
     if(count > hill_climb_stage){
       Actual_T = exp(estimate_avg/T);
       max_reject_rate = conv_rate;
     }

     reject_rate = float(reject)/MT;

     fs    << " reject=" << reject_rate << endl;
  }while(reject_rate < max_reject_rate && Actual_T > Term_T);

  if(reject_rate >= conv_rate)
    cout << "Convergent!\n";

  if(Actual_T < Term_T)
    cout << "Cooling Enough!\n";

  fp.recover(Best);
}

float Random_Floorplanning(Clo_Red_Graph &fp,int times){
  Solution Best;  
  fp.contour_packing();
  Best.Area=fp.getArea();
  //show_graph(fp.getWidth(), fp.getHeight(), fp.modules_info); 
  fp.get_solution(Best);
  double total_cost=0, pre_cost, cost;
  int t = 0;
  int N = times;
  pre_cost = (double)fp.get_cost();

  for(int i=0; i < N; i++){
    fp.perturbate1();
    //fp.Test();
    fp.contour_packing();

    cost = fp.get_cost();
    if(cost-pre_cost > 0){
      total_cost+= (cost-pre_cost);
      t++;
      pre_cost = cost;
    }
   
    if(cost <= Best.Cost){
      fp.get_solution(Best);
    }
  }
  fp.recover(Best);
  return (total_cost/t);
}

double seconds()
{
#ifdef BSD       // BSD system instead of SYSV
   rusage time;
   getrusage(RUSAGE_SELF,&time);
   return (double)(1.0*time.ru_utime.tv_sec+0.000001*time.ru_utime.tv_usec);
#else
   return double(clock())/CLOCKS_PER_SEC;
#endif
}


int main(int argc,char *argv[])
{
  // cout<<"chen"<<endl;  
   cout << "Floorplanning Version:" << TIMESTAMP << endl;
   int mergetype = 0;
   int times=10;
   char filename[80];
   char filename1[80];
   float init_temp=0.9,term_temp=0.3,temp_scale=0.9,cvg_r=0.95;
   int target_cost=-1;
   if(argc<=1){
     printf("Usage: Area_CGS <filename> [times=%d] [hill_climb_stage=%d]\n",
           times, hill_climb_stage);
     printf("             [avg_ratio=%d]\n",avg_ratio);
     printf("             [inital-temp=%.2f] [terminal-temp=%.2f]\n",
	   init_temp, term_temp);
     printf("             [temp-scale=%.2f]  [convergence-ratio=%.2f\n",
	   temp_scale, cvg_r);
     return 0;
   }else{
     int argi=1;
     if(argi < argc) strcpy(filename,argv[argi++]);
     if(argi < argc ) strcpy(filename1,argv[argi++]);
     if(argi < argc ) mergetype = atoi(argv[argi++]);
    /*
     if(argi < argc) times=atoi(argv[argi++]);
     if(argi < argc) hill_climb_stage=atoi(argv[argi++]);
     if(argi < argc) avg_ratio=atof(argv[argi++]);
     if(argi < argc) target_cost=atoi(argv[argi++]); 

     if(argi < argc) init_temp=atof(argv[argi++]);
     if(argi < argc) term_temp=atof(argv[argi++]);
     if(argi < argc) temp_scale=atof(argv[argi++]);
    
     if(argi < argc) cvg_r=atof(argv[argi++]);
  */
   }
   float area, wire;
   init_time_s=seconds();
   double time_s;

   Clo_Red_Graph fp;
   fp.read(filename);
   //time_t t;
   //srand((unsigned) time(&t)); 
   //fp.show_modules(); 
   
  // fp.init_sqr();
   //fp.Test(); 
   fp.show_modules();
  // SA_Floorplanning(fp,init_temp,temp_scale, term_temp,cvg_r,times);
    
   
   matrixgraph Ch1;
   initGraph(&Ch1, fp.modules_info.size());
   bst Ts, Tv;

   if(argc > 2)
   {
       Clo_Red_Graph fp2;
       fp2.read(filename1);
    //   fp2.init_sqr();
       fp2.show_modules();
       matrixgraph Ch2;
       initGraph(&Ch2, fp2.modules_info.size());
      // bst Tv2;
       RB_Floorplanning_merge(fp,fp2,Tv,&Ch1,&Ch2, mergetype, init_temp, temp_scale, term_temp, cvg_r, times);
   }
   else{
   RB_Floorplanning(fp, Ts, Tv, &Ch1, init_temp, temp_scale, term_temp, cvg_r, times);   
   }
   area=fp.getArea();
  // cout<<"Best Area="<< area<<endl;
   cout<<"totall Area =" <<area<<endl;  
  // Clo_Red_Graph& myfp = fp;
   Tv.inorder();
   show_graph(fp, fp.getWidth(), fp.getHeight(), fp.modules_info);
   time_s = seconds()-init_time_s;
   strcat(filename,"_area");
   strcpy(filename1, filename);
   if(area<=(float)target_cost){
      save_rect_line(filename1, fp, time_s);
   }
   
//   { //log performance and quality
     FILE *fs= fopen(strcat(filename,".res"),"a+");
     //cpu_time area times avg_ratio hill_climb_stage init_temp temp_scale
      fprintf(fs,"%.1f %.1f %.1f %d %d %d %.2f %.2f\n",time_s, area, wire, times, 
      hill_climb_stage, int(avg_ratio), init_temp, temp_scale);
     fclose(fs);
//   }
   
   return 1;
}
