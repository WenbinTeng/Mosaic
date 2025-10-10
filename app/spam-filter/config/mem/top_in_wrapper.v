`timescale 1ns/1ps

module top_in_wrapper
#(
    parameter NIN  = 0,   // logic input streams
    parameter NOUT = 2,  // logic output streams
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
    input  wire [15:0]   data_stream_tdata,
    input  wire                     data_stream_tvalid,
    output wire                     data_stream_tready,
    input  wire [7:0]   label_stream_tdata,
    input  wire                     label_stream_tvalid,
    output wire                     label_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        data_stream_tvalid & label_stream_tvalid;
    assign lii_out_p0_tdata = {
        data_stream_tdata, 
        label_stream_tdata
    };
    assign { data_stream_tready, label_stream_tready } =
           { lii_out_p0_tready, lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (data_stream_tvalid & label_stream_tvalid) &
                (lii_out_p0_tready) &
                ();
endmodule