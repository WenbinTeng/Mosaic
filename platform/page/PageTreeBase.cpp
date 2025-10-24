#include "PageTreeBase.hpp"

PageTreeBase::PageTreeBase() {
    this->_resImage = ResImage();
    this->_root = PageNode(this->allocId(), {0, 0, this->_resImage.getW(), this->_resImage.getH()},
                           this->_resImage.getRes(), -1);
    this->_nodes[this->_root.id] = &(this->_root);
}

PageTreeBase::~PageTreeBase() {}

int PageTreeBase::allocId() {
    return this->_nextId++;
}

double PageTreeBase::bestfitScore(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];

    if (pnode->resCount > demand) {
        double slack = 0;
        slack += (pnode->resCount.CLB  - demand.CLB)  * 0.1;
        slack += (pnode->resCount.DSP  - demand.DSP)  * 1;
        slack += (pnode->resCount.BRAM - demand.BRAM) * 1;
        return slack * 1000.0 + pnode->rect.getArea();
    }

    return 100000000.0;
}

int PageTreeBase::bestfit(int nodeId, ResCount &demand) {
    PageNode *pnode = this->_nodes[nodeId];

    if (pnode->isLeaf()) {
        return pnode->isFree() ? pnode->id : -1;
    }

    const double MAX_SCORE = 100000000.0;
    double bestScore = MAX_SCORE;
    int bestCandidate = -1;

    for (int cid : pnode->childrenId) {
        int candidate = bestfit(cid, demand);
        if (candidate != -1) {
            double score = bestfitScore(candidate, demand);
            if (bestCandidate == -1 || bestScore > score) {
                bestScore = score;
                bestCandidate = candidate;
            }
        }
    }

    return bestCandidate;
}

int PageTreeBase::alloc(ResCount &demand) {
    int candidate = bestfit(this->_root.id, demand);

    if (candidate == -1) {
        std::cout << "  [ERROR]: Unable to alloc - no sparse resource." << std::endl;
        return -1;
    }
    while (shouldSplit(candidate, demand)) {
        split(candidate, demand);
        candidate = bestfit(candidate, demand);
    }

    this->_nodes[candidate]->allocated = true;
    return candidate;
}

void PageTreeBase::recycle(int nodeId) {
    if (nodeId == -1)
        return;
    PageNode *pnode = this->_nodes[nodeId];
    pnode->allocated = false;
    merge(nodeId);
    return;
}