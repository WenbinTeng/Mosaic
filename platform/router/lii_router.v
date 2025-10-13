`timescale 1ns/1ps

// --------------------------- router core -----------------------------------
module lii_router_core #(
    parameter integer N_IN        = 4,
    parameter integer N_OUT       = 2,
    parameter integer DW          = 256,
    parameter integer SRC_W       = 8,
    parameter integer DST_W       = 8,
    parameter integer TYPE_W      = 2,
    parameter integer ROUTE_MSB   = 7,
    parameter integer FIFO_DEPTH  = 2,
    parameter integer TYPE_PRIO   = 2
)(
    input  wire                          clk,
    input  wire                          rstn,
    // ---- LII inputs: flatten ----
    input  wire [N_IN*DW-1:0]            in_data,
    input  wire [N_IN*(DW/8)-1:0]        in_keep,
    input  wire [N_IN*(DW/8)-1:0]        in_strb,
    input  wire [N_IN-1:0]               in_last,
    input  wire [N_IN*SRC_W-1:0]         in_src,
    input  wire [N_IN*DST_W-1:0]         in_dst,
    input  wire [N_IN*TYPE_W-1:0]        in_type,
    input  wire [N_IN-1:0]               in_valid,
    output wire [N_IN-1:0]               in_ready,
    // ---- LII outputs: flatten ----
    output wire [N_OUT*DW-1:0]           out_data,
    output wire [N_OUT*(DW/8)-1:0]       out_keep,
    output wire [N_OUT*(DW/8)-1:0]       out_strb,
    output wire [N_OUT-1:0]              out_last,
    output wire [N_OUT*SRC_W-1:0]        out_src,
    output wire [N_OUT*DST_W-1:0]        out_dst,
    output wire [N_OUT*TYPE_W-1:0]       out_type,
    output wire [N_OUT-1:0]              out_valid,
    input  wire [N_OUT-1:0]              out_ready
);

    // ---------- optional input FIFOs (q_* are post-FIFO signals) ----------
    localparam integer TW = DW + (DW/8) + (DW/8) + 1 + SRC_W + DST_W + TYPE_W;

    wire [N_IN*DW-1:0]       q_data;
    wire [N_IN*(DW/8)-1:0]   q_keep;
    wire [N_IN*(DW/8)-1:0]   q_strb;
    wire [N_IN-1:0]          q_last;
    wire [N_IN*SRC_W-1:0]    q_src;
    wire [N_IN*DST_W-1:0]    q_dst;
    wire [N_IN*TYPE_W-1:0]   q_type;
    wire [N_IN-1:0]          q_valid;
    wire [N_IN-1:0]          q_ready_merge;
    wire [N_IN-1:0]          q_ready;

    genvar gi;
    generate
        if (FIFO_DEPTH == 0) begin
            assign q_data   = in_data;
            assign q_keep   = in_keep;
            assign q_strb   = in_strb;
            assign q_last   = in_last;
            assign q_src    = in_src;
            assign q_dst    = in_dst;
            assign q_type   = in_type;
            assign q_valid  = in_valid;
            assign q_ready  = q_ready_merge;
            assign in_ready = q_ready_merge;
        end else begin
            for (gi=0; gi<N_IN; gi=gi+1) begin
                lii_stream_fifo #(.DW(DW), .SRC_W(SRC_W), .DST_W(DST_W), .TYPE_W(TYPE_W), .DEPTH(FIFO_DEPTH)) U (
                                  .clk(clk), .rstn(rstn),
                                  .s_data ( in_data  [ (gi+1)*DW-1      : gi*DW     ] ),
                                  .s_keep ( in_keep  [ (gi+1)*(DW/8)-1  : gi*(DW/8) ] ),
                                  .s_strb ( in_strb  [ (gi+1)*(DW/8)-1  : gi*(DW/8) ] ),
                                  .s_last ( in_last  [ gi ] ),
                                  .s_src  ( in_src   [ (gi+1)*SRC_W-1   : gi*SRC_W  ] ),
                                  .s_dst  ( in_dst   [ (gi+1)*DST_W-1   : gi*DST_W  ] ),
                                  .s_type ( in_type  [ (gi+1)*TYPE_W-1  : gi*TYPE_W ] ),
                                  .s_valid( in_valid [ gi ] ),
                                  .s_ready( in_ready [ gi ] ),
                                  .m_data ( q_data   [ (gi+1)*DW-1      : gi*DW     ] ),
                                  .m_keep ( q_keep   [ (gi+1)*(DW/8)-1  : gi*(DW/8) ] ),
                                  .m_strb ( q_strb   [ (gi+1)*(DW/8)-1  : gi*(DW/8) ] ),
                                  .m_last ( q_last   [ gi ] ),
                                  .m_src  ( q_src    [ (gi+1)*SRC_W-1   : gi*SRC_W  ] ),
                                  .m_dst  ( q_dst    [ (gi+1)*DST_W-1   : gi*DST_W  ] ),
                                  .m_type ( q_type   [ (gi+1)*TYPE_W-1  : gi*TYPE_W ] ),
                                  .m_valid( q_valid  [ gi ] ),
                                  .m_ready( q_ready  [ gi ] ));
            end
        assign q_ready = q_ready_merge;
        end
    endgenerate

    // ---------- routing / arbitration ----------
    localparam integer OUTL     = (N_OUT<=1) ? 1 : $clog2(N_OUT);
    localparam integer SHIFT    = (ROUTE_MSB - OUTL + 1);
    localparam integer MASK_VAL = (1<<OUTL)-1;

    wire [N_OUT*N_IN-1:0] grant_flat;
    wire [N_OUT-1:0]      grant_v;

    genvar go, gj;
    generate
        for (go=0; go<N_OUT; go=go+1) begin
            wire [N_IN-1:0] req_vec;
            wire [N_IN-1:0] hi_vec;
            wire [N_IN-1:0] lo_vec;

            for (gj=0; gj<N_IN; gj=gj+1) begin
                wire [DST_W-1:0]  d_dst  = q_dst [ (gj+1)*DST_W-1  : gj*DST_W  ];
                wire [TYPE_W-1:0] d_type = q_type[ (gj+1)*TYPE_W-1 : gj*TYPE_W ];
                wire [OUTL-1:0]   rsel   = (SHIFT>=0) ? ((d_dst >> SHIFT) & MASK_VAL[OUTL-1:0]) : (d_dst[OUTL-1:0]);
                assign req_vec[gj] = q_valid[gj] & (rsel == go[OUTL-1:0]);
                assign hi_vec [gj] = req_vec[gj] & ((TYPE_PRIO >> d_type) & 1);
                assign lo_vec [gj] = req_vec[gj] & ~hi_vec[gj];
            end

            wire [N_IN-1:0] g_hi, g_lo;
            wire            v_hi, v_lo;

            rr_arb #(.N(N_IN)) U_HI (.clk(clk), .rstn(rstn),
                                    .req(hi_vec), .gnt(g_hi), .gnt_v(v_hi),
                                    .accept(out_ready[go] & v_hi));
            rr_arb #(.N(N_IN)) U_LO (.clk(clk), .rstn(rstn),
                                    .req(lo_vec), .gnt(g_lo), .gnt_v(v_lo),
                                    .accept(out_ready[go] & ~v_hi & v_lo));

            wire [N_IN-1:0] g_vec = v_hi ? g_hi : g_lo;
            assign grant_v[go] = v_hi | v_lo;
            for (gj=0; gj<N_IN; gj=gj+1) begin
                assign grant_flat[go*N_IN + gj] = g_vec[gj];
            end

            // ----------------- crossbar: one-hot OR -----------------
            reg [DW-1:0]      mux_data;
            reg [DW/8-1:0]    mux_keep, mux_strb;
            reg               mux_last;
            reg [SRC_W-1:0]   mux_src;
            reg [DST_W-1:0]   mux_dst;
            reg [TYPE_W-1:0]  mux_type;

            integer k;
            always @(*) begin
                mux_data = {DW{1'b0}};
                mux_keep = {(DW/8){1'b0}};
                mux_strb = {(DW/8){1'b0}};
                mux_last = 1'b0;
                mux_src  = {SRC_W{1'b0}};
                mux_dst  = {DST_W{1'b0}};
                mux_type = {TYPE_W{1'b0}};
                for (k=0; k<N_IN; k=k+1) begin
                    if (g_vec[k]) begin
                        mux_data = q_data[ k*DW     +: DW     ];
                        mux_keep = q_keep[ k*(DW/8) +: (DW/8) ];
                        mux_strb = q_strb[ k*(DW/8) +: (DW/8) ];
                        mux_last = q_last[ k ];
                        mux_src  = q_src [ k*SRC_W  +: SRC_W  ];
                        mux_dst  = q_dst [ k*DST_W  +: DST_W  ];
                        mux_type = q_type[ k*TYPE_W +: TYPE_W ];
                    end
                end
            end

            assign out_valid[ go ]                           = grant_v[ go ];
            assign out_data [ (go+1)*DW-1      : go*DW ]     = mux_data;
            assign out_keep [ (go+1)*(DW/8)-1  : go*(DW/8) ] = mux_keep;
            assign out_strb [ (go+1)*(DW/8)-1  : go*(DW/8) ] = mux_strb;
            assign out_last [ go ]                           = mux_last;
            assign out_src  [ (go+1)*SRC_W-1   : go*SRC_W ]  = mux_src;
            assign out_dst  [ (go+1)*DST_W-1   : go*DST_W ]  = mux_dst;
            assign out_type [ (go+1)*TYPE_W-1  : go*TYPE_W ] = mux_type;
        end
    endgenerate

    // ---------- merge READY back to inputs ----------
    genvar gm;
    generate
        for (gm=0; gm<N_IN; gm=gm+1) begin
            integer t;
            reg r;
            always @(*) begin
                r = 1'b0;
                for (t=0; t<N_OUT; t=t+1) begin
                    if (grant_flat[t*N_IN + gm] & out_ready[t] & out_valid[t])
                        r = 1'b1;
                end
            end
            assign q_ready_merge[gm] = r;
        end
    endgenerate

endmodule



// ----------------------- FIFO -----------------------
module lii_stream_fifo #(
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



// --------------------------- round-robin arb --------------------------
module rr_arb #(
    parameter integer N = 4
)(
    input  wire                 clk,
    input  wire                 rstn,
    input  wire [N-1:0]         req,
    output reg  [N-1:0]         gnt,
    output wire                 gnt_v,
    input  wire                 accept
);
    localparam integer PW = (N<=1)?1:$clog2(N);

    reg [PW-1:0] ptr;
    reg [PW-1:0] sel_idx;
    reg found;

    assign gnt_v = |gnt;

    // choose first '1' from ptr .. N-1, then 0 .. ptr-1
    integer i;
    always @(*) begin
        gnt = {N{1'b0}};
        found = 1'b0;
        sel_idx = {PW{1'b0}};
        // pass 1: ptr..N-1
        for (i=0; i<N; i=i+1) begin
            if (!found) begin
                if (req[(ptr+i)%N]) begin
                    found = 1'b1;
                    sel_idx = (ptr+i)%N;
                    gnt[sel_idx] = 1'b1;
                end
            end
        end
    end

    // rotate pointer on accept
    always @(posedge clk or negedge rstn) begin
        if (!rstn)
            ptr <= {PW{1'b0}};
        else if (gnt_v && accept)
            ptr <= (sel_idx+1) % N;
    end
endmodule
