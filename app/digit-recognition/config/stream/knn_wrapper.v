`timescale 1ns/1ps

module knn_wrapper
#(
    parameter NIN  = 2,   // logic input streams
    parameter NOUT = 1,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 512              // packing width
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
    output wire [255:0]   training_stream_tdata,
    output wire                     training_stream_tvalid,
    input  wire                     training_stream_tready,
    output wire [255:0]   test_stream_tdata,
    output wire                     test_stream_tvalid,
    input  wire                     test_stream_tready,
    input  wire [7:0]   out_stream_tdata,
    input  wire                     out_stream_tvalid,
    output wire                     out_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        training_stream_tready & test_stream_tready;
    assign training_stream_tdata  = lii_in_p0_tdata[255:0];
    assign training_stream_tvalid = lii_in_p0_tvalid;
    assign test_stream_tdata  = lii_in_p0_tdata[511:256];
    assign test_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        out_stream_tvalid;
    assign lii_out_p0_tdata = {
        out_stream_tdata
    };
    assign { out_stream_tready } =
           { lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (out_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule