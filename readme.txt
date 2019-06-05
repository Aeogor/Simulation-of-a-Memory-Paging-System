Name  : Srinivas C Lingutla
UIN   : 655115444
NetID : slingu2

------------------------------ Homework 3 -----------------------------------

 === BUILD ====
 make vmpager
 or
 g++ -o vmpager vmpager.cpp

------------------------------

=== RUN ===
./vmpager filename #MemoryAccess FRAMETABLESIZE

** make run , runs the following command for the .nz file **

./vmpager Chevalier_473.nz 0 256

Leaving #MemoryAccess empty or 0 will run the simulation for the entire file
Leaving FRAMETABLESIZE empty will default the frame table to a size of 256

------------------------------

=== CLEAN ===
make clean
- this will remove the executable

------------------------------

The program currently outputs my name, project, and my netid
along with Filename, the arguments used and the Hits and misses for both the
FIFO and infinite simulation.

-------------------------------
FILES INCLUDED


├── Chevalier_473.nz
├── Chevalier_473.raw
├── Esher.raw
├── GrandeJatte.raw
├── HW3-361-f17.pdf
├── JohnBell.raw
├── LUG_Newbies.raw
├── VRUPL_Logo.raw
├── analysis.xlsx
├── calibration.raw
├── diver.raw
├── diver_resize.raw
├── light_drops.raw
├── makefile
├── readme.txt
├── slingu2_REPORT.pdf
├── vmpager.cpp
