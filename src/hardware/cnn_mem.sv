/*
 * custom peripheral
 */

module cnn_mem(
		input logic        clk,
	    input logic 	   reset,
		input logic [7:0]  writedata, // must be multiple of 8
		input logic 	   write,
		input 		   		chipselect,
		input logic [15:0]  address,
		input logic  	   read,

		output logic [7:0] val_out); // must be multiple of 8!

	reg [7:0]	value;
	reg [15:0] params;
	reg [15:0] img;
	reg [255:0] ram;

	localparam bandwidth = 7; // for 8 bit

	always_ff @(posedge clk) begin
		if (reset) begin
			ram[255:0] = 256'd0;
		end else if (chipselect && write) begin
			//case (address)
			//	2'h0 : 
			//		value <= writedata;
			//		break;
			//	2'h1 : 
			//		params <= writedata;
			//		break;
			//	2'h2:
			//		img <= writedata;
			//		break;
			//ram[address+bandwidth:address] <= writedata;
			ram[address] <= writedata;
			//endcase
		end else if (chipselect && read) begin
			val_out <= ram[address];
		end
	end

	//always_comb begin
	//	val_out = ram[bandwidth:0];
	//end
	       
endmodule

