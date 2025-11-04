#include "PageTreeO1.hpp"

PageTreeO1::PageTreeO1() : PageTreeBase() {}

PageTreeO1::~PageTreeO1() {}

bool PageTreeO1::shouldSplit(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];
    return pnode->resCount.CLB  * 0.5 >= demand.CLB
        && pnode->resCount.DSP  * 0.5 >= demand.DSP
        && pnode->resCount.BRAM * 0.5 >= demand.BRAM;
}

std::vector<Rect> PageTreeO1::rectSplit(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];
    auto [x0, y0, x1, y1] = pnode->rect;
    int w = pnode->rect.getW();
    int h = pnode->rect.getH();
    if (w >= h) {
        int mid = x0 + (w / 2) - 1;
        return {{x0, y0, mid, y1}, {mid + 1, y0, x1, y1}};
    } else {
        int mid = y0 + (h / 2) + 1;
        return {{x0, y0, x1, mid}, {x0, mid + 1, x1, y1}};
    }
}

void PageTreeO1::split(int nodeId, ResCount &demand) {
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

void PageTreeO1::merge(int nodeId) {
    while (nodeId != -1) {
        PageNode *pnode = this->_nodes[nodeId];
        assert(pnode->childrenId.empty() || pnode->childrenId.size() == 2);
        if (pnode->childrenId.size() == 2) {
            PageNode* a = this->_nodes[pnode->childrenId[0]];
            PageNode* b = this->_nodes[pnode->childrenId[1]];
            if (a->isFree() && b->isFree()) {
                auto m = a->rect.tryMerge(b->rect);
                if (m.has_value()) {
                    delete a;
                    delete b;
                    pnode->childrenId.clear();
                    nodeId = pnode->parentId;
                    continue;
                }
            }
        }
        break;
    }
}