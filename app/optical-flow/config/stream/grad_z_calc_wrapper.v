`timescale 1ns/1ps

module grad_z_calc_wrapper
#(
    parameter NIN  = 5,   // logic input streams
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
    output wire [16:0]   frame1_stream_tdata,
    output wire                     frame1_stream_tvalid,
    input  wire                     frame1_stream_tready,
    output wire [16:0]   frame2_stream_tdata,
    output wire                     frame2_stream_tvalid,
    input  wire                     frame2_stream_tready,
    output wire [16:0]   frame3_stream_tdata,
    output wire                     frame3_stream_tvalid,
    input  wire                     frame3_stream_tready,
    output wire [16:0]   frame4_stream_tdata,
    output wire                     frame4_stream_tvalid,
    input  wire                     frame4_stream_tready,
    output wire [16:0]   frame5_stream_tdata,
    output wire                     frame5_stream_tvalid,
    input  wire                     frame5_stream_tready,
    input  wire [31:0]   gradient_z_stream_tdata,
    input  wire                     gradient_z_stream_tvalid,
    output wire                     gradient_z_stream_tready,
    // ------ clock enable for HLS kernel ------
    output wire                     ce
);

    // ========= input: unpack =========
    assign lii_in_p0_tready =
        frame1_stream_tready & frame2_stream_tready & frame3_stream_tready & frame4_stream_tready & frame5_stream_tready;
    assign frame1_stream_tdata  = lii_in_p0_tdata[16:0];
    assign frame1_stream_tvalid = lii_in_p0_tvalid;
    assign frame2_stream_tdata  = lii_in_p0_tdata[33:17];
    assign frame2_stream_tvalid = lii_in_p0_tvalid;
    assign frame3_stream_tdata  = lii_in_p0_tdata[50:34];
    assign frame3_stream_tvalid = lii_in_p0_tvalid;
    assign frame4_stream_tdata  = lii_in_p0_tdata[67:51];
    assign frame4_stream_tvalid = lii_in_p0_tvalid;
    assign frame5_stream_tdata  = lii_in_p0_tdata[84:68];
    assign frame5_stream_tvalid = lii_in_p0_tvalid;

    // ========= output: pack =========
    assign lii_out_p0_tvalid = 
        gradient_z_stream_tvalid;
    assign lii_out_p0_tdata = {
        gradient_z_stream_tdata
    };
    assign { gradient_z_stream_tready } =
           { lii_out_p0_tready };

    // ========= kernel clock gating =========
    assign ce = (gradient_z_stream_tvalid) &
                (lii_out_p0_tready) &
                (lii_in_p0_tready);
endmodule