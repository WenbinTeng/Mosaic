`timescale 1ns/1ps

module top_out_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 0,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 0,             // phy out channels
    parameter PW   = 64              // packing width
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
    // ------ connection to HLS kernel ------
    output wire [7:0]   frame_stream_tdata,
    output wire                     frame_stream_tvalid,
    input  wire                     frame_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        frame_stream_tready;
    assign frame_stream_tdata  = lii_in_p0_tdata[7:0];
    assign frame_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========

    // ========= kernel clock gating =========
    assign ce = () &
                () &
                (lii_in_p0_tready);
endmodule