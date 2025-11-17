# generate_all_bitstreams.tcl

# Create folders for storing full, first-order and second-order bitstreams
file mkdir ./Bitstreams
{%- for l1_module in modules %}
file mkdir ./Bitstreams/{{l1_module.name}}
{%- for l2_module in l1_module.children %}
file mkdir ./Bitstreams/{{l2_module.name}}
{%- endfor %}
{%- endfor %}

# Generate second-order partial bitstreams
{%- for l1_module in modules %}
puts "  #HD: Generating partial bitstreams for {{l1_module.name}} submodules"
open_checkpoint ./Implement/sub_{{l1_module.name}}/top_{{l1_module.children[0].insts[0]}}_route_design.dcp
{%- for l2_module in l1_module.children %}
{%- for l2_inst in l2_module.insts %}
write_bitstream -force -cell {{l1_module.insts[0]}}/{{l2_inst}} ./Bitstreams/{{l2_module.name}}/{{l2_inst}}_partial.bit
{%- endfor %}
puts "	#HD: Completed"
close_project
{%- endfor %}
{%- endfor %}

# Generate first-order partial bitstreams
{%- for l1_module in modules %}
puts "  #HD: Generating partial bitstreams for {{l1_module.name}}"
open_checkpoint ./Implement/sub_{{l1_module.name}}/top_{{l1_module.children[0].insts[0]}}_recombined.dcp
write_bitstream -force -cell {{l1_module.insts[0]}} ./Bitstreams/{{l1_module.name}}/{{l1_module.name}}_{{l1_module.children[0].insts[0]}}_recombined_partial.bit
puts "	#HD: Completed"
close_project
{%- endfor %}

# Generate full bitstream
puts "  #HD: Generating full bitstrean"
open_checkpoint ./Implement/sub_{{modules[0].name}}/top_{{modules[0].children[0].insts[0]}}_route_design.dcp
write_bitstream -force -no_partial_bitfile ./Bitstreams/top_full.bit
puts "	#HD: Completed"
close_project
