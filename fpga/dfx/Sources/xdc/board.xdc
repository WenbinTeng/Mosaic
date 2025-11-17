# top_io_zcu102.xdc

create_clock -name clk -period 5     [get_ports clk]
set_property PACKAGE_PIN AL8         [get_ports clk]  ;# Bank  64 VCCO - VCC1V2   - IO_L12P_T1U_N10_GC_64
set_property IOSTANDARD  DIFF_SSTL12 [get_ports clk]  ;# Bank  64 VCCO - VCC1V2   - IO_L12P_T1U_N10_GC_64

set_property PACKAGE_PIN AN14        [get_ports rstn] ;# Bank  44 VCCO - VCC3V3   - IO_L1P_AD11P_44
set_property IOSTANDARD  LVCMOS33    [get_ports rstn] ;# Bank  44 VCCO - VCC3V3   - IO_L1P_AD11P_44