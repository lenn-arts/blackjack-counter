/*
 * custom peripheral
 */

module cnn_mem(
		input logic        clk,
	    input logic 	   reset,
		input logic [7:0]  writedata, // must be multiple of 8
		input logic 	   write,
		input 		   		chipselect,
		input logic [18:0]  address,
		input logic  	   read,

		output logic [7:0] val_out); // must be multiple of 8!

	localparam bandwidth = 7; // for 8 bit
	localparam RAM_SIZE = 375000; // 300kbit
	localparam NUM_LAYERS = 5;

	reg [7:0] img[100*100-1:0];
	reg [7:0] ram[RAM_SIZE-1:0]; // RAM: n slots each with 8 bit
	//reg [8:0] ram_fc[RAM_SIZE-1:0];

	reg [18:0] start_adresses[NUM_LAYERS-1:0]; // for all layers + img
	reg [18:0] end_adresses[NUM_LAYERS-1:0]; // for all layers + img
	reg [NUM_LAYERS-1:0] present; // whether in memory

	// idea: for every layer have own RAM + input + output
	// use address to decide to which to write
	// use end_address to determine where content stops for over-size rams (such as output, which will get smaller every layer)

	always_ff @(posedge clk) begin
		if (reset) begin
			for(int j = 0; j < RAM_SIZE; j = j+1)
				ram[j] = 8'd0;
			for (int j = 0; j < NUM_LAYERS; j = j+1)
				present[j] = 1'b0;
		end else if (chipselect && write) begin
			ram[address] <= writedata;
		end else if (chipselect && read) begin
			val_out <= ram[address];
		end
	end

	//always_comb begin
	//	val_out = ram[bandwidth:0];
	//end
	       
endmodule

