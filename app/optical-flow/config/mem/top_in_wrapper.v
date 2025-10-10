`timescale 1ns/1ps

module top_in_wrapper
#(
    parameter NIN  = 0,   // logic input streams
    parameter NOUT = 6,  // logic output streams
    parameter P    = 0,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 128              // packing width
)
(
    // ------ clock and reset ------
    input  wire                     aclk,
    input  wire                     arstn,
    // ------ LII phy input ------
    // ------ LII phy output ------
    output wire [PW-1:0]            lii_out_p0_tdata,
    output wire                     lii_out_p0_tvalid,
    input  wire                     lii_out_p0_tready,
    output wire [7:0]               lii_out_p0_src,
    output wire [7:0]               lii_out_p0_dst,
    // ------ connection to HLS kernel ------
    input  wire [16:0]   frame1_stream_tdata,
    input  wire                     frame1_stream_tvalid,
    output wire                     frame1_stream_tready,
    input  wire [16:0]   frame2_stream_tdata,
    input  wire                     frame2_stream_tvalid,
    output wire                     frame2_stream_tready,
    input  wire [16:0]   frame3_stream_a_tdata,
    input  wire                     frame3_stream_a_tvalid,
    output wire                     frame3_stream_a_tready,
    input  wire [16:0]   frame3_stream_b_tdata,
    input  wire                     frame3_stream_b_tvalid,
    output wire                     frame3_stream_b_tready,
    input  wire [16:0]   frame4_stream_tdata,
    input  wire                     frame4_stream_tvalid,
    output wire                     frame4_stream_tready,
    input  wire [16:0]   frame5_stream_tdata,
    input  wire                     frame5_stream_tvalid,
    output wire                     frame5_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        frame1_stream_tvalid & frame2_stream_tvalid & frame3_stream_a_tvalid & frame3_stream_b_tvalid & frame4_stream_tvalid & frame5_stream_tvalid;
    assign lii_out_p0_tdata = {
        frame1_stream_tdata, 
        frame2_stream_tdata, 
        frame3_stream_a_tdata, 
        frame3_stream_b_tdata, 
        frame4_stream_tdata, 
        frame5_stream_tdata
    };
    assign { frame1_stream_tready, frame2_stream_tready, frame3_stream_a_tready, frame3_stream_b_tready, frame4_stream_tready, frame5_stream_tready } =
           { lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (frame1_stream_tvalid & frame2_stream_tvalid & frame3_stream_a_tvalid & frame3_stream_b_tvalid & frame4_stream_tvalid & frame5_stream_tvalid) &
                (lii_out_p0_tready) &
                ();
endmodule