import argparse
import yaml, jinja2, json, os

DEBUG=0

tmpl_str_path = "./utility/wrapper_t_str.v.j2"
tmpl_mem_path = "./utility/wrapper_t_mem.v.j2"
conf_path = "./app/lenet/config/mem/top_in.yaml"
wrap_type = 1
if not DEBUG:
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--conf", required=True, help="The wrapper config YAML file path.")
    parser.add_argument("-t", "--type", required=True, help="The wrapper type, 0-stream, 1-mem")
    args = parser.parse_args()
    conf_path = args.conf
    wrap_type = args.type
conf = yaml.safe_load(open(conf_path))

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
in_grp  = pack(conf["streams_in"],  conf["packing"]) if "streams_in" in conf else []
out_grp = pack(conf["streams_out"], conf["packing"]) if "streams_out" in conf else []
dst_dir = os.path.dirname(conf_path)
dst_filename = os.path.splitext(os.path.basename(conf_path))[0]+"_wrapper.v"
tmpl = jinja2.Template(open(tmpl_str_path).read())
open(os.path.join(dst_dir, dst_filename),"w").write(
    tmpl.render(**conf,
                module_name=os.path.splitext(dst_filename)[0],
                phy_in=len(in_grp),
                phy_out=len(out_grp),
                in_packing_groups=list(enumerate(in_grp)),
                out_packing_groups=list(enumerate(out_grp))))

if int(wrap_type) == 1:
    dst_mem_filename = os.path.splitext(os.path.basename(conf_path))[0]+"_mem_wrapper.v"
    tmpl = jinja2.Template(open(tmpl_mem_path).read())
    open(os.path.join(dst_dir, dst_mem_filename),"w").write(
        tmpl.render(**conf,
                    module_name=os.path.splitext(dst_mem_filename)[0]))
