import argparse
import jinja2, os

DEBUG=1

params = {
    "regions": [
        {
            "virt_base": "64\'h0000_0000_0000_0000",
            "phys_base": "64\'h0000_0000_8000_0000",
            "size":      "64\'h0000_0000_1000_0000"
        },
        {
            "virt_base": "64\'h0000_0001_0000_0000",
            "phys_base": "64\'h0000_0001_8000_0000",
            "size":      "64\'h0000_0000_1000_0000"
        },
        {
            "virt_base": "64\'h0000_0002_0000_0000",
            "phys_base": "64\'h0000_0002_8000_0000",
            "size":      "64\'h0000_0000_1000_0000"
        },
        {
            "virt_base": "64\'h0000_0003_0000_0000",
            "phys_base": "64\'h0000_0003_8000_0000",
            "size":      "64\'h0000_0000_1000_0000"
        }
    ],
    "aximm": {
        "addr_w": 48,
        "data_w": 128
    }
}

tmpl_file = "./utility/memory-management-unit/mmu_axi.v.j2"
output_path = "build"
if not DEBUG:
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", required=True, help="The output path for generated memory management unit files.")
    args = parser.parse_args()
    output_path = args.output
    
tmpl = jinja2.Template(open(tmpl_file).read())
dst_dir = output_path + "/memory-management-unit"
dst_filename = "mmu_axi.v"
os.makedirs(dst_dir, exist_ok=True)
open(os.path.join(dst_dir, dst_filename), "w").write(
    tmpl.render(params=params))