# subdivide_children.tcl
{% for l1_module in modules %}
# Regenerate output files
file delete -force ./Implement/sub_{{l1_module.name}}
file mkdir ./Implement/sub_{{l1_module.name}}
file mkdir ./Implement/sub_{{l1_module.name}}/reports
# Open routed top module
open_checkpoint ./Implement/top_static/top_route_design.dcp
# Subdivide RPs
puts "  #HD: Subdividing RP into second-order counter RPs"
pr_subdivide \
    -cell {{l1_module.insts[0]}} \
    -subcells {{'{'}}{% for l2_module in l1_module.children %}{{l1_module.insts[0]}}/{{l2_module.insts[0]}}{{' ' if not loop.last}}{% endfor %}{{'}'}} \
    ./Synth/{{l1_module.insts[0]}}/{{l1_module.name}}_synth.dcp
write_checkpoint -force ./Implement/sub_{{l1_module.name}}/top_static_{{l1_module.name}}.dcp
puts "	#HD: Completed"
{% endfor %}
