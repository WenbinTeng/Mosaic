`timescale 1ns/1ps

module top_in_wrapper
#(
    parameter NIN  = 0,   // logic input streams
    parameter NOUT = 7,  // logic output streams
    parameter P    = 0,              // phy in channels
    parameter Q    = 1,             // phy out channels
    parameter PW   = 64              // packing width
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
    input  wire [7:0]   img_stream_tdata,
    input  wire                     img_stream_tvalid,
    output wire                     img_stream_tready,
    input  wire [7:0]   fc1_weight_stream_tdata,
    input  wire                     fc1_weight_stream_tvalid,
    output wire                     fc1_weight_stream_tready,
    input  wire [7:0]   fc1_bias_stream_tdata,
    input  wire                     fc1_bias_stream_tvalid,
    output wire                     fc1_bias_stream_tready,
    input  wire [7:0]   fc2_weight_stream_tdata,
    input  wire                     fc2_weight_stream_tvalid,
    output wire                     fc2_weight_stream_tready,
    input  wire [7:0]   fc2_bias_stream_tdata,
    input  wire                     fc2_bias_stream_tvalid,
    output wire                     fc2_bias_stream_tready,
    input  wire [7:0]   fc3_weight_stream_tdata,
    input  wire                     fc3_weight_stream_tvalid,
    output wire                     fc3_weight_stream_tready,
    input  wire [7:0]   fc3_bias_stream_tdata,
    input  wire                     fc3_bias_stream_tvalid,
    output wire                     fc3_bias_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        img_stream_tvalid & fc1_weight_stream_tvalid & fc1_bias_stream_tvalid & fc2_weight_stream_tvalid & fc2_bias_stream_tvalid & fc3_weight_stream_tvalid & fc3_bias_stream_tvalid;
    assign lii_out_p0_tdata = {
        img_stream_tdata, 
        fc1_weight_stream_tdata, 
        fc1_bias_stream_tdata, 
        fc2_weight_stream_tdata, 
        fc2_bias_stream_tdata, 
        fc3_weight_stream_tdata, 
        fc3_bias_stream_tdata
    };
    assign { img_stream_tready, fc1_weight_stream_tready, fc1_bias_stream_tready, fc2_weight_stream_tready, fc2_bias_stream_tready, fc3_weight_stream_tready, fc3_bias_stream_tready } =
           { lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready, lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (img_stream_tvalid & fc1_weight_stream_tvalid & fc1_bias_stream_tvalid & fc2_weight_stream_tvalid & fc2_bias_stream_tvalid & fc3_weight_stream_tvalid & fc3_bias_stream_tvalid) &
                (lii_out_p0_tready) &
                ();
endmodule