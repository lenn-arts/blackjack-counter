/*
 * custom peripheral
 */

module cnn_mem(
		input logic        clk,
	    input logic 	   reset,
		input logic [15:0]  writedata, // must be multiple of 8
		input logic 	   write,
		input 		   		chipselect,
		input logic [15:0]  address,
		input logic  	   read,

		output logic [15:0] val_out); // must be multiple of 8!

	localparam bandwidth = 7; // for 8 bit
	localparam RAM_SIZE = 256

	reg [7:0]	value;
	reg [15:0] params;
	reg [15:0] img;
	reg [15:0] ram[RAM_SIZE-1:0]; // RAM: 256 slots each with 16 bit

	

	always_ff @(posedge clk) begin
		if (reset) begin
			for(j = 0; j < RAM_SIZE; j = j+1)
				ram[j] = 16'd0;
			//ram[255:0] = 256'd0;
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

