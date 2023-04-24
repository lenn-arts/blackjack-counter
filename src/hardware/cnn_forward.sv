module Conv(
    input [3:0] input1,
    input [7:0] img_addr,
    input clk,
    output reg[10:0] output1
);

    wire img;
    wire output_conv_one;
    wire output_relu_one;
    wire output_flatten;
    wire output_fc1;
    wire output_classify;

    Imageloader il1(
        .clk(clk),
        .addr(img_addr),
        .output(img)
    );

    ConvolutionOne conv_one1(
        .clk(clk),
        .input(img),
        .output(output_conv_one)
    );

    Relu relu1(
        .clk(clk),
        .input(output_conv_one),
        .output(output_relu_one)
    );

    Flatten flatten1(
        .clk(clk),
        .input(output_relu_one),
        .output(output_flatten)
    );

    FullyConnected fc(
        .clk(clk),
        .input(output_flatten),
        .output(output_fc1)
    );

    Classify classify(
        .clk(clk),
        .input(output_fc1),
        .output(output_classify)
    );

    Storeresult store(
        .clk(clk),
        .input(output_classify)
    )

/* 
1. load image
per cnn layer 
    - load weights
    - loop
        - select window
        - apply convolution
    - output convolved image
*/

endmodule