#ifndef __PAGE_TREE_BASE_H__
#define __PAGE_TREE_BASE_H__

#include <iostream>
#include <unordered_map>
#include <vector>

#include "ResImage.hpp"

struct PageNode {
    int id;
    Rect rect;
    ResCount resCount;
    int parentId;
    std::vector<int> childrenId = {};
    bool allocated = false;
    PageNode() {}
    PageNode(int id, const Rect &r, const ResCount &c, int pid)
        : id(id), rect(r), resCount(c), parentId(pid) {}
    bool isLeaf() { return childrenId.empty(); }
    bool isFree() { return isLeaf() && !allocated; }
};

class PageTreeBase {
  protected:
    ResImage _resImage;
    PageNode _root;
    std::unordered_map<int, PageNode *> _nodes;
    int _nextId = 1;
    int allocId();
    double bestfitScore(int nodeId, ResCount &demand);
    int bestfit(int nodeId, ResCount &demand);
    virtual bool shouldSplit(int nodeId, ResCount &demand) = 0;
    virtual std::vector<Rect> rectSplt(int nodeId, ResCount &demand) = 0;
    virtual void split(int nodeId, ResCount &demand) = 0;
    virtual void merge(int nodeId) = 0;

  public:
    PageTreeBase();
    ~PageTreeBase();
    int alloc(ResCount &demand);
    void recycle(int nodeId);
};

#endif