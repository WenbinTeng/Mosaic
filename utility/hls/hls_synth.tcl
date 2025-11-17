# hls_synth.tcl

set project_name "app_hls_synth_project"

open_project -reset $project_name
set_top "{{name}}"
{%- for src in source_path %}
add_file "{{src}}"
{%- endfor %}
open_solution -reset "solution1" -flow_target vivado
set_part {xczu9eg-ffvb1156-2-e}
create_clock -period 10 -name default
config_export -format ip_catalog -output {{output_path}} -rtl verilog
csynth_design
export_design -format ip_catalog
close_project
