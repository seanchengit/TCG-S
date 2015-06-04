---------------------------------------------------------

		      Florplan Merge
 based on RBT
---------------------------------------------------------

The code is used to show modules packing and merge two sets of packed modules, both based on RBT(Red-Black Tree).
------------------------------------------------------------------------
------------------------------------------------------------------------


Designed by
Sean Chen 

-------------------------------------------------------------------------
-------------------------------------------------------------------------

Instruction:
  main function lacated in file "main_area.cc", system will run differently due to different number of parameters.
  
 
makefile is already existed, after type "make", running program as:
"program parameter1 parameter2 ...", "program" is executable file which 
can be modified in "makefile".
 
(1). If only one parameter, the parameter would be seemed as modules filename, system will read information(module's height and weight) from the file, and then packing them through TCG-S method, finally  packing result would show in LEDA WINDOW, for example:
 "program file1"  
  

(2). If contains two parameters, two parameter would be seemed as modules filename, system will read information(module's height and weight) from each files, then packing them through TCG-S method and merge them horizontally, finally call show result in LEDA WINDOW, for example:
 "program file1 file2"
  
 

(3). If contains three parameters, the third parameter would seemed as merge type, for example: 
     
 "program file1 file2 0"
      
 "program file1 file2 1"
  
'0' means merge two sets horizontally, while '1' means vertically.
  
  

(4). if contains four parameters, the fourth parameter would seemed as displacement value, for example:
     
 "program file1 file2 1 1500,3500"
      
 "program file1 file2 0 2500,0"
  
wherein first line means two sets would merged vertically and first set moved 1500nm to the right, and second set moved 3500nm to the right.
  While second line means two sets merged horizontally, and first set moved up 2500nm, and second without displacement.

-------------------------------------------------------------------
-------------------------------------------------------------------
