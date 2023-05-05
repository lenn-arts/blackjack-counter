/*
 * custom peripheral
 */

module cnn_mem(
		input logic        clk,
	    input logic 	   reset,
		input logic [7:0]  writedata, // must be multiple of 8
		input logic 	   write,
		input 		   		chipselect,
		input logic [1:0]  address,
		input logic  	   read,

		output logic [7:0] val_out); // must be multiple of 8!

	reg [7:0]	value;
	reg [16:0] params;
	reg [16:0] img;

	always_ff @(posedge clk)
		if (reset) begin
			value = 8'd0;
		end else if (chipselect && write)
			case (address)
				2'h0 : 
					value <= writedata;
					break;
				2'h1 : 
					params <= writedata;
					break;
				2'h2:
					img <= writedata;
					break;
			endcase

	always_comb begin
		val_out = value + 1;
	end
	       
endmodule

