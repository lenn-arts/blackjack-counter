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

module memory#(parameter
     N_BYTES = 16,            // Number of counts to store in RAM
     N_ADDR_BITS = 4) 
	(input logic		clk,    // Clock
    input logic 		write_enable,     
    input logic [N_ADDR_BITS-1:0] 	addr,  
    input logic [7:0]	data_in,   
    output logic [7:0] 	data_out);

	logic [7:0] mem[N_BYTES-1:0];

	always_ff @(posedge clk) begin
		if (write_enable) mem[addr] <= data_in;
		data_out <= mem[addr]; // read or replaced (old) data
	end
endmodule


// TODO: idea how to use ram in layers:
// A) have as additional output pins of the layers the input pins to the ream
// and as additional input pins of the layers the output of the ram
// then have top moudle (cnn_mem) link them accordingly
// OR B) instantiate ram modules in layer modules and somehow deal with the input/output mem 

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
	localparam ADDR_SIZE = 19;

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

	logic start_l1 = 0;
	logic start_l2 = 0;
	logic start_l3 = 0;
	logic start_l4 = 0;

	logic l1_done = 0;
	logic l2_done = 0;
	logic l3_done = 0;
	logic l4_done = 0;	

	logic [ADDR_SIZE-1:0] input_addr = -1;
	logic [ADDR_SIZE-1:0] l1_addr = -1;
	logic [ADDR_SIZE-1:0] l2_addr = -1;
	logic [ADDR_SIZE-1:0] l3_addr = -1;
	logic [ADDR_SIZE-1:0] l4_addr = -1;
	logic [ADDR_SIZE-1:0] output_addr = -1;

	logic [ADDR_SIZE-1:0] input_limit = 0; // where actual content in ram ends
	logic [ADDR_SIZE-1:0] output_limit = 0;

	logic [7:0] input_mem_in;
	logic [7:0] input_mem_out;
	logic input_mem_we;
	memory #(input_size, ADDR_SIZE) input_mem(.clk(clk), .write_enable(input_mem_we), .address(input_addr), .data_in(input_mem_in),.data_out(input_mem_out));
	
	logic [7:0] l1_mem_in;
	logic [7:0] l1_mem_out;
	logic l1_mem_we;
	memory #(l1_size, ADDR_SIZE) l1_mem(.clk(clk), .write_enable(l1_mem_we), .address(l1_addr), .data_in(l1_mem_in),.data_out(l1_mem_out));

	logic [7:0] l2_mem_in;
	logic [7:0] l2_mem_out;
	logic l2_mem_we;
	memory #(l2_size, ADDR_SIZE) l2_mem(.clk(clk), .write_enable(l2_mem_we), .address(l2_addr), .data_in(l2_mem_in),.data_out(l2_mem_out));

	logic [7:0] l3_mem_in;
	logic [7:0] l3_mem_out;
	logic l3_mem_we;
	memory #(l3_size, ADDR_SIZE) l3_mem(.clk(clk), .write_enable(l3_mem_we), .address(l3_addr), .data_in(l3_mem_in),.data_out(l3_mem_out));

	logic [7:0] l4_mem_in;
	logic [7:0] l4_mem_out;
	logic l4_mem_we;
	memory #(l4_size, ADDR_SIZE) l4_mem(.clk(clk), .write_enable(l4_mem_we), .address(l4_addr), .data_in(l4_mem_in),.data_out(l4_mem_out));

	/*reg [7:0] input_dat[100*100-1:0]; // input of every layer, max size: input image
	reg [7:0] output_dat[32*32*16-1:0]; // output of every layer, max size: layer 2 output
	reg [7:0] wlayer1_conv[5*5*1*16-1:0]; // weights per layer
	reg [7:0] wlayer2_conv[5*5*16*32-1:0];
	reg [7:0] wlayer3_fc[1152*200-1:0];
	reg [7:0] wlayer4_fc[200*53-1:0];*/
	
	//reg [7:0] ram[RAM_SIZE-1:0]; // RAM: n slots each with 8 bit

	reg [ADDR_SIZE:0] start_adresses[NUM_LAYERS+1:0]; // for all layers + input + output
	reg [ADDR_SIZE:0] end_adresses[NUM_LAYERS+1:0]; // for all layers + input + output
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
			case(address)
				4'd0: begin
					//input_dat[input_addr] <= writedata;
					if (input_loaded == 0) begin
						input_addr <= input_addr +1;
						input_mem_we <= 1;
						input_mem_in <= writedata;
						if (input_addr > input_size -1) begin
							input_loaded = 1;
							input_mem_we <= 0;
						end
					end
				end

				4'd1: begin
					//wlayer1_conv[l1_addr] <= writedata;
					if (l1_loaded == 0) begin
						l1_addr <= l1_addr +1;
						l1_mem_we <= 1;
						l1_mem_in <= writedata;
						if (l1_addr > l1_size -1) begin
							l1_loaded = 1;
							l1_mem_we <= 0;
						end
					end
				end

				4'd2: begin
					//wlayer2_conv[l2_addr] <= writedata;
					if (l2_loaded == 0) begin
						l2_addr <= l2_addr +1;
						l2_mem_we <= 1;
						l2_mem_in <= writedata;
						if (l2_addr > l2_size -1) begin
							l2_loaded = 1;
							l2_mem_we <= 0;
						end
					end
				end

				4'd3: begin
					//wlayer3_fc[l3_addr] <= writedata;
					if (l3_loaded == 0) begin
						l3_addr <= l3_addr +1;
						l3_mem_we <= 1;
						l3_mem_in <= writedata;
						if (l3_addr > l3_size -1) begin
							l3_loaded = 1;
							l3_mem_we <= 0;
						end
					end
				end   

				4'd4: begin
					//wlayer4_fc[l4_addr] <= writedata;
					if (l4_loaded == 0) begin
						l4_addr <= l4_addr +1;
						l4_mem_we <= 1;
						l4_mem_in <= writedata;
						if (l4_addr > l4_size -1) begin
							l4_loaded = 1;
							l4_mem_we <= 0;
						end
					end
				end

				default:;
			endcase
		
		end else if (&{input_loaded, l1_loaded, l2_loaded, l3_loaded, l4_loaded}) begin
			start_l1 <= 1;

		end else if (l1_done) begin
			start_l2 <= 1;
			start_l1 <= 0;

		end else if (l2_done) begin
			start_l3 <= 1;
			start_l2 <= 0;

		end else if (l3_done) begin
			start_l4 <= 1;
			start_l3 <= 0;

		end else if (l4_done) begin
			start_l4 <= 0; // TODO

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

