/*
 * custom peripheral
 */

module img_reader(
		input logic        clk,
	    input logic 	   reset,
		input logic [31:0]  writedata, // must be multiple of 8
		input logic 	   write,
		input 		   		chipselect,
		input logic [7:0] 	address,
		input logic  	   read,

		input logic [7:0] VGA_R,
		input logic [7:0] VGA_G,
		input logic [7:0] VGA_B,
		input logic HSYNC,
		input logic VSYNC,

		output logic [31:0] readdata,
		output logic get_img); // must be multiple of 8!

	assign readdata = get_img ? {VGA_R, VGA_G, VGA_B, 8'd0} : counter_to_zeros;

	logic [31:0] counter_to_zeros = 0;
	logic [31:0] counter = 0;

	always_ff @(posedge clk) begin
		// RESET
		if (reset) begin
			get_img <= 0;
		end else if (chipselect && read) begin
			case(address)
				8'b0: begin
					get_img <= 1;
					counter_to_zeros <= 0;
					counter <= counter + 1;
					if (!HSYNC && !VSYNC) begin
						counter_to_zeros <= counter;
					end
					break;
				end
				8'b1: begin
					get_img <= 0;
					counter <= 0;
					counter_to_zeros <= 0;
					break;
				end
			endcase
		end else begin 
			get_img <= 0;
			counter <= 0;
			counter_to_zeros <= 0;
		end
	end

	       
endmodule

