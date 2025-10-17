import sys
import re
from typing import Dict, Iterable, List, Tuple

# Regex robust to variants: <TYPE>_<SIDE>_X<d+>Y<d+>, e.g., CLBLL_L_X2Y0, BRAM_L_X6Y0, DSP_R_X9Y0
TILE_RE = re.compile(r'^(?P<prefix>[A-Z0-9]+)(?:_[LR])?_X(?P<X>\d+)Y(?P<Y>\d+)$')

# Map concrete prefixes to categories
CATEGORY_MAP = {
    "CLBLL": "CLB",
    "CLBLM": "CLB",
    "CLE":   "CLB",
    "BRAM":  "BRAM",
    "DSP":   "DSP",
}

class ResGraph:
    def __init__(self, device: str = "platform/page/zu9eg.txt"):
        self._points: List[Tuple[int,int,str]] = []  # (x,y,cat)
        self._W = 0
        self._H = 0
        self._psum = {'CLB': None, 'BRAM': None, 'DSP': None}
        tiles = self.load_device(device)
        self.add_many(tiles)
        self.build()

    def parse_tile_name(self, name: str) -> Tuple[int,int,str]:
        m = TILE_RE.match(name.strip())
        if not m:
            return None
        prefix = m.group('prefix')
        x = int(m.group('X'))
        y = int(m.group('Y'))
        # Some families carry suffixes like "CLBLL_L" or "CLBLM_R" already removed by regex group.
        # Also accept long prefixes where category is the start token (e.g., BRAM36, DSP48, etc.).
        cat = CATEGORY_MAP.get(prefix)
        if cat is None:
            # fallback: normalize startswith
            for k,v in CATEGORY_MAP.items():
                if prefix.startswith(k):
                    cat = v
                    break
        if cat is None:
            return None
        return (x,y,cat)

    def add_tile_name(self, name: str):
        parsed = self.parse_tile_name(name)
        if parsed is None:
            return False
        x,y,cat = parsed
        self._points.append((x,y,cat))
        if x+1 > self._W: self._W = x+1
        if y+1 > self._H: self._H = y+1
        return True

    def add_many(self, names: Iterable[str]):
        for n in names:
            self.add_tile_name(n)

    def load_device(self, device: str):
        # vivado tcl command: puts $fp [join [get_tiles] "\n"]
        with open(device, "r") as d:
            return d.readlines()

    def build(self):
        # allocate count grids
        def alloc_matrix(W: int, H: int):
            return [[0]*(H) for _ in range(W)]
        grids = {k:alloc_matrix(self._W, self._H) for k in self._psum.keys()}
        for x,y,cat in self._points:
            grids[cat][x][y] += 1
        # build 2D prefix sums
        for cat,mat in grids.items():
            ps = alloc_matrix(self._W+1, self._H+1) # matrix for psum
            ps[0][0] = mat[0][0]
            for x in range(1, self._W):
                ps[x][0] = ps[x-1][0] + mat[x][0]
            for y in range(1, self._H):
                ps[0][y] = ps[0][y-1] + mat[0][y]
            for x in range(self._W):
                for y in range(self._H):
                    ps[x][y] = ps[x-1][y] + ps[x][y-1] - ps[x-1][y-1] + mat[x][y]
            self._psum[cat] = ps

    def query(self, x0:int, y0:int, x1:int, y1:int) -> Dict[str,int]:
        def rect_sum(ps, x0, y0, x1, y1) -> int:
            # inclusive prefix-sum rectangle query
            def get(x,y) -> int:
                if x<0 or y<0: return 0
                if x>=self._W or y>=self._H: return ps[self._W-1][self._H-1]
                return ps[x][y]
            A = get(x1,y1)
            B = get(x0-1,y1)
            C = get(x1,y0-1)
            D = get(x0-1,y0-1)
            return A - B - C + D
        x0,x1 = sorted((x0,x1))
        y0,y1 = sorted((y0,y1))
        res = {}
        for cat,ps in self._psum.items():
            res[cat] = rect_sum(ps,x0,y0,x1,y1)
        return res

if __name__ == "__main__":
    if len(sys.args) < 2:
        print("Usage: python graph.py <tiles.txt> --query x0 y0 x1 y1")
        raise SystemExit(2)
    elif len(sys.argv) == 7 and sys.argv[2] == '--query':
        rg = ResGraph()
        x0,y0,x1,y1 = map(int, sys.argv[3:7])
        out = rg.query(x0,y0,x1,y1)
        print(f"Query [{x0},{y0}]..[{x1},{y1}] ->", out)
    else:
        print("Error: invalid argvs.")
        raise SystemExit(2)
