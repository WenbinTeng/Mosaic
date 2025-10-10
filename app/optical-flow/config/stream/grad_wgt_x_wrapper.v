`timescale 1ns/1ps

module grad_wgt_x_wrapper
#(
    parameter NIN  = 1,   // logic input streams
    parameter NOUT = 1,  // logic output streams
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
    output wire [95:0]   y_filt_grad_stream_tdata,
    output wire                     y_filt_grad_stream_tvalid,
    input  wire                     y_filt_grad_stream_tready,
    input  wire [95:0]   filt_grad_stream_tdata,
    input  wire                     filt_grad_stream_tvalid,
    output wire                     filt_grad_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        y_filt_grad_stream_tready;
    assign y_filt_grad_stream_tdata  = lii_in_p0_tdata[95:0];
    assign y_filt_grad_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        filt_grad_stream_tvalid;
    assign lii_out_p0_tdata = {
        filt_grad_stream_tdata
    };
    assign { filt_grad_stream_tready } =
           { lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (filt_grad_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule