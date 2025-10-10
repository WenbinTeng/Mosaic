`timescale 1ns/1ps

module rasterization1_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 2,  // logic output streams
    parameter P    = 1,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 128              // packing width
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
    output wire [55:0]   triangle_2d_stream_tdata,
    output wire                     triangle_2d_stream_tvalid,
    input  wire                     triangle_2d_stream_tready,
    input  wire [55:0]   triangle_2d_copy_stream_tdata,
    input  wire                     triangle_2d_copy_stream_tvalid,
    output wire                     triangle_2d_copy_stream_tready,
    input  wire [57:0]   triangle_info_stream_tdata,
    input  wire                     triangle_info_stream_tvalid,
    output wire                     triangle_info_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        triangle_2d_stream_tready;
    assign triangle_2d_stream_tdata  = lii_in_p0_tdata[55:0];
    assign triangle_2d_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        triangle_2d_copy_stream_tvalid & triangle_info_stream_tvalid;
    assign lii_out_p0_tdata = {
        triangle_2d_copy_stream_tdata, 
        triangle_info_stream_tdata
    };
    assign { triangle_2d_copy_stream_tready, triangle_info_stream_tready } =
           { lii_out_p0_tready, lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (triangle_2d_copy_stream_tvalid & triangle_info_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule