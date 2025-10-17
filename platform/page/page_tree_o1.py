from page_tree_base import *

class PageTreeO1(PageTreeBase):
    def __init__(self):
        super().__init__()
    
    def __rect_split(self, rect, demand):
        x0,y0,x1,y1 = rect
        w,h = rect_w(rect),rect_h(rect)
        if w >= h:
            mid=x0+(w//2)-1
            return (x0,y0,mid,y1),(mid+1,y0,x1,y1)
        else:
            mid=y0+(h//2)-1
            return (x0,y0,x1,mid),(x0,mid+1,x1,y1)
        
    def __should_splt(self, node_id, demand):
        node=self._nodes[node_id]
        return (
                node.res["CLB"]  * 0.4 >= demand["CLB"]
            and node.res["BRAM"] * 0.5 >= demand["BRAM"]
            and node.res["DSP"]  * 0.5 >= demand["DSP"]
        )
    
    def __split(self, node_id, demand):
        node=self._nodes[node_id]
        assert node.is_free()
        r1,r2=self.__rect_split(node.rect)
        a=PageNode(id=self.__alloc_id(), rect=r1, res=self._res_graph.query(*r1), parent=node_id)
        b=PageNode(id=self.__alloc_id(), rect=r2, res=self._res_graph.query(*r2), parent=node_id)
        self._nodes[a.id]=a
        self._nodes[b.id]=b
        node.children=[a.id,b.id]
    
    def __try_merge(self, node_id: int) -> bool:
        merged=False
        while node_id is not None:
            node=self._nodes[node_id]
            if len(node.children)==2:
                a=self._nodes[node.children[0]]
                b=self._nodes[node.children[1]]
                if a.is_free() and b.is_free():
                    merged_rect=rect_mergeable(a.rect,b.rect)
                    if merged_rect==node.rect:
                        del self._nodes[a.id]
                        del self._nodes[b.id]
                        node.children.clear()
                        node_id=node.parent
                        merged=True
                        continue
            break
        return merged
    