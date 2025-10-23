#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# 

###############################################################
### Design Settings
###############################################################
# Load utilities
#Define location for "Tcl" directory. Defaults to "./tcl_HD"
if {[file exists "./Tcl_HD"]} { 
   set tclDir  "./Tcl_HD"
} else {
   error "ERROR: No valid location found for required Tcl scripts. Set \$tclDir in design.tcl to a valid location."
}
puts "Setting TCL dir to $tclDir"

####Source required Tcl Procs
source $tclDir/design_utils.tcl
source $tclDir/log_utils.tcl
source $tclDir/synth_utils.tcl
source $tclDir/impl_utils.tcl
source $tclDir/hd_utils.tcl
source $tclDir/dfx_utils.tcl

###############################################################
### Board Settings
### -Board: default device, package and speed for selected board
###############################################################
set xboard "zcu102"

switch $xboard {
zcu102 {
 set device       "xczu9eg"
 set package      "-ffvb1156"
 set speed        "-2-e"
}
default {
 # zcu102
 set device       "xczu9eg"
 set package      "-ffvb1156"
 set speed        "-2-e"
}
}
set part         $device$package$speed
check_part $part


