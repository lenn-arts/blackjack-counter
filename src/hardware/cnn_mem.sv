/*
 * custom peripheral
 */

module SimpleLayer(
		input logic clk,
		input logic start,
		input logic data_in,
		input logic weights,
		
		output logic data_out);

	assign data_out = data_in;
endmodule

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

	localparam input_size = 100*100; 
	localparam l1_size = 5*5*1*16;
	localparam l2_size = 5*5*16*32;
	localparam l3_size = 1152*200;
	localparam l4_size = 200*53;

	logic input_loaded = 0;
	logic l1_loaded = 0;
	logic l2_loaded = 0;
	logic l3_loaded = 0;
	logic l4_loaded = 0;

	logic l1_done = 0;
	logic l2_done = 0;
	logic l3_done = 0;
	logic l4_done = 0;	

	logic [15:0] input_addr = 0;
	logic [15:0] l1_addr = 0;
	logic [15:0] l2_addr = 0;
	logic [15:0] l3_addr = 0;
	logic [15:0] l4_addr = 0;
	logic [15:0] output_addr = 0;

	logic [15:0] input_limit = 0; // where actual content in ram ends
	logic [15:0] output_limit = 0;


	reg [7:0] input_dat[100*100-1:0]; // input of every layer, max size: input image
	reg [7:0] output_dat[32*32*16-1:0]; // output of every layer, max size: layer 2 output
	reg [7:0] wlayer1_conv[5*5*1*16-1:0]; // weights per layer
	reg [7:0] wlayer2_conv[5*5*16*32-1:0];
	reg [7:0] wlayer3_fc[1152*200-1:0];
	reg [7:0] wlayer4_fc[200*53-1:0];
	
	//reg [7:0] ram[RAM_SIZE-1:0]; // RAM: n slots each with 8 bit

	reg [15:0] start_adresses[NUM_LAYERS+1:0]; // for all layers + img
	reg [15:0] end_adresses[NUM_LAYERS+1:0]; // for all layers + img
	reg [NUM_LAYERS-1:0] present; // whether in memory

	// idea: for every layer have own RAM + input + output
	// use address to decide to which to write
	// use end_address to determine where content stops for over-size rams (such as output, which will get smaller every layer)

	always_ff @(posedge clk) begin
		// RESET
		if (reset) begin
			//for(int j = 0; j < RAM_SIZE; j = j+1)
			//	ram[j] = 8'd0;
			//for (int j = 0; j < NUM_LAYERS; j = j+1)
			//	present[j] = 1'b0;

		// INITIAL LOAD
		end else if (chipselect && write) begin
			//ram[address] <= writedata;
			case(address):
				4'd0: begin
					input_dat[input_addr] <= writedata;
					input_addr <= input_addr +1;
					if (input_addr > input_size -1) begin
						input_loaded = 1;
					end
				end

				4'd1: begin
					wlayer1_conv[l1_addr] <= writedata;
					l1_addr <= l1_addr +1;
					if (l1_addr > l1_size -1) begin
						l1_loaded = 1;
					end
				end

				4'd2: begin
					wlayer2_conv[l2_addr] <= writedata;
					l2_addr <= l2_addr +1;
					if (l2_addr > l2_size -1) begin
						l2_loaded = 1;
					end
				end

				4'd3: begin
					wlayer3_fc[l3_addr] <= writedata;
					l3_addr <= l3_addr +1;
					if (l3_addr > l3_size -1) begin
						l3_loaded = 1;
					end
				end   

				4'd4: begin
					wlayer4_fc[l4_addr] <= writedata;
					l4_addr <= l4_addr +1;
					if (l4_addr > l4_size -1) begin
						l4_loaded = 1;
					end
				end

				default:;
			endcase
		
		end else if (&{input_loaded, l1_loaded, l2_loaded, l3_loaded, l4_loaded}) begin
			start_l1 <= 1;

		end else if l1_done begin
			start_l2 <= 1;

		end else if l2_done begin
			start_l3 <= 1;

		end else if l3_done begin
			start_l4 <= 1;

		end else if l4_done begin
			start_l4 <= 1;

		// READING
		end else if (chipselect && read) begin
			val_out <= output_dat[address];
		end
	end


	/*SimpleLayer layer1_conv(
		.clk(clk),
		.start(start_l1),
		.data_in(input_dat),
		.weights(wlayer1_conv),
		.data_out(output)
	);

	SimpleLayer layer2_conv(
		.clk(clk),
		.start(start_l2),
		.data_in(input_dat),
		.weights(wlayer2_conv),
		.data_out(output)
	);

	SimpleLayer layer3_fc(
		.clk(clk),
		.start(start_l3),
		.data_in(input_dat),
		.weights(wlayer3_conv),
		.data_out(output)
	);

	SimpleLayer layer4_fc(
		.clk(clk),
		.start(start_l4),
		.data_in(input_dat),
		.weights(wlayer4_conv),
		.data_out(output)
	);*/

	//always_comb begin
	//	val_out = ram[bandwidth:0];
	//end
	       
endmodule

