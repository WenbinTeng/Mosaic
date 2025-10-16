`timescale 1ns/1ps

module lii_rr_arb #(
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