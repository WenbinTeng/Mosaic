# implement_parent.tcl

source ./design_settings.tcl
set loc "./Implement/top_static"

file mkdir ./Implement
file delete -force $loc
file mkdir $loc
file mkdir $loc/reports
puts "#HD: Running implementation top_static"
create_project -in_memory -part $part > $loc/create_project.log

# Synthesize top module
add_file ./Synth/Static/top_synth.dcp
add_file ./Sources/xdc/board.xdc
set_property USED_IN {implementation} [get_files ./Sources/xdc/board.xdc]
# Synthesize sub module
{%- for l1_module in modules %}
# -- Load level-1 pblocks
add_file ./Sources/xdc/{{l1_module.name}}_pblock.xdc
set_property USED_IN {implementation} [get_files ./Sources/xdc/{{l1_module.name}}_pblock.xdc]
# -- Load synthesized level-1 modules
add_file ./Synth/{{l1_module.insts[0]}}/{{l1_module.name}}_synth.dcp
set_property SCOPED_TO_CELLS {{'{'}}{{l1_module.insts[0]}}{{'}'}} [get_files ./Synth/{{l1_module.insts[0]}}/{{l1_module.name}}_synth.dcp]
{%- for l2_module in l1_module.children %}
# -- Load synthesized level-2 modules
add_file ./Synth/{{l2_module.insts[0]}}/{{l2_module.name}}_synth.dcp
set_property SCOPED_TO_CELLS {{'{'}}{% for inst in l2_module.insts %}{{l1_module.insts[0]}}/{{inst}}{{' ' if not loop.last}}{% endfor %}{{'}'}} [get_files ./Synth/{{l2_module.insts[0]}}/{{l2_module.name}}_synth.dcp]
{%- endfor %}
{%- endfor %}
# Link design
link_design -mode default -reconfig_partitions {{'{'}}{% for l1_module in modules %}{{l1_module.insts[0]}}{{' ' if not loop.last}}{% endfor %}{{'}'}} -part $part -top top > $loc/top_link_design.log
write_checkpoint -force $loc/top_link_design.dcp > $loc/write_checkpoint.log
# Optimize design
opt_design > $loc/top_opt_design.log
puts "	#HD: Completed: opt_design"
write_checkpoint -force $loc/top_opt_design.dcp > $loc/write_checkpoint.log
# Place design
place_design > $loc/top_place_design.log
puts "	#HD: Completed: place_design"
write_checkpoint -force $loc/top_place_design.dcp > $loc/write_checkpoint.log
# Phy optimize design
phys_opt_design > $loc/top_phys_opt_design.log
puts "	#HD: Completed: phys_opt_design"
write_checkpoint -force $loc/top_phys_opt_design.dcp > $loc/write_checkpoint.log
# Route design
route_design > $loc/top_route_design.log
puts "	#HD: Completed: route_design"
write_checkpoint -force $loc/top_route_design.dcp > $loc/write_checkpoint.log
# Report
report_utilization -file $loc/reports/top_utilization_route_design.rpt > $loc/temp.log
report_route_status -file $loc/reports/top_route_status.rpt > $loc/temp.log
report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10 -input_pins -file $loc/reports/top_timing_summary.rpt > $loc/temp.log
debug::report_design_status > $loc/reports/top_design_status.rpt
report_drc -ruledeck bitstream_checks -name top -file $loc/reports/top_drc_bitstream_checks.rpt > $loc/reports/temp.log
puts "	#HD: Completed"
close_project
