#ifndef __PAGE_TREE_O2_H__
#define __PAGE_TREE_O2_H__

#include <cassert>

#include "PageTreeBase.hpp"

class PageTreeO2 : public PageTreeBase {
  protected:
    bool shouldSplit(int nodeId, ResCount &demand) override;
    std::vector<Rect> rectSplit(int nodeId, ResCount &demand) override;
    void split(int nodeId, ResCount &demand) override;
    void merge(int nodeId) override;

  public:
    PageTreeO2();
    ~PageTreeO2();
};

#endif