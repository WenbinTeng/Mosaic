from abc import abstractmethod
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

from res_graph import ResGraph

Rect = Tuple[int,int,int,int]
def rect_w(r): return r[2]-r[0]+1
def rect_h(r): return r[3]-r[1]+1
def rect_area(r): return rect_w(r)*rect_h(r)
def rect_mergeable(r1,r2):
    ax0,ay0,ax1,ay1 = r1; bx0,by0,bx1,by1 = r2
    if ay0==by0 and ay1==by1 and ax1+1==bx0: return (ax0,ay0,bx1,ay1) # down-top
    if by0==ay0 and by1==ay1 and bx1+1==ax0: return (bx0,by0,ax1,ay1) # top-down
    if ax0==bx0 and ax1==bx1 and ay1+1==by0: return (ax0,ay0,ax1,by1) # left-right
    if bx0==ax0 and bx1==ax1 and by1+1==ay0: return (bx0,by0,ax1,ay1) # right-left
    return None

@dataclass
class PageNode:
    id: int
    rect: Rect
    res: Dict[str,int]
    parent: Optional[int]=None
    children: List[int]=field(default_factory=list)
    allocated: bool=False
    def is_leaf(self): return len(self.children)==0
    def is_free(self): return self.is_leaf() and not self.allocated

class PageTreeBase:
    def __init__(self):
        self._res_graph=ResGraph()
        self._root=PageNode(id=0,
                            rect=(0,0,self._res_graph._W,self._res_graph._H),
                            res=self._res_graph.query(0,0,self._res_graph._W,self._res_graph._H),
                            parent=None,
                            children=[],
                            allocated=False)
        self._nodes: Dict[int,PageNode]={0:self._root}
        self._next_id=1

    def __alloc_id(self):
        nid=self._next_id
        self._next_id+=1
        return nid

    def __feasible(self, rect: Rect, demand: Dict[str,int]) -> bool:
        res=self._res_graph.query(*rect)
        return all(res.get(k,0)>=demand.get(k,0) for k in ('CLB','BRAM','DSP'))

    def __feasible(self, node_id: int, demand: Dict[str,int]) -> bool:
        node=self._nodes[node_id]
        return all(node.res.get(k,0)>=demand.get(k,0) for k in ('CLB','BRAM','DSP'))

    def __bestfit_score(self, node_id: int, demand: Dict[str,int]) -> int:
        slack=0
        weight={"CLB":0.1,"BRAM":1,"DSP":1}
        node=self._nodes[node_id]
        for r,c in demand.items():
            if int(node.res.get(r,0)) < c:
                return 10**12
            else:
                slack+=(int(node.res.get(r,0))-c)*weight.get(r,1)
        return slack*1000 + rect_area(node.rect)

    def __bestfit(self, node_id: int, demand: Dict[str,int]) -> int:
        node=self._nodes[node_id]
        if node.is_leaf():
            return node_id if node.is_free() else None
        best_score=None
        best_candidate=None
        for cid in node.children:
            candidate=self.__bestfit(cid, demand)
            if candidate is not None:
                score=self.__bestfit_score(cid, demand)
                if best_score is None or best_score>score:
                    best_score=score
                    best_candidate=candidate
        return best_candidate

    @abstractmethod
    def __rect_split(self, rect: Rect, demand: Dict[str,int]) -> List[Rect]:
        pass
    
    @abstractmethod
    def __should_splt(self, node_id: int, demand: Dict[str,int]) -> bool:
        pass

    @abstractmethod
    def __split(self, node_id: int, demand: Dict[str,int]):
        pass

    def alloc(self, demand: Dict[str,int]) -> int:
        candidate=self.__bestfit(self._root, demand)
        if candidate is None:
            raise ValueError("no_sufficient_resource")
        if self.__should_splt(candidate, demand):
            self.__split(candidate)
            candidate=self.alloc(demand)
        return candidate

    @abstractmethod
    def __try_merge(self, node_id: int) -> bool:
        pass

    def recycle(self, node_id: int):
        node=self._nodes[node_id]
        node.allocated=False
        self.__try_merge(node)

    def status(self):
        total=len(self.nodes)
        alloc=sum(1 for n in self.nodes.values() if n.allocated)
        leaves=sum(1 for n in self.nodes.values() if n.is_leaf())
        free_leaves=sum(1 for n in self.nodes.values() if n.is_free())
        print({'nodes':total,'leaves':leaves,'allocated':alloc,'free_leaves':free_leaves})
