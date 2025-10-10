`timescale 1ns/1ps

module coloring_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 1,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 1,             // phy out channels
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
    output wire [PW-1:0]            lii_out_p0_tdata,
    output wire                     lii_out_p0_tvalid,
    input  wire                     lii_out_p0_tready,
    output wire [7:0]               lii_out_p0_src,
    output wire [7:0]               lii_out_p0_dst,
    // ------ connection to HLS kernel ------
    output wire [23:0]   pixel_stream_tdata,
    output wire                     pixel_stream_tvalid,
    input  wire                     pixel_stream_tready,
    input  wire [7:0]   frame_stream_tdata,
    input  wire                     frame_stream_tvalid,
    output wire                     frame_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        pixel_stream_tready;
    assign pixel_stream_tdata  = lii_in_p0_tdata[23:0];
    assign pixel_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        frame_stream_tvalid;
    assign lii_out_p0_tdata = {
        frame_stream_tdata
    };
    assign { frame_stream_tready } =
           { lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (frame_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule