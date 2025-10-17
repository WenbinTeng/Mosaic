`timescale 1ns/1ps

module lii_fifo #(
    parameter integer DW=256, SRC_W=8, DST_W=8, TYPE_W=2, DEPTH=2
)(
    input  wire                     clk,
    input  wire                     rstn,
    input  wire [DW-1:0]            s_data,
    input  wire [DW/8-1:0]          s_keep,
    input  wire [DW/8-1:0]          s_strb,
    input  wire                     s_last,
    input  wire [SRC_W-1:0]         s_src,
    input  wire [DST_W-1:0]         s_dst,
    input  wire [TYPE_W-1:0]        s_type,
    input  wire                     s_valid,
    output wire                     s_ready,

    output wire [DW-1:0]            m_data,
    output wire [DW/8-1:0]          m_keep,
    output wire [DW/8-1:0]          m_strb,
    output wire                     m_last,
    output wire [SRC_W-1:0]         m_src,
    output wire [DST_W-1:0]         m_dst,
    output wire [TYPE_W-1:0]        m_type,
    output wire                     m_valid,
    input  wire                     m_ready
);
    localparam integer AW = (DEPTH<=1) ? 1 : $clog2(DEPTH);
    localparam integer TW = DW + (DW/8) + (DW/8) + 1 + SRC_W + DST_W + TYPE_W;

    generate
        if (DEPTH == 0) begin
            assign m_data  = s_data;
            assign m_keep  = s_keep;
            assign m_strb  = s_strb;
            assign m_last  = s_last;
            assign m_src   = s_src;
            assign m_dst   = s_dst;
            assign m_type  = s_type;
            assign m_valid = s_valid;
            assign s_ready = m_ready;
        end else begin
            reg [TW-1:0] mem [0:DEPTH-1];
            reg [AW:0]   wr_ptr;
            reg [AW:0]   rd_ptr;

            wire empty = (wr_ptr == rd_ptr);
            wire full  = (wr_ptr[AW-1:0] == rd_ptr[AW-1:0]) && (wr_ptr[AW] != rd_ptr[AW]);

            assign s_ready = ~full;
            assign m_valid = ~empty;

            // write
            always @(posedge clk or negedge rstn) begin
                if (!rstn)
                    wr_ptr <= {AW+1{1'b0}};
                else if (s_valid && s_ready) begin
                    mem[wr_ptr[AW-1:0]] <= { s_data, s_keep, s_strb, s_last, s_src, s_dst, s_type };
                    wr_ptr <= wr_ptr + 1'b1;
                end
            end

            // read
            always @(posedge clk or negedge rstn) begin
                if (!rstn)
                    rd_ptr <= {AW+1{1'b0}};
                else if (m_valid && m_ready)
                    rd_ptr <= rd_ptr + 1'b1;
            end

            wire [TW-1:0] r = mem[rd_ptr[AW-1:0]];
            assign { m_data, m_keep, m_strb, m_last, m_src, m_dst, m_type } = r;
        end
    endgenerate
endmodule