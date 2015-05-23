#include <LEDA/graphics/window.h>
//#include <LEDA/string.h>
#include "fplan.h"
#include "stdio.h"
#include "show_graph.h"

void  show_graph(Clo_Red_Graph& fp, float width, 
                 float height, Modules_Info& modules_info)
{
     
     int i, j, n;
     n=modules_info.size();
     leda_window W;
     W.set_bg_color(13);
     W.display();
     int c1;
     float s=50.0;

    //if(width<height)
    //  s=height/W.xmax();
    //else
    //  s=width/W.xmax();

     do{
         int area;
         int wire;
         double time;
         double max_x=0.0;
         double max_y=0.0;


         W.set_line_width(2);  
         for(i=0; i<n; i++){
            float x1, y1, x2, y2;
            x1=modules_info[i].x;
            y1=modules_info[i].y;
            x2=modules_info[i].rx;
            y2=modules_info[i].ry;
//            cout<<"module "<<i+1<<endl;
//            cout<<x1<<", "<<y1<<", "<<x2<<", "<<y2<<endl;

            W.draw_filled_rectangle(x1/s, y1/s, x2/s, y2/s, 0); 
            W.draw_segment(x1/s, y1/s, x1/s, y2/s, 1);
            W.draw_segment(x1/s, y1/s, x2/s, y1/s, 1);
            W.draw_segment(x1/s, y2/s, x2/s, y2/s, 1);
            W.draw_segment(x2/s, y1/s, x2/s, y2/s, 1);
 
            if(max_x<double(x2/s))
              max_x=x2/s;
            if(max_y<double(y2/s))
              max_y=y2/s;
            
           
            char p1[10];
            
           
             
           //  W.text_box(x1/s, x2/s, y2/s, p1);
             p1[0]=p1[1]=p1[2]=p1[3]=p1[4]=' ';
             p1[5]=i+97;
            W.text_box(x1/s+(x2/s-x1/s)/2, x2/s, y2/s-(y2/s-y1/s)/2, p1);
            
         }//for
/*
  for(i=0; i < fp.network.size(); i++){
     Pin& p0=*fp.network[i][0];
     int x=p0.ax;
     int y=p0.ay;

     if(fp.network[i].size()==1)
        W.draw_segment(p0.ax, p0.ay, p0.ax, p0.ay);
     else{ 
        for(j=1; j< fp.network[i].size(); j++){
           Pin& p=*fp.network[i][j];
           W.draw_segment(x/s, y/s, p.ax/s, p.ay/s);    
           x=p.ax;
           y=p.ay;
        }
      }
  }
*/

         W.set_line_width(5);
         W.draw_segment(0.0, max_y, max_x, max_y);
         W.draw_segment(max_x, 0.0, max_x, max_y);
         W.draw_segment(0.0, 0.0, max_x, 0);
         W.draw_segment(0.0, 0.0, 0, max_y);

         int c1=W.read_mouse();
   
         if(c1==MOUSE_BUTTON(1))
           s=s-10;
         else if(c1==MOUSE_BUTTON(3))
           s=s+10;
    
        W.clear();
     }while(!W.ctrl_key_down());
//      }while(!MOUSE_BUTTON(1));

     //int c1;
     //c1=getchar();
}
