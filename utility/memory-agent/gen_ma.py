import argparse
import jinja2, os

DEBUG=1

params = {
    "aximm": {
        "addr_w": 48,
        "data_w": 128
    },
    "lii_top": 1024
}

tmpl_file = "./utility/memory-agent/memory_agent.v.j2"
output_path = "build"
if not DEBUG:
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", required=True, help="The output path for generated memory agent files.")
    args = parser.parse_args()
    output_path = args.output
    
tmpl = jinja2.Template(open(tmpl_file).read())
dst_dir = output_path + "/memory-agent"
dst_filename = "memory_agent.v"
os.makedirs(dst_dir, exist_ok=True)
open(os.path.join(dst_dir, dst_filename), "w").write(
    tmpl.render(params=params))