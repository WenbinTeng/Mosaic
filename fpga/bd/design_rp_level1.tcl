
################################################################
# This is a generated script based on design: design_rp_level1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2023.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   if { [string compare $scripts_vivado_version $current_vivado_version] > 0 } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2042 -severity "ERROR" " This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Sourcing the script failed since it was created with a future version of Vivado."}

   } else {
     catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   }

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_rp_level1_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xczu7ev-ffvc1156-2-e
   set_property BOARD_PART xilinx.com:zcu104:part0:1.1 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name design_rp_level1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
xilinx.com:hls:alexnet_layer_1:1.0\
xilinx.com:ip:dfx_decoupler:1.0\
xilinx.com:ip:smartconnect:1.0\
xilinx.com:ip:xlslice:1.0\
xilinx.com:ip:axi_intc:4.1\
xilinx.com:ip:xlconcat:2.1\
xilinx.com:hls:alexnet_layer_2:1.0\
xilinx.com:hls:alexnet_layer_3:1.0\
xilinx.com:hls:alexnet_layer_4:1.0\
xilinx.com:hls:alexnet_layer_5:1.0\
xilinx.com:hls:alexnet_layer_6:1.0\
xilinx.com:hls:alexnet_layer_7:1.0\
xilinx.com:hls:alexnet_layer_8:1.0\
xilinx.com:ip:ila:6.2\
"

   set list_ips_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2011 -severity "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2012 -severity "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

if { $bCheckIPsPassed != 1 } {
  common::send_gid_msg -ssname BD::TCL -id 2023 -severity "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set M00_AXI [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 M00_AXI ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {49} \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.HAS_REGION {0} \
   CONFIG.NUM_READ_OUTSTANDING {16} \
   CONFIG.NUM_WRITE_OUTSTANDING {16} \
   CONFIG.PROTOCOL {AXI4} \
   ] $M00_AXI

  set S00_AXI [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S00_AXI ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {40} \
   CONFIG.ARUSER_WIDTH {16} \
   CONFIG.AWUSER_WIDTH {16} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {128} \
   CONFIG.HAS_BRESP {1} \
   CONFIG.HAS_BURST {1} \
   CONFIG.HAS_CACHE {1} \
   CONFIG.HAS_LOCK {1} \
   CONFIG.HAS_PROT {1} \
   CONFIG.HAS_QOS {1} \
   CONFIG.HAS_REGION {0} \
   CONFIG.HAS_RRESP {1} \
   CONFIG.HAS_WSTRB {1} \
   CONFIG.ID_WIDTH {16} \
   CONFIG.MAX_BURST_LENGTH {256} \
   CONFIG.NUM_READ_OUTSTANDING {8} \
   CONFIG.NUM_READ_THREADS {4} \
   CONFIG.NUM_WRITE_OUTSTANDING {8} \
   CONFIG.NUM_WRITE_THREADS {4} \
   CONFIG.PROTOCOL {AXI4} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {0} \
   CONFIG.SUPPORTS_NARROW_BURST {1} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {0} \
   ] $S00_AXI


  # Create ports
  set Din [ create_bd_port -dir I -from 15 -to 0 Din ]
  set M01_ARESETN [ create_bd_port -dir I -type rst M01_ARESETN ]
  set irq [ create_bd_port -dir O irq ]
  set s_axi_aclk [ create_bd_port -dir I -type clk s_axi_aclk ]
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S00_AXI:M00_AXI} \
   CONFIG.ASSOCIATED_RESET {M01_ARESETN} \
 ] $s_axi_aclk

  # Create instance: alexnet_layer_1_0, and set properties
  set alexnet_layer_1_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_1:1.0 alexnet_layer_1_0 ]

  # Create instance: axi_interconnect_0, and set properties
  set axi_interconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_0 ]
  set_property CONFIG.NUM_MI {9} $axi_interconnect_0


  # Create instance: dfx_decoupler_0, and set properties
  set dfx_decoupler_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_0 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_0


  # Create instance: smartconnect_0, and set properties
  set smartconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 smartconnect_0 ]
  set_property CONFIG.NUM_SI {8} $smartconnect_0


  # Create instance: xlslice_0, and set properties
  set xlslice_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_0 ]
  set_property CONFIG.DIN_WIDTH {16} $xlslice_0


  # Create instance: xlslice_1, and set properties
  set xlslice_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_1 ]
  set_property CONFIG.DIN_WIDTH {16} $xlslice_1


  # Create instance: axi_intc_0, and set properties
  set axi_intc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_intc:4.1 axi_intc_0 ]

  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property CONFIG.NUM_PORTS {8} $xlconcat_0


  # Create instance: dfx_decoupler_1, and set properties
  set dfx_decoupler_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_1 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_1


  # Create instance: xlslice_2, and set properties
  set xlslice_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_2 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {2} \
    CONFIG.DIN_TO {2} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_2


  # Create instance: xlslice_3, and set properties
  set xlslice_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_3 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {3} \
    CONFIG.DIN_TO {3} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_3


  # Create instance: dfx_decoupler_2, and set properties
  set dfx_decoupler_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_2 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_2


  # Create instance: xlslice_4, and set properties
  set xlslice_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_4 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {4} \
    CONFIG.DIN_TO {4} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_4


  # Create instance: xlslice_5, and set properties
  set xlslice_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_5 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {5} \
    CONFIG.DIN_TO {5} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_5


  # Create instance: dfx_decoupler_3, and set properties
  set dfx_decoupler_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_3 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_3


  # Create instance: xlslice_6, and set properties
  set xlslice_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_6 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {6} \
    CONFIG.DIN_TO {6} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_6


  # Create instance: xlslice_7, and set properties
  set xlslice_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_7 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {7} \
    CONFIG.DIN_TO {7} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_7


  # Create instance: dfx_decoupler_4, and set properties
  set dfx_decoupler_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_4 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_4


  # Create instance: xlslice_8, and set properties
  set xlslice_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_8 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {8} \
    CONFIG.DIN_TO {8} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_8


  # Create instance: xlslice_9, and set properties
  set xlslice_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_9 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {9} \
    CONFIG.DIN_TO {9} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_9


  # Create instance: dfx_decoupler_5, and set properties
  set dfx_decoupler_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_5 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_5


  # Create instance: xlslice_10, and set properties
  set xlslice_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_10 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {10} \
    CONFIG.DIN_TO {10} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_10


  # Create instance: xlslice_11, and set properties
  set xlslice_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_11 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {11} \
    CONFIG.DIN_TO {11} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_11


  # Create instance: dfx_decoupler_6, and set properties
  set dfx_decoupler_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_6 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_6


  # Create instance: xlslice_12, and set properties
  set xlslice_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_12 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {12} \
    CONFIG.DIN_TO {12} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_12


  # Create instance: xlslice_13, and set properties
  set xlslice_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_13 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {13} \
    CONFIG.DIN_TO {13} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_13


  # Create instance: dfx_decoupler_7, and set properties
  set dfx_decoupler_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:dfx_decoupler:1.0 dfx_decoupler_7 ]
  set_property -dict [list \
    CONFIG.ALL_PARAMS {INTF {axi_data {ID 0 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4 SIGNALS {ARVALID {WIDTH 1 PRESENT 1} ARREADY {WIDTH 1 PRESENT 1} AWVALID {WIDTH 1 PRESENT 1} AWREADY {WIDTH\
1 PRESENT 1} BVALID {WIDTH 1 PRESENT 1} BREADY {WIDTH 1 PRESENT 1} RVALID {WIDTH 1 PRESENT 1} RREADY {WIDTH 1 PRESENT 1} WVALID {WIDTH 1 PRESENT 1} WREADY {WIDTH 1 PRESENT 1} AWID {WIDTH 1 PRESENT 1} AWADDR\
{WIDTH 64 PRESENT 1} AWLEN {WIDTH 8 PRESENT 1} AWSIZE {WIDTH 3 PRESENT 1} AWBURST {WIDTH 2 PRESENT 0} AWLOCK {WIDTH 1 PRESENT 1} AWCACHE {WIDTH 4 PRESENT 1} AWPROT {WIDTH 3 PRESENT 1} AWREGION {WIDTH 4\
PRESENT 1} AWQOS {WIDTH 4 PRESENT 1} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 1 PRESENT 1} WDATA {WIDTH 32 PRESENT 1} WSTRB {WIDTH 4 PRESENT 1} WLAST {WIDTH 1 PRESENT 1} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH\
1 PRESENT 1} BRESP {WIDTH 2 PRESENT 1} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH 1 PRESENT 1} ARADDR {WIDTH 64 PRESENT 1} ARLEN {WIDTH 8 PRESENT 1} ARSIZE {WIDTH 3 PRESENT 1} ARBURST {WIDTH 2 PRESENT 0} ARLOCK\
{WIDTH 1 PRESENT 1} ARCACHE {WIDTH 4 PRESENT 1} ARPROT {WIDTH 3 PRESENT 1} ARREGION {WIDTH 4 PRESENT 1} ARQOS {WIDTH 4 PRESENT 1} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 1 PRESENT 1} RDATA {WIDTH 32 PRESENT\
1} RRESP {WIDTH 2 PRESENT 1} RLAST {WIDTH 1 PRESENT 1} RUSER {WIDTH 0 PRESENT 0}}} axi_control {ID 1 VLNV xilinx.com:interface:aximm_rtl:1.0 PROTOCOL AXI4LITE SIGNALS {ARVALID {PRESENT 1 WIDTH 1} ARREADY\
{PRESENT 1 WIDTH 1} AWVALID {PRESENT 1 WIDTH 1} AWREADY {PRESENT 1 WIDTH 1} BVALID {PRESENT 1 WIDTH 1} BREADY {PRESENT 1 WIDTH 1} RVALID {PRESENT 1 WIDTH 1} RREADY {PRESENT 1 WIDTH 1} WVALID {PRESENT 1\
WIDTH 1} WREADY {PRESENT 1 WIDTH 1} AWADDR {PRESENT 1 WIDTH 7} AWLEN {PRESENT 0 WIDTH 8} AWSIZE {PRESENT 0 WIDTH 3} AWBURST {PRESENT 0 WIDTH 2} AWLOCK {PRESENT 0 WIDTH 1} AWCACHE {PRESENT 0 WIDTH 4} AWPROT\
{PRESENT 0 WIDTH 3} WDATA {PRESENT 1 WIDTH 32} WSTRB {PRESENT 1 WIDTH 4} WLAST {PRESENT 0 WIDTH 1} BRESP {PRESENT 1 WIDTH 2} ARADDR {PRESENT 1 WIDTH 7} ARLEN {PRESENT 0 WIDTH 8} ARSIZE {PRESENT 0 WIDTH\
3} ARBURST {PRESENT 0 WIDTH 2} ARLOCK {PRESENT 0 WIDTH 1} ARCACHE {PRESENT 0 WIDTH 4} ARPROT {PRESENT 0 WIDTH 3} RDATA {PRESENT 1 WIDTH 32} RRESP {PRESENT 1 WIDTH 2} RLAST {PRESENT 0 WIDTH 1} AWID {WIDTH\
0 PRESENT 0} AWREGION {WIDTH 4 PRESENT 0} AWQOS {WIDTH 4 PRESENT 0} AWUSER {WIDTH 0 PRESENT 0} WID {WIDTH 0 PRESENT 0} WUSER {WIDTH 0 PRESENT 0} BID {WIDTH 0 PRESENT 0} BUSER {WIDTH 0 PRESENT 0} ARID {WIDTH\
0 PRESENT 0} ARREGION {WIDTH 4 PRESENT 0} ARQOS {WIDTH 4 PRESENT 0} ARUSER {WIDTH 0 PRESENT 0} RID {WIDTH 0 PRESENT 0} RUSER {WIDTH 0 PRESENT 0}} MODE slave} interrupt {ID 2 VLNV xilinx.com:signal:interrupt_rtl:1.0\
SIGNALS {INTERRUPT {PRESENT 1 WIDTH 1}}}} ALWAYS_HAVE_AXI_CLK 1 IPI_PROP_COUNT 0} \
    CONFIG.GUI_INTERFACE_NAME {axi_data} \
    CONFIG.GUI_SELECT_VLNV {xilinx.com:interface:aximm_rtl:1.0} \
  ] $dfx_decoupler_7


  # Create instance: xlslice_14, and set properties
  set xlslice_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_14 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {14} \
    CONFIG.DIN_TO {14} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_14


  # Create instance: xlslice_15, and set properties
  set xlslice_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_15 ]
  set_property -dict [list \
    CONFIG.DIN_FROM {15} \
    CONFIG.DIN_TO {15} \
    CONFIG.DIN_WIDTH {16} \
  ] $xlslice_15


  # Create instance: alexnet_layer_2_0, and set properties
  set alexnet_layer_2_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_2:1.0 alexnet_layer_2_0 ]

  # Create instance: alexnet_layer_3_0, and set properties
  set alexnet_layer_3_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_3:1.0 alexnet_layer_3_0 ]

  # Create instance: alexnet_layer_4_0, and set properties
  set alexnet_layer_4_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_4:1.0 alexnet_layer_4_0 ]

  # Create instance: alexnet_layer_5_0, and set properties
  set alexnet_layer_5_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_5:1.0 alexnet_layer_5_0 ]

  # Create instance: alexnet_layer_6_0, and set properties
  set alexnet_layer_6_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_6:1.0 alexnet_layer_6_0 ]

  # Create instance: alexnet_layer_7_0, and set properties
  set alexnet_layer_7_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_7:1.0 alexnet_layer_7_0 ]

  # Create instance: alexnet_layer_8_0, and set properties
  set alexnet_layer_8_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:alexnet_layer_8:1.0 alexnet_layer_8_0 ]

  # Create instance: ila_0, and set properties
  set ila_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila:6.2 ila_0 ]
  set_property CONFIG.C_DATA_DEPTH {8192} $ila_0


  # Create interface connections
  connect_bd_intf_net -intf_net alexnet_layer_1_0_m_axi_data [get_bd_intf_pins alexnet_layer_1_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_0/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_2_0_m_axi_data [get_bd_intf_pins alexnet_layer_2_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_1/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_3_0_m_axi_data [get_bd_intf_pins alexnet_layer_3_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_2/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_4_0_m_axi_data [get_bd_intf_pins alexnet_layer_4_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_3/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_5_0_m_axi_data [get_bd_intf_pins alexnet_layer_5_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_4/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_6_0_m_axi_data [get_bd_intf_pins alexnet_layer_6_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_5/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_7_0_m_axi_data [get_bd_intf_pins alexnet_layer_7_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_6/rp_axi_data]
  connect_bd_intf_net -intf_net alexnet_layer_8_0_m_axi_data [get_bd_intf_pins alexnet_layer_8_0/m_axi_data] [get_bd_intf_pins dfx_decoupler_7/rp_axi_data]
  connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI [get_bd_intf_pins axi_interconnect_0/M00_AXI] [get_bd_intf_pins dfx_decoupler_0/s_axi_control]
  connect_bd_intf_net -intf_net axi_interconnect_0_M01_AXI [get_bd_intf_pins axi_interconnect_0/M01_AXI] [get_bd_intf_pins dfx_decoupler_1/s_axi_control]
  connect_bd_intf_net -intf_net axi_interconnect_0_M02_AXI [get_bd_intf_pins axi_interconnect_0/M02_AXI] [get_bd_intf_pins dfx_decoupler_2/s_axi_control]
  connect_bd_intf_net -intf_net axi_interconnect_0_M03_AXI [get_bd_intf_pins axi_interconnect_0/M03_AXI] [get_bd_intf_pins dfx_decoupler_3/s_axi_control]
  connect_bd_intf_net -intf_net axi_interconnect_0_M04_AXI [get_bd_intf_pins dfx_decoupler_4/s_axi_control] [get_bd_intf_pins axi_interconnect_0/M04_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M05_AXI [get_bd_intf_pins dfx_decoupler_5/s_axi_control] [get_bd_intf_pins axi_interconnect_0/M05_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M06_AXI [get_bd_intf_pins dfx_decoupler_6/s_axi_control] [get_bd_intf_pins axi_interconnect_0/M06_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M07_AXI [get_bd_intf_pins dfx_decoupler_7/s_axi_control] [get_bd_intf_pins axi_interconnect_0/M07_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M08_AXI [get_bd_intf_pins axi_interconnect_0/M08_AXI] [get_bd_intf_pins axi_intc_0/s_axi]
  connect_bd_intf_net -intf_net dfx_decoupler_0_rp_axi_control [get_bd_intf_pins alexnet_layer_1_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_0/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_0_s_axi_data [get_bd_intf_pins dfx_decoupler_0/s_axi_data] [get_bd_intf_pins smartconnect_0/S00_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_1_rp_axi_control [get_bd_intf_pins alexnet_layer_2_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_1/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_1_s_axi_data [get_bd_intf_pins dfx_decoupler_1/s_axi_data] [get_bd_intf_pins smartconnect_0/S01_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_2_rp_axi_control [get_bd_intf_pins alexnet_layer_3_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_2/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_2_s_axi_data [get_bd_intf_pins dfx_decoupler_2/s_axi_data] [get_bd_intf_pins smartconnect_0/S02_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_3_rp_axi_control [get_bd_intf_pins alexnet_layer_4_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_3/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_3_s_axi_data [get_bd_intf_pins dfx_decoupler_3/s_axi_data] [get_bd_intf_pins smartconnect_0/S03_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_4_rp_axi_control [get_bd_intf_pins alexnet_layer_5_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_4/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_4_s_axi_data [get_bd_intf_pins dfx_decoupler_4/s_axi_data] [get_bd_intf_pins smartconnect_0/S04_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_5_rp_axi_control [get_bd_intf_pins alexnet_layer_6_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_5/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_5_s_axi_data [get_bd_intf_pins dfx_decoupler_5/s_axi_data] [get_bd_intf_pins smartconnect_0/S05_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_6_rp_axi_control [get_bd_intf_pins alexnet_layer_7_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_6/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_6_s_axi_data [get_bd_intf_pins dfx_decoupler_6/s_axi_data] [get_bd_intf_pins smartconnect_0/S06_AXI]
  connect_bd_intf_net -intf_net dfx_decoupler_7_rp_axi_control [get_bd_intf_pins alexnet_layer_8_0/s_axi_control] [get_bd_intf_pins dfx_decoupler_7/rp_axi_control]
  connect_bd_intf_net -intf_net dfx_decoupler_7_s_axi_data [get_bd_intf_pins dfx_decoupler_7/s_axi_data] [get_bd_intf_pins smartconnect_0/S07_AXI]
  connect_bd_intf_net -intf_net smartconnect_0_M00_AXI [get_bd_intf_pins smartconnect_0/M00_AXI] [get_bd_intf_ports M00_AXI]
connect_bd_intf_net -intf_net [get_bd_intf_nets smartconnect_0_M00_AXI] [get_bd_intf_pins smartconnect_0/M00_AXI] [get_bd_intf_pins ila_0/SLOT_0_AXI]
  connect_bd_intf_net -intf_net zynq_ultra_ps_e_0_M_AXI_HPM0_FPD [get_bd_intf_ports S00_AXI] [get_bd_intf_pins axi_interconnect_0/S00_AXI]

  # Create port connections
  connect_bd_net -net alexnet_layer_1_0_interrupt [get_bd_pins alexnet_layer_1_0/interrupt] [get_bd_pins dfx_decoupler_0/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_2_0_interrupt [get_bd_pins alexnet_layer_2_0/interrupt] [get_bd_pins dfx_decoupler_1/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_3_0_interrupt [get_bd_pins alexnet_layer_3_0/interrupt] [get_bd_pins dfx_decoupler_2/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_4_0_interrupt [get_bd_pins alexnet_layer_4_0/interrupt] [get_bd_pins dfx_decoupler_3/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_5_0_interrupt [get_bd_pins alexnet_layer_5_0/interrupt] [get_bd_pins dfx_decoupler_4/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_6_0_interrupt [get_bd_pins alexnet_layer_6_0/interrupt] [get_bd_pins dfx_decoupler_5/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_7_0_interrupt [get_bd_pins alexnet_layer_7_0/interrupt] [get_bd_pins dfx_decoupler_6/rp_interrupt_INTERRUPT]
  connect_bd_net -net alexnet_layer_8_0_interrupt [get_bd_pins alexnet_layer_8_0/interrupt] [get_bd_pins dfx_decoupler_7/rp_interrupt_INTERRUPT]
  connect_bd_net -net axi_intc_0_irq [get_bd_pins axi_intc_0/irq] [get_bd_ports irq]
  connect_bd_net -net dfx_decoupler_0_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_0/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In0]
  connect_bd_net -net dfx_decoupler_1_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_1/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In1]
  connect_bd_net -net dfx_decoupler_2_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_2/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In2]
  connect_bd_net -net dfx_decoupler_3_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_3/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In3]
  connect_bd_net -net dfx_decoupler_4_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_4/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In4]
  connect_bd_net -net dfx_decoupler_5_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_5/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In5]
  connect_bd_net -net dfx_decoupler_6_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_6/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In6]
  connect_bd_net -net dfx_decoupler_7_s_interrupt_INTERRUPT [get_bd_pins dfx_decoupler_7/s_interrupt_INTERRUPT] [get_bd_pins xlconcat_0/In7]
  connect_bd_net -net rst_ps8_0_100M_peripheral_aresetn [get_bd_ports M01_ARESETN] [get_bd_pins axi_interconnect_0/M01_ARESETN] [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins dfx_decoupler_0/axi_control_arstn] [get_bd_pins dfx_decoupler_0/axi_data_arstn] [get_bd_pins axi_intc_0/s_axi_aresetn] [get_bd_pins smartconnect_0/aresetn] [get_bd_pins dfx_decoupler_1/axi_control_arstn] [get_bd_pins dfx_decoupler_1/axi_data_arstn] [get_bd_pins dfx_decoupler_2/axi_control_arstn] [get_bd_pins dfx_decoupler_2/axi_data_arstn] [get_bd_pins dfx_decoupler_3/axi_control_arstn] [get_bd_pins dfx_decoupler_3/axi_data_arstn] [get_bd_pins dfx_decoupler_4/axi_control_arstn] [get_bd_pins dfx_decoupler_4/axi_data_arstn] [get_bd_pins dfx_decoupler_5/axi_control_arstn] [get_bd_pins dfx_decoupler_5/axi_data_arstn] [get_bd_pins dfx_decoupler_6/axi_control_arstn] [get_bd_pins dfx_decoupler_6/axi_data_arstn] [get_bd_pins dfx_decoupler_7/axi_control_arstn] [get_bd_pins dfx_decoupler_7/axi_data_arstn] [get_bd_pins axi_interconnect_0/M02_ARESETN] [get_bd_pins axi_interconnect_0/M03_ARESETN] [get_bd_pins axi_interconnect_0/M04_ARESETN] [get_bd_pins axi_interconnect_0/M05_ARESETN] [get_bd_pins axi_interconnect_0/M06_ARESETN] [get_bd_pins axi_interconnect_0/M07_ARESETN] [get_bd_pins axi_interconnect_0/M08_ARESETN]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins xlconcat_0/dout] [get_bd_pins axi_intc_0/intr]
  connect_bd_net -net xlslice_0_Dout [get_bd_pins xlslice_0/Dout] [get_bd_pins dfx_decoupler_0/decouple]
  connect_bd_net -net xlslice_10_Dout [get_bd_pins xlslice_10/Dout] [get_bd_pins dfx_decoupler_5/decouple]
  connect_bd_net -net xlslice_11_Dout [get_bd_pins xlslice_11/Dout] [get_bd_pins alexnet_layer_6_0/ap_rst_n]
  connect_bd_net -net xlslice_12_Dout [get_bd_pins xlslice_12/Dout] [get_bd_pins dfx_decoupler_6/decouple]
  connect_bd_net -net xlslice_13_Dout [get_bd_pins xlslice_13/Dout] [get_bd_pins alexnet_layer_7_0/ap_rst_n]
  connect_bd_net -net xlslice_14_Dout [get_bd_pins xlslice_14/Dout] [get_bd_pins dfx_decoupler_7/decouple]
  connect_bd_net -net xlslice_15_Dout [get_bd_pins xlslice_15/Dout] [get_bd_pins alexnet_layer_8_0/ap_rst_n]
  connect_bd_net -net xlslice_1_Dout [get_bd_pins xlslice_1/Dout] [get_bd_pins alexnet_layer_1_0/ap_rst_n]
  connect_bd_net -net xlslice_2_Dout [get_bd_pins xlslice_2/Dout] [get_bd_pins dfx_decoupler_1/decouple]
  connect_bd_net -net xlslice_3_Dout [get_bd_pins xlslice_3/Dout] [get_bd_pins alexnet_layer_2_0/ap_rst_n]
  connect_bd_net -net xlslice_4_Dout [get_bd_pins xlslice_4/Dout] [get_bd_pins dfx_decoupler_2/decouple]
  connect_bd_net -net xlslice_5_Dout [get_bd_pins xlslice_5/Dout] [get_bd_pins alexnet_layer_3_0/ap_rst_n]
  connect_bd_net -net xlslice_6_Dout [get_bd_pins xlslice_6/Dout] [get_bd_pins dfx_decoupler_3/decouple]
  connect_bd_net -net xlslice_7_Dout [get_bd_pins xlslice_7/Dout] [get_bd_pins alexnet_layer_4_0/ap_rst_n]
  connect_bd_net -net xlslice_8_Dout [get_bd_pins xlslice_8/Dout] [get_bd_pins dfx_decoupler_4/decouple]
  connect_bd_net -net xlslice_9_Dout [get_bd_pins xlslice_9/Dout] [get_bd_pins alexnet_layer_5_0/ap_rst_n]
  connect_bd_net -net zynq_ultra_ps_e_0_emio_gpio_o [get_bd_ports Din] [get_bd_pins xlslice_1/Din] [get_bd_pins xlslice_0/Din] [get_bd_pins xlslice_2/Din] [get_bd_pins xlslice_3/Din] [get_bd_pins xlslice_4/Din] [get_bd_pins xlslice_5/Din] [get_bd_pins xlslice_6/Din] [get_bd_pins xlslice_7/Din] [get_bd_pins xlslice_8/Din] [get_bd_pins xlslice_9/Din] [get_bd_pins xlslice_10/Din] [get_bd_pins xlslice_11/Din] [get_bd_pins xlslice_12/Din] [get_bd_pins xlslice_13/Din] [get_bd_pins xlslice_14/Din] [get_bd_pins xlslice_15/Din]
  connect_bd_net -net zynq_ultra_ps_e_0_pl_clk0 [get_bd_ports s_axi_aclk] [get_bd_pins axi_interconnect_0/M01_ACLK] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins smartconnect_0/aclk] [get_bd_pins alexnet_layer_1_0/ap_clk] [get_bd_pins dfx_decoupler_0/axi_control_aclk] [get_bd_pins dfx_decoupler_0/axi_data_aclk] [get_bd_pins axi_intc_0/s_axi_aclk] [get_bd_pins alexnet_layer_2_0/ap_clk] [get_bd_pins dfx_decoupler_1/axi_control_aclk] [get_bd_pins dfx_decoupler_1/axi_data_aclk] [get_bd_pins alexnet_layer_3_0/ap_clk] [get_bd_pins dfx_decoupler_2/axi_control_aclk] [get_bd_pins dfx_decoupler_2/axi_data_aclk] [get_bd_pins alexnet_layer_4_0/ap_clk] [get_bd_pins dfx_decoupler_3/axi_control_aclk] [get_bd_pins dfx_decoupler_3/axi_data_aclk] [get_bd_pins alexnet_layer_5_0/ap_clk] [get_bd_pins dfx_decoupler_4/axi_control_aclk] [get_bd_pins dfx_decoupler_4/axi_data_aclk] [get_bd_pins alexnet_layer_6_0/ap_clk] [get_bd_pins dfx_decoupler_5/axi_control_aclk] [get_bd_pins dfx_decoupler_5/axi_data_aclk] [get_bd_pins alexnet_layer_7_0/ap_clk] [get_bd_pins dfx_decoupler_6/axi_control_aclk] [get_bd_pins dfx_decoupler_6/axi_data_aclk] [get_bd_pins alexnet_layer_8_0/ap_clk] [get_bd_pins dfx_decoupler_7/axi_control_aclk] [get_bd_pins dfx_decoupler_7/axi_data_aclk] [get_bd_pins axi_interconnect_0/M02_ACLK] [get_bd_pins axi_interconnect_0/M03_ACLK] [get_bd_pins axi_interconnect_0/M04_ACLK] [get_bd_pins axi_interconnect_0/M05_ACLK] [get_bd_pins axi_interconnect_0/M06_ACLK] [get_bd_pins axi_interconnect_0/M07_ACLK] [get_bd_pins axi_interconnect_0/M08_ACLK] [get_bd_pins ila_0/clk]

  # Create address segments
  assign_bd_address -external -dict [list offset 0x00000000 range 0x80000000 name SEG_zynq_ultra_ps_e_0_HPC0_DDR_LOW offset 0xC0000000 range 0x20000000 name SEG_zynq_ultra_ps_e_0_HPC0_QSPI] -target_address_space [get_bd_addr_spaces alexnet_layer_1_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_2_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_3_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_4_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_5_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_6_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_7_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x20000000 -target_address_space [get_bd_addr_spaces alexnet_layer_8_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg] -force
  assign_bd_address -offset 0xA0000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_1_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_2_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_3_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00020000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_4_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_5_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_6_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00050000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_7_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0x00060000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs alexnet_layer_8_0/s_axi_control/Reg] -force
  assign_bd_address -offset 0xA0010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces S00_AXI] [get_bd_addr_segs axi_intc_0/S_AXI/Reg] -force

  # Exclude Address Segments
  exclude_bd_addr_seg -offset 0xFF000000 -range 0x01000000 -target_address_space [get_bd_addr_spaces alexnet_layer_1_0/Data_m_axi_data] [get_bd_addr_segs M00_AXI/Reg]

  set_property USAGE memory [get_bd_addr_segs M00_AXI/Reg]


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


