#Project: Clo_Red_Graph_Based Floorplanning
#Advisor: Yao-Wen Chang  <ywchang@cis.nctu.edu.tw>
#Author:  Jai_Ming Lin	 <gis87808@cis.nctu.edu.tw>
#Date:    7/20/2000 

.SUFFIXES: .cc 
SHELL=/bin/sh
LEDAROOT=/home/jessie/CMC/LEDA-6.4-fedora-core-8-64-g++-4.1.2-mt
TIMESTAMP=\"`date +%m/%d/%y_%H:%M:%S`\" 
STLLEDA=-DLEDA_PREFIX
FASTLEDA=-DLEDA_STL_ITERATORS
CXX=g++ 
#CXXFLAGS= -c -g   -O0 $(STLLEDA) $(FASTLEDA) -DTIMESTAMP=$(TIMESTAMP) -I$(LEDAROOT)/incl 
CXXFLAGS= -DLEDA_MULTI_THREAD -pthread -Wall --warn-error -Wno-error -c -g -O0 -fno-strict-aliasing  -DTIMESTAMP=$(TIMESTAMP) -I$(LEDAROOT)/incl -L
LDFLAGS= 
###########################################################################
LIBS = -lleda -lX11 -lm -L$(LEDAROOT)
OBJS = main_area.o hv_matrix.o fplan.o clo_red.o RBT.o MyGraph.o save_rect_line.o show_graph.o
#show_graph.o
SRCS = ${OBJS:%.o=%.cc}
EXE  = area

$(EXE): $(OBJS)
	  $(CXX) $(LDFLAGS) -o  $(EXE) $(OBJS) $(LIBS)  

$(OBJS): hv_matrix.h fplan.h clo_red.h save_rect_line.h show_graph.h MyGraph.h
#show_graph.h
clean: 
	rm -f $(OBJS); rm $(EXE); 

compact : $(EXE)
	strip $?
