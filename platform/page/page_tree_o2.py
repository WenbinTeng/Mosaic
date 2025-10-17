from page_tree_base import *

class PageTreeO2(PageTreeBase):
    def __init__(self):
        super().__init__()

    def __rect_split(self, rect, demand):
        x_threshold=10
        y_threshold=5
        x0,y0,x1,y1 = rect
        best_score = None
        best_subrect = None
        for i0 in range(x0,x1+1):
            for j0 in range(y0,y1+1):
                for i1 in range(x1,x0,-1):
                    for j1 in range(y1,y0,-1):
                        subrect=Rect(i0,j0,i1,j1)
                        if self.__feasible(subrect, demand):
                            score=self.__bestfit_score
                            if best_score is None or best_score>score:
                                best_score=score
                                best_subrect=subrect
                        else:
                            break
        sx0,sy0,sx1,sy1 = best_subrect
        splited = [
            best_subrect,
            (sx0,sy1+1,x1,y1),
            (sx1+1,y0,x1,sy1),
            (x0,y0,sx1,sy0-1),
            (x0,sy0,sx0-1,y1)
        ]
        return [r for r in splited if r[2]-r[0]>x_threshold and r[3]-r[1]>y_threshold]

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
        rects=self.__rect_split(node.rect, demand)
        for rect in rects:
            n=PageNode(id=self.__alloc_id(),rect=rect,res=self._res_graph.query(*rect),parent=node_id)
            self._nodes[n.id]=n
            node.children.append(n.id)

    def __try_merge(self, node_id):
        merged=False
        def merge(id: int):
            flag=False
            node=self._nodes[id]
            for ci in range(len(node.children)):
                for cj in range(ci+1, len(node.children)):
                    ni=node.children[ci]
                    nj=node.children[cj]
                    node_i=self._nodes[ni]
                    node_j=self._nodes[nj]
                    if node_i.is_free() and node_j.is_free():
                        rect_merged=rect_mergeable(node_i.rect, node_j.rect)
                        if rect_merged is not None:
                            node.children.remove(ni)
                            node.children.remove(nj)
                            del self._nodes[ni]
                            del self._nodes[nj]
                            new_child=PageNode(id=self.__alloc_id(),rect=rect_merged,res=self._res_graph.query(rect_merged))
                            node.children.append(new_child.id)
                            self._nodes[new_child.id]=new_child
                            flag=True
                            break
                if flag:
                    break
            return flag
        while node_id is not None:
            node=self._nodes[node_id]
            if merge(node_id):
                if len(node.children)==1:
                    del self._nodes[node.children[0]]
                    node.children.clear()
                    node_id=node.parent
                merged=True
                continue
            break
        return merged
    