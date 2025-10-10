`timescale 1ns/1ps

module top_in_mem_wrapper #(
    parameter AXI_AW = 48,
    parameter AXI_DW = 64,
    parameter LII_DW = 128
)(
    // ---------- clock and reset ----------
    input  wire                 clk,
    input  wire                 rstn,
    // ---------- AXI4 (Master) ----------
    // Read address
    input  wire [AXI_AW-1:0]    aximm_araddr,
    input  wire [7:0]           aximm_arlen,
    input  wire [2:0]           aximm_arsize,
    input  wire                 aximm_arvalid,
    output reg                  aximm_arready,
    // Read data
    output reg  [AXI_DW-1:0]    aximm_rdata,
    output reg  [1:0]           aximm_rresp,
    output reg                  aximm_rlast,
    output reg                  aximm_rvalid,
    input  wire                 aximm_rready,
    // Write address
    input  wire [AXI_AW-1:0]    aximm_awaddr,
    input  wire [7:0]           aximm_awlen,
    input  wire [2:0]           aximm_awsize,
    input  wire                 aximm_awvalid,
    output reg                  aximm_awready,
    // Write data
    input  wire [AXI_DW-1:0]    aximm_wdata,
    input  wire [AXI_DW/8-1:0]  aximm_wstrb,
    input  wire                 aximm_wlast,
    input  wire                 aximm_wvalid,
    output reg                  aximm_wready,
    // Write response
    output reg  [1:0]           aximm_bresp,
    output reg                  aximm_bvalid,
    input  wire                 aximm_bready,
    // ---------- LII request stream (out) ----------
    output reg  [LII_DW-1:0]    lii_req_tdata,
    output reg  [LII_DW/8-1:0]  lii_req_tkeep,
    output reg  [LII_DW/8-1:0]  lii_req_tstrb,
    output reg                  lii_req_tlast,
    output reg  [7:0]           lii_req_src,
    output reg  [7:0]           lii_req_dst,
    output reg                  lii_req_tvalid,
    input  wire                 lii_req_tready,
    // ---------- LII response stream (in) ----------
    input  wire [LII_DW-1:0]    lii_resp_tdata,
    input  wire [LII_DW/8-1:0]  lii_resp_tkeep,
    input  wire [LII_DW/8-1:0]  lii_resp_tstrb,
    input  wire                 lii_resp_tlast,
    input  wire [7:0]           lii_resp_src,
    input  wire [7:0]           lii_resp_dst,
    input  wire                 lii_resp_tvalid,
    output reg                  lii_resp_tready,
    // Simple TAG source & static route (optional)
    input  wire [7:0]           tag_alloc,
    input  wire [7:0]           cfg_src,
    input  wire [7:0]           cfg_dst
);

    localparam OP_READ  = 2'b00;
    localparam OP_WRITE = 2'b01;

    // ================ Request path ================
    typedef enum logic [1:0] {Q_IDLE, Q_HDR_RD, Q_HDR_WR, Q_SEND_W} qstate_t;
    qstate_t qs, qs_n;

    reg [AXI_AW-1:0]    addr_q;
    reg [7:0]           len_q;
    reg [2:0]           size_q;
    reg [7:0]           tag_q;

    wire req_handshake = lii_req_tvalid & lii_req_tready;

    // pack header into MSB side; clear lower area
    function [LII_DW-1:0] pack_hdr(
        input [1:0]         op,
        input [7:0]         len,
        input [2:0]         size,
        input [AXI_AW-1:0]  addr,
        input [7:0]         tag
    );
        reg [LII_DW-1:0] t;
        integer msb;
    begin
        t = 'b0;
        msb = LII_DW-1;
        t[msb -: 2] = op;        msb -= 2;
        t[msb -: 8] = len;       msb -= 8;
        t[msb -: 3] = size;      msb -= 3;
        t[msb -: AXI_AW] = addr; msb -= AXI_AW;
        t[msb -: 8]  = tag;      msb -= 8;
        pack_hdr = t;
    end
    endfunction

    // req FSM
    always @(*) begin
        qs_n = qs;
        lii_req_tvalid = 1'b0;
        lii_req_tlast  = 1'b0;
        lii_req_tdata  = '0;
        lii_req_tkeep  = {LII_DW/8{1'b1}};
        lii_req_tstrb  = '0;
        lii_req_src   = cfg_src;
        lii_req_dst   = cfg_dst;
        aximm_arready = 1'b0;
        aximm_awready = 1'b0;
        aximm_wready  = 1'b0;

        case (qs)
            Q_IDLE: begin
                // priority: write address if both valid (can tweak)
                if (aximm_awvalid) begin
                    aximm_awready = 1'b1;
                    if (aximm_awvalid && aximm_awready) qs_n = Q_HDR_WR;
                end else if (aximm_arvalid) begin
                    aximm_arready = 1'b1;
                    if (aximm_arvalid && aximm_arready) qs_n = Q_HDR_RD;
                end
            end

            Q_HDR_RD: begin
                // emit READ header
                lii_req_tvalid = 1'b1;
                lii_req_tlast  = 1'b1;
                lii_req_tdata  = pack_hdr(OP_READ, len_q, size_q, addr_q, tag_q);
                if (req_handshake) qs_n = Q_IDLE;
            end

            Q_HDR_WR: begin
                // emit WRITE header then go send write data beats
                lii_req_tvalid = 1'b1;
                lii_req_tlast  = 1'b0;
                lii_req_tdata  = pack_hdr(OP_WRITE, len_q, size_q, addr_q, tag_q);
                if (req_handshake) qs_n = Q_SEND_W;
            end

            Q_SEND_W: begin
                // stream aximm_wdata beats as data flits
                aximm_wready  = lii_req_tready;
                lii_req_tvalid = aximm_wvalid;
                lii_req_tlast  = aximm_wlast & aximm_wvalid;
                // place write data at LSB, aximm_wstrb mirrored to KEEP/STRB low bytes
                lii_req_tdata[AXI_DW-1:0]   = aximm_wdata;
                lii_req_tstrb[AXI_DW/8-1:0] = aximm_wstrb;
                lii_req_tkeep[AXI_DW/8-1:0] = aximm_wstrb;
                if (aximm_wvalid && aximm_wready && aximm_wlast) qs_n = Q_IDLE;
            end
        endcase
    end

    // capture address/len/size/tag when handshake AW/AR
    always @(posedge clk) begin
        if (!rstn) begin
            qs     <= Q_IDLE;
            addr_q <= '0;
            len_q  <= '0;
            size_q <= '0;
            tag_q  <= '0;
        end else begin
            qs <= qs_n;
            if (aximm_arvalid && aximm_arready) begin
                addr_q <= aximm_araddr;
                len_q  <= aximm_arlen;
                size_q <= aximm_arsize;
                tag_q  <= tag_alloc;
            end
            if (aximm_awvalid && aximm_awready) begin
                addr_q <= aximm_awaddr;
                len_q  <= aximm_awlen;
                size_q <= aximm_awsize;
                tag_q  <= tag_alloc;
            end
        end
    end

    // ================ Response path ================
    // For READ: pass DATA beats to R*, LAST mirrors resp_last.
    // For WRITE: expect a single ack flit (no DATA payload), produce B*.
    localparam RESP_OKAY  = 2'b00;
    localparam RESP_SLVERR= 2'b10;

    // simple decode: assume resp_data[1:0] carries RESP when last=1 and no data
    wire resp_has_data = |lii_resp_tkeep; // heuristic: any byte marked valid -> data beat

    always @(*) begin
        // defaults
        aximm_rdata    = 'b0;
        aximm_rresp    = RESP_OKAY;
        aximm_rlast    = 1'b0;
        aximm_rvalid   = 1'b0;
        aximm_bresp    = RESP_OKAY;
        aximm_bvalid   = 1'b0;
        lii_resp_tready = 1'b0;

        if (lii_resp_tvalid) begin
            if (resp_has_data) begin
                // READ data beat
                lii_resp_tready = aximm_rready;
                aximm_rvalid   = 1'b1;
                aximm_rdata    = lii_resp_tdata[AXI_DW-1:0];
                aximm_rlast    = lii_resp_tlast;
                // (optional) decode RESP from high bits if you embed it
            end else begin
                // WRITE ack (no data)
                lii_resp_tready = aximm_bready;
                aximm_bvalid   = 1'b1;
                aximm_bresp    = lii_resp_tdata[1:0]; // or tie to OKAY
            end
        end
    end

endmodule