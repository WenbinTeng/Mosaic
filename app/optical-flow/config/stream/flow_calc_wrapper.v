`timescale 1ns/1ps

module flow_calc_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 1,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 256              // packing width
)
(
    // ------ clock and reset ------
    input  wire                     aclk,
    input  wire                     arstn,
    // ------ LII phy input ------
    input  wire [PW-1:0]            lii_in_p0_tdata,
    input  wire                     lii_in_p0_tvalid,
    output wire                     lii_in_p0_tready,
    input  wire [7:0]               lii_in_p0_src,
    input  wire [7:0]               lii_in_p0_dst,
    // ------ LII phy output ------
    output wire [PW-1:0]            lii_out_p0_tdata,
    output wire                     lii_out_p0_tvalid,
    input  wire                     lii_out_p0_tready,
    output wire [7:0]               lii_out_p0_src,
    output wire [7:0]               lii_out_p0_dst,
    // ------ connection to HLS kernel ------
    output wire [191:0]   tensor_stream_tdata,
    output wire                     tensor_stream_tvalid,
    input  wire                     tensor_stream_tready,
    input  wire [63:0]   output_stream_tdata,
    input  wire                     output_stream_tvalid,
    output wire                     output_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        tensor_stream_tready;
    assign tensor_stream_tdata  = lii_in_p0_tdata[191:0];
    assign tensor_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        output_stream_tvalid;
    assign lii_out_p0_tdata = {
        output_stream_tdata
    };
    assign { output_stream_tready } =
           { lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (output_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule