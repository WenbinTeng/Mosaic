#ifndef __PAGE_TREE_O1_H__
#define __PAGE_TREE_O1_H__

#include <cassert>

#include "PageTreeBase.hpp"

class PageTreeO1 : public PageTreeBase {
  protected:
    bool shouldSplit(int nodeId, ResCount &demand) override;
    std::vector<Rect> rectSplt(int nodeId, ResCount &demand) override;
    void split(int nodeId, ResCount &demand) override;
    void merge(int nodeId) override;

  public:
    PageTreeO1();
    ~PageTreeO1();
};

#endif