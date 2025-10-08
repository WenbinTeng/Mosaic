module conv1_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 1,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 512              // packing width
)
(
    // ------ clock and reset ------
    input  wire                     clk,
    input  wire                     rstn,
    // ------ LII phy input ------
    input  wire [PW-1:0]            lii_din_p0,
    input  wire                     lii_valid_in_p0,
    output wire                     lii_ready_in_p0,
    input  wire [7:0]               lii_src_in_p0,
    input  wire [7:0]               lii_dst_in_p0,
    // ------ LII phy output ------
    output wire [PW-1:0]            lii_dout_p0,
    output wire                     lii_valid_out_p0,
    input  wire                     lii_ready_out_p0,
    output wire [7:0]               lii_src_out_p0,
    output wire [7:0]               lii_dst_out_p0,
    // ------ connection to HLS kernel ------
    output wire [63:0]   in_stream_data,
    output wire                     in_stream_valid,
    input  wire                     in_stream_ready,
    input  wire [383:0]   out_stream_data
    input  wire                     out_stream_valid,
    output wire                     out_stream_ready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_ready_in_p0 =
        in_stream_ready;
    always @(*) begin
        in_stream_valid = lii_valid_in_p0;
        in_stream_data   = lii_din_p0[63:0];
    end

    // ========= output: pack =========
    assign lii_ready_out_p0 = 
        out_stream_valid;
    assign lii_dout_p0 = {
        out_stream_dout
    };
    assign { out_stream_ready } =
           { lii_ready_out_p0 };

    // ========= kernel clock gating =========
    assign ce = (in_stream_valid) &
                (~(~lii_ready_out_p0));

endmodule