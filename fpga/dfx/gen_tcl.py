import argparse
import yaml, jinja2, os, shutil

DEBUG = 1

if not DEBUG:
    parser = argparse.ArgumentParser()
    args = parser.parse_args()

config_path = "fpga/dfx/dfx_conf.yaml"
output_path = "build"
tmpl_files = [
    "./fpga/dfx/synth.tcl",
    "./fpga/dfx/implement_parent.tcl",
    "./fpga/dfx/subdivide_children.tcl",
    "./fpga/dfx/implement_children.tcl",
    "./fpga/dfx/verify_configurations.tcl",
    "./fpga/dfx/generate_all_bitstreams.tcl",
]

conf = yaml.safe_load(open(config_path))
for tmpl_file in tmpl_files:
    tmpl = jinja2.Template(open(tmpl_file).read())
    dst_dir = output_path + "/dfx"
    dst_filename = os.path.basename(tmpl_file)
    os.makedirs(dst_dir, exist_ok=True)
    open(os.path.join(dst_dir, dst_filename), "w").write(
        tmpl.render(**conf)
    )

util_files_dir = "fpga/dfx/Tcl_HD"
util_dst_dir = os.path.join(dst_dir, "Tcl_HD")
os.makedirs(util_dst_dir, exist_ok=True)
for item in os.listdir(util_files_dir):
    item_path = os.path.join(util_files_dir, item)
    shutil.copy2(item_path, util_dst_dir)
shutil.copy2("fpga/dfx/advanced_settings.tcl", dst_dir)
shutil.copy2("fpga/dfx/design_settings.tcl", dst_dir)

pblock_tmpl_file = "fpga/dfx/Sources/xdc/pblocks.xdc"
pblock_dst_dir = os.path.join(dst_dir, "Sources/xdc")
os.makedirs(pblock_dst_dir, exist_ok=True)
for l1_module in conf["modules"]:
    l1_tmpl = jinja2.Template(open(pblock_tmpl_file).read())
    l1_dst_filename = l1_module["name"] + "_pblock.xdc"
    open(os.path.join(pblock_dst_dir, l1_dst_filename), "w").write(
        l1_tmpl.render(**l1_module)
    )
    for l2_module in l1_module["children"]:
        l2_tmpl = jinja2.Template(open(pblock_tmpl_file).read())
        l2_dst_filename = l2_module["name"] + "_pblock.xdc"
        open(os.path.join(pblock_dst_dir, l2_dst_filename), "w").write(
            l2_tmpl.render(**l2_module)
        )
