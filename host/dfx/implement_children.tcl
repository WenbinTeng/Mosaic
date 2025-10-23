# implement_children.tcl

source ./design_settings.tcl
{% for l1_module in modules %}


set loc ./Implement/sub_{{l1_module.name}}
puts "#HD: Running implementation sub_{{l1_module.name}}"
{% for l2_module in l1_module.children %}
# Ceate in memory project
create_project -in_memory -part $part > $loc/create_project.log
# Load synthesized top module
add_file $loc/top_static_{{l1_module.name}}.dcp
# Load synthesized level-2 modules
add_file ./Synth/{{l2_module.insts[0]}}/{{l2_module.name}}_synth.dcp
set_property SCOPED_TO_CELLS {{'{'}}{% for inst in l2_module.insts %}{{l1_module.insts[0]}}/{{inst}}{{' ' if not loop.last}}{% endfor %}{{'}'}} [get_files ./Synth/{{l2_module.insts[0]}}/{{l2_module.name}}_synth.dcp]
# Load level-2 pblocks
add_file ./Sources/xdc/{{l2_module.name}}_pblock.xdc
set_property USED_IN {implementation} [get_files ./Sources/xdc/{{l2_module.name}}_pblock.xdc]
set_property PROCESSING_ORDER LATE [get_files ./Sources/xdc/{{l2_module.name}}_pblock.xdc]
# Link design
link_design -mode default -reconfig_partitions {{'{'}}{% for inst in l2_module.insts %}{{l1_module.insts[0]}}/{{inst}}{{' ' if not loop.last}}{% endfor %}{{'}'}} -part $part -top top
write_checkpoint -force $loc/top_{{l2_module.name}}_link_design.dcp > $loc/write_checkpoint.log
# Optimize design
opt_design > $loc/top_{{l2_module.name}}_opt_design.log
puts "	#HD: Completed: opt_design"
write_checkpoint -force $loc/top_{{l2_module.name}}_opt_design.dcp > $loc/write_checkpoint.log
# Place design
place_design > $loc/top_{{l2_module.name}}_place_design.log
puts "	#HD: Completed: place_design"
write_checkpoint -force $loc/top_{{l2_module.name}}_place_design.dcp > $loc/write_checkpoint.log
# Phy optimize design
phys_opt_design > $loc/top_{{l2_module.name}}_phys_opt_design.log
puts "	#HD: Completed: phys_opt_design"
write_checkpoint -force $loc/top_{{l2_module.name}}_phys_opt_design.dcp > $loc/write_checkpoint.log
# Route design
route_design > $loc/top_{{l2_module.name}}_route_design.log
puts "	#HD: Completed: route_design"
write_checkpoint -force $loc/top_{{l2_module.name}}_route_design.dcp > $loc/write_checkpoint.log
{%- for inst in l2_module.insts %}
write_checkpoint -force -cell {{l1_module.insts[0]}}/{{inst}} $loc/{{inst}}_RM.dcp
{%- endfor %}
report_utilization -file $loc/reports/top_{{l2_module.name}}_utilization_route_design.rpt > $loc/temp.log
report_route_status -file $loc/reports/top_{{l2_module.name}}_route_status.rpt > $loc/temp.log
report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10 -input_pins -file $loc/reports/top_{{l2_module.name}}_timing_summary.rpt > $loc/temp.log
debug::report_design_status > $loc/reports/top_{{l2_module.name}}_design_status.rpt
report_drc -ruledeck bitstream_checks -name top -file $loc/reports/top_{{l2_module.name}}_drc_bitstream_checks.rpt > $loc/reports/temp.log
{% endfor %}
open_checkpoint ./Implement/sub_{{l1_module.name}}/top_{{l1_module.children[0].insts[0]}}_route_design.dcp
pr_recombine -cell {{l1_module.insts[0]}}
write_checkpoint ./Implement/sub_{{l1_module.name}}/top_{{l1_module.children[0].insts[0]}}_recombined.dcp
puts "	#HD: Completed"
{% endfor %}
