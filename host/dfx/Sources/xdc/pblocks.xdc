# pblocks.xdc

{%- for module in modules %}
# pblock_{{module.name}}
create_pblock pblock_{{module.name}}
add_cells_to_pblock [get_pblocks pblock_{{module.name}}] [get_cells -quiet [list {% for inst in module.insts %}{{module.name}}/{{inst}}{% if not loop.last %} {% endif %}{% endfor %}]]
resize_pblock [get_pblocks pblock_{{module.name}}] -add {CLEM_X{{module.partition.x0}}Y{{module.partition.y0}}:CLEM_X{{module.partition.x1}}Y{{module.partition.y1}}}
set_property SNAPPING_MODE ON [get_pblocks pblock_{{module.name}}]
set_property IS_SOFT FALSE [get_pblocks pblock_{{module.name}}]
{%- endfor %}
