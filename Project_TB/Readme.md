To run Test bench run the following:

make SCCB.vcd
gtkwave --save=SCCB.gtkw SCCB.vcd

NOT COMPLETE but here are what the files do:
SCCB.vs: Code to be tested
SCCB.cpp: The actual test bench
SCCB.gtkw: reference for gtkwave

