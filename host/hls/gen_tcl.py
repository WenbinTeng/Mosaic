import argparse
import yaml, jinja2, os

DEBUG = 1

if not DEBUG:
    parser = argparse.ArgumentParser()
    args = parser.parse_args()
    
config_path = "host/hls/hls_conf.yaml"
output_path = "build"
tmpl_file = "./host/hls/hls_synth.tcl"

conf = yaml.safe_load(open(config_path))
tmpl = jinja2.Template(open(tmpl_file).read())
dst_dir = output_path + "/hls"
dst_files = []
os.makedirs(dst_dir, exist_ok=True)
for func in conf["functions"]:
    dst_filename = "hls_synth_" + func["name"] + ".tcl"
    dst_files.append(dst_filename)
    open(os.path.join(dst_dir, dst_filename),"w").write(
        tmpl.render(**func, output_path=".")
    )

with open(os.path.join(dst_dir, "run_all.tcl"),"w") as f:
    for file in dst_files:
        f.write("source " + file + "\n")
        