# verify_configurations.tcl

# Verify checkpoints with only the first-order Reconfigurable Partition
puts "	#HD: Comparing top-level static regions"
pr_verify \
{%- for l1_module in modules %}
./Implement/sub_{{l1_module.name}}/top_{{l1_module.children[0].insts[0]}}_recombined.dcp \
{%- endfor %}
-file ./Implement/pr_verify_recombined.log
puts "	#HD: Completed"

# Verify checkpoints with the second-order Reconfigurable Partitions
{%- for l1_module in modules %}
puts "	#HD: Comparing subregions"
pr_verify \
{%- for l2_module in l1_module.children %}
./Implement/sub_{{l2_module.name}}/top_{{l2_inst}}_route_design.dcp \
{%- endfor %}
-file ./Implement/pr_verify_{{l1_module.name}}.log
puts "	#HD: Completed"
{%- endfor %}
