# pblocks.xdc

# pblock_{{name}}
create_pblock pblock_{{name}}
add_cells_to_pblock [get_pblocks pblock_{{name}}] [get_cells -quiet [list {% for inst in insts %}{{name}}/{{inst}}{{' ' if not loop.last}}{% endfor %}]]
resize_pblock [get_pblocks pblock_{{name}}] -add {CLEM_X{{partition.x0}}Y{{partition.y0}}:CLEM_X{{partition.x1}}Y{{partition.y1}}}
set_property SNAPPING_MODE ON [get_pblocks pblock_{{name}}]
set_property IS_SOFT FALSE [get_pblocks pblock_{{name}}]
