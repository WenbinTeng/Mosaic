# run_synth.tcl

###############################################################
###  Minimum settings required to run DFX flow:
###  1. Specify flow steps
###  2. Define target board
###  3. Identify source directories
###  4. Define static module
###  5. Define RPs, and their RM variants
###############################################################
#### flow control (1 = run step , 0 = skip step)
set run.topSynth       1 ;#synthesize static
set run.rmSynth        1 ;#synthesize RM variants

###############################################################
###  Run Settings
###############################################################
#### Input Directories
set srcDir     "./Sources"
set rtlDir     "$srcDir/hdl"
set prjDir     "$srcDir/prj"
set xdcDir     "$srcDir/xdc"
set coreDir    "$srcDir/cores"
set netlistDir "$srcDir/netlist"

#### Output Directories
set synthDir  "./Synth"
set implDir   "./Implement"
set dcpDir    "./Checkpoint"
set bitDir    "./Bitstreams"

###############################################################
### Static Module Definition
###############################################################
set top "top"

###############################################################
### RP & RM Definitions (Repeat for each RP)
### 1. Define Reconfigurable Partition (RP) name
### 2. Associate Reconfigurable Modules (RMs) to the RP
###############################################################

{%- for l1_module in modules %}
{%- for l2_module in l1_module.children %}
set rp_{{l2_module.name}} "{{l2_module.name}}"
set rm_variants($rp_{{l2_module.name}}) "{{l2_module.insts|join(' ')}}"
{%- endfor %}
set rp_{{l1_module.name}} "{{l1_module.name}}"
set rm_variants($rp_{{l1_module.name}}) "{{l1_module.insts|join(' ')}}"
{%- endfor %}

########################################################################
### RM Configurations (Valid combinations of RM variants)
### 1. Define initial configuration: rm_config(initial)
### 2. Define additional configurations: rm_config(xyz)
########################################################################

# This section is not used in Lab 8
# run_dfx.tcl is not set up for Nested DFX

########################################################################
### Task / flow portion
########################################################################
# Build the designs
source ./design_settings.tcl
source ./advanced_settings.tcl
source $tclDir/run.tcl

#exit ;#uncomment if running in batch mode
