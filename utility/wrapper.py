import argparse
import yaml, jinja2, json, os

parser = argparse.ArgumentParser()
parser.add_argument("-t", "--template", required=True, help="The wrapper template VERILOG file path.")
parser.add_argument("-c", "--config", required=True, help="The wrapper config YAML file path.")
args = parser.parse_args()
conf = yaml.safe_load(open(args.config))
tmpl = jinja2.Template(open(args.template).read())

# 1) logic->phy packing
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
in_grp  = pack(conf["streams_in"],  conf["packing"])
out_grp = pack(conf["streams_out"], conf["packing"])

print()
print(in_grp)
print()

# 2) rendering RTL
dst_dir = os.path.dirname(args.config)
dst_filename = os.path.splitext(os.path.basename(args.config))[0]+"_wrapper.v"

open(os.path.join(dst_dir, dst_filename),"w").write(
    tmpl.render(**conf,
                filename=os.path.splitext(dst_filename)[0],
                phy_in=len(in_grp),
                phy_out=len(out_grp),
                in_packing_groups=list(enumerate(in_grp)),
                out_packing_groups=list(enumerate(out_grp))))
