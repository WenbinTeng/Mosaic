#include "PageTreeO2.hpp"

PageTreeO2::PageTreeO2() : PageTreeBase() {}

PageTreeO2::~PageTreeO2() {}

bool PageTreeO2::shouldSplit(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];
    return pnode->resCount.CLB  * 0.5 >= demand.CLB
        && pnode->resCount.DSP  * 0.5 >= demand.DSP
        && pnode->resCount.BRAM * 0.5 >= demand.BRAM;
}

std::vector<Rect> PageTreeO2::rectSplit(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];
    auto [x0, y0, x1, y1] = pnode->rect;
    double bestScore = 100000000.0;
    Rect bestSubRect;

    for (int i0 = x0; i0 <= x1; i0++) {
        for (int j0 = y0; j0 <= y1; j0++) {
            for (int i1 = x1; i1 > i0; i1--) {
                for (int j1 = y1; j1 > j0; j1--) {
                    Rect subRect = {i0, j0, i1, j1};
                    ResCount subResCount = this->_resImage.query(subRect);
                    if (subResCount > demand) {
                        int vid = -1;
                        PageNode *vnode = new PageNode(vid, subRect, subResCount, -1);
                        this->_nodes[vid] = vnode;
                        double score = bestfitScore(vid, demand);
                        if (bestScore > score) {
                            bestScore = score;
                            bestSubRect = subRect;
                        }
                        this->_nodes[vid] = nullptr;
                        delete vnode;
                    } else {
                        break;
                    }
                }
            }
        }
    }

    const int xThreshold = 10;
    const int yThreshold = 5;
    auto [sx0, sy0, sx1, sy1] = bestSubRect;
    std::vector<Rect> ret;
    std::vector<Rect> splited = {
        bestSubRect,
        {sx0, sy1 + 1, x1, y1},
        {sx1 + 1, y0, x1, sy1},
        {x0, y0, sx1, sy0 - 1},
        {x0, sy0, sx0 - 1, y1}
    };

    for (auto &rect : splited) {
        rect.norm();
        if (rect.x1 - rect.x0 > xThreshold && rect.y1 - rect.y0 > yThreshold) {
            ret.push_back(rect);
        }
    }

    return ret;
}

void PageTreeO2::split(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];
    assert(pnode->isFree());
    std::vector<Rect> subregions = rectSplit(nodeId, demand);
    for (Rect region : subregions) {
        PageNode *newNode = new PageNode(this->allocId(), region, this->_resImage.query(region), nodeId);
        this->_nodes[newNode->id] = newNode;
        newNode->parentId = nodeId;
        pnode->childrenId.push_back(newNode->id);
    }
}

void PageTreeO2::merge(int nodeId) {
    auto mergeChild = [&](int id) -> bool {
        bool merged = false;
        PageNode *parent = this->_nodes[id];
        for (int ci = 0; ci < parent->childrenId.size(); ci++) {
            for (int cj = ci + 1; cj < parent->childrenId.size(); cj++) {
                int idI = parent->childrenId[ci];
                int idJ = parent->childrenId[cj];
                PageNode *childI = this->_nodes[idI];
                PageNode *childJ = this->_nodes[idJ];
                if (childI->isFree() && childJ->isFree()) {
                    auto m = childI->rect.tryMerge(childJ->rect);
                    if (m.has_value()) {
                        parent->childrenId.erase(std::find(parent->childrenId.begin(), parent->childrenId.end(), idI));
                        parent->childrenId.erase(std::find(parent->childrenId.begin(), parent->childrenId.end(), idJ));
                        this->_nodes.erase(idI);
                        this->_nodes.erase(idJ);
                        delete childI;
                        delete childJ;
                        PageNode *newChild =
                            new PageNode(
                                this->allocId(),
                                m.value(),
                                this->_resImage.query(m.value()),
                                parent->id);
                        parent->childrenId.push_back(newChild->id);
                        this->_nodes[newChild->id] = newChild;
                        return true;
                    }
                }
            }
        }
        return false;
    };
    while (nodeId != -1) {
        if (mergeChild(nodeId)) {
            PageNode* parent = this->_nodes[nodeId];
            if (parent->childrenId.size() == 1) {
                int childId = parent->childrenId[0];
                PageNode *child = this->_nodes[childId];
                this->_nodes.erase(childId);
                delete child;
                parent->childrenId.clear();
                parent->allocated = false;
                nodeId = parent->parentId;
            }
            continue;
        }
        break;
    }
}