`timescale 1ns/1ps

module grad_xy_calc_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 2,  // logic output streams
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
    output wire [16:0]   frame_stream_tdata,
    output wire                     frame_stream_tvalid,
    input  wire                     frame_stream_tready,
    input  wire [31:0]   gradient_x_stream_tdata,
    input  wire                     gradient_x_stream_tvalid,
    output wire                     gradient_x_stream_tready,
    input  wire [31:0]   gradient_y_stream_tdata,
    input  wire                     gradient_y_stream_tvalid,
    output wire                     gradient_y_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        frame_stream_tready;
    assign frame_stream_tdata  = lii_in_p0_tdata[16:0];
    assign frame_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        gradient_x_stream_tvalid & gradient_y_stream_tvalid;
    assign lii_out_p0_tdata = {
        gradient_x_stream_tdata, 
        gradient_y_stream_tdata
    };
    assign { gradient_x_stream_tready, gradient_y_stream_tready } =
           { lii_out_p0_tready, lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (gradient_x_stream_tvalid & gradient_y_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule