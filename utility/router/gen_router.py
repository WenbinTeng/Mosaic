import argparse
import jinja2, os

DEBUG=1

tmpl_path = "./utility/router/lii_router.v.j2"
output_path = "build"
if not DEBUG:
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", required=True, help="The output path for generated router files.")
    args = parser.parse_args()
    output_path = args.output

params = {
    'N_IN': 4,
    'N_OUT': 2,
    'DW': 256,
    'SRC_W': 8,
    'DST_W': 8,
    'TYPE_W': 2,
    'ROUTE_MSB': 7,
    'FIFO_DEPTH': 2,
    "TYPE_PRIO": 2
}

tmpl = jinja2.Template(open(tmpl_path).read())
dst_dir = output_path + "/router"
dst_filename = "lii_router.v"
os.makedirs(dst_dir, exist_ok=True)
open(os.path.join(dst_dir, dst_filename),"w").write(
    tmpl.render(params=params, 
                module_name="lii_router"))