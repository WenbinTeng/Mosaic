import argparse
import yaml, jinja2, os

DEBUG=1

tmpl_str_path = "./utility/wrapper/wrapper_t_str.v.j2"
tmpl_mem_path = "./utility/wrapper/wrapper_t_mem.v.j2"
config_path = "app/lenet/config/mem/top_in.yaml"
output_path = "build"
if not DEBUG:
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--config", required=True, help="The wrapper config YAML file path.")
    parser.add_argument("-o", "--output", required=True, help="The output path for generated wrapper files.")
    args = parser.parse_args()
    config_path = args.config
    output_path = args.output
conf = yaml.safe_load(open(config_path))

# 1) logic->phy packing, m->n
def pack(streams, W):
    groups, cur, acc = [], [], 0
    for s in streams:
        if acc + s["width"] > W:
            groups.append(cur)
            cur, acc = [], 0
        cur.append(s)
        acc += s["width"]
    if cur:
        groups.append(cur)
    return groups

# 2) rendering RTL
tmpl = jinja2.Template(open(tmpl_str_path).read())
in_grp  = pack(conf["streams_in"],  conf["packing"]) if "streams_in" in conf else []
out_grp = pack(conf["streams_out"], conf["packing"]) if "streams_out" in conf else []
dst_dir = os.path.join(output_path, os.path.dirname(config_path))
dst_filename = os.path.splitext(os.path.basename(config_path))[0]+"_wrapper.v"
os.makedirs(dst_dir, exist_ok=True)
open(os.path.join(dst_dir, dst_filename),"w").write(
    tmpl.render(**conf,
                module_name=os.path.splitext(dst_filename)[0],
                phy_in=len(in_grp),
                phy_out=len(out_grp),
                in_packing_groups=list(enumerate(in_grp)),
                out_packing_groups=list(enumerate(out_grp))))

if "aximm" in conf:
    dst_mem_filename = os.path.splitext(os.path.basename(config_path))[0]+"_mem_wrapper.v"
    tmpl = jinja2.Template(open(tmpl_mem_path).read())
    open(os.path.join(dst_dir, dst_mem_filename),"w").write(
        tmpl.render(**conf,
                    module_name=os.path.splitext(dst_mem_filename)[0]))
