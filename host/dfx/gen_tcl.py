import argparse
import yaml, jinja2, os, shutil

DEBUG=1

if not DEBUG:
    parser = argparse.ArgumentParser()
    args = parser.parse_args()
    
config_path = "host/dfx/dfx_conf.yaml"
output_path = "build"
tmpl_files = [
    "./host/dfx/synth.tcl",
    "./host/dfx/implement_parent.tcl",
    "./host/dfx/subdivide_children.tcl",
    "./host/dfx/implement_children.tcl",
    "./host/dfx/verify_configurations.tcl",
    "./host/dfx/generate_all_bitstreams.tcl"
]

conf = yaml.safe_load(open(config_path))
for tmpl_file in tmpl_files:
    tmpl = jinja2.Template(open(tmpl_file).read())
    dst_dir = output_path + "/dfx"
    dst_filename = os.path.basename(tmpl_file)
    os.makedirs(dst_dir, exist_ok=True)
    open(os.path.join(dst_dir, dst_filename),"w").write(
        tmpl.render(**conf)
    )

copy_dir = "host/dfx/Tcl_HD"
os.makedirs(os.path.join(dst_dir, "Tcl_HD"), exist_ok=True)
for item in os.listdir(copy_dir):
    item_path = os.path.join(copy_dir, item)
    shutil.copy2(item_path, os.path.join(dst_dir, "Tcl_HD"))
shutil.copy2("host/dfx/advanced_settings.tcl", dst_dir)
shutil.copy2("host/dfx/design_settings.tcl",   dst_dir)
