.PHONY: default program clean hex7seg lint

SVFILES = SCCB.sv

# Path to USB Blaster II firmware file in Quartus, only for openFPGALoader

BLASTER-6810 = /opt/intelFPGA_lite/21.1/quartus/linux64/blaster_6810.hex

default :
	@echo "No target given. Try:"
	@echo ""
	@echo "make hex7seg"
	@echo "make collatz.vcd"
	@echo "make range.vcd"
	@echo "make lint"
	@echo "make output_files/lab1.sof"
	@echo "make program"

# Program the FPGA through the USB Blaster connection
# It is the second thing on the JTAG chain (the HPS block is first), hence @2

program : output_files/lab1.sof
	quartus_pgm -c 1 -m jtag -o "p;output_files/lab1.sof@2"


# Compile the project in Quartus

output_files/lab1.sof : lab1.qpf $(SVFILES)
	quartus_sh --flow compile lab1

# Create the Quartus project files from a Tcl script

lab1.qpf lab1.qsf lab1.sdc : de1-soc-project.tcl
	quartus_sh -t de1-soc-project.tcl



# Program the FPGA using openFPGALoader, an alternative to the
# Quartus programmer

program-open : output_files/lab1.svf
	openFPGALoader -b de1Soc \
	  --probe-firmware $(BLASTER-6810) \
	  output_files/lab1.svf

# Convert the .sof to to an .svf file for openFPGALoader

output_files/lab1.svf : output_files/lab1.sof
	quartus_cpf -c \
		--operation p --voltage 3.3 --freq 24.0MHz \
		output_files/lab1.sof output_files/lab1.svf



# Quickly check the System Verilog files for errors

lint :
	for file in $(SVFILES); do \
	verilator --lint-only -Wall $$file; done



# Run Verilator simulation

SCCB.vcd : obj_dir/VSCCB
	obj_dir/VSCCB


# Create Verilator simulations


obj_dir/VSCCB : SCCB.sv SCCB.cpp
	verilator -trace -Wall -cc SCCB.sv -exe SCCB.cpp \
		-top-module SCCB
	cd obj_dir && make -j -f VSCCB.mk


# Create a .tar.gz file of the contents

TARFILES = Makefile $(SVFILES) \
	SCCB.cpp SCCB.gtkw \
	de1-soc-project.tcl

lab1.tar.gz : $(TARFILES)
	cd .. && tar zchf lab1/lab1.tar.gz $(TARFILES:%=lab1/%)


clean :
	rm -rf obj_dir db incremental_db output_files \
	lab1.qpf lab1.qsf lab1.sdc lab1.qws c5_pin_model_dump.txt
