#ifndef __RES_IMAGE_H__
#define __RES_IMAGE_H__

#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <optional>

using TileInfo = std::tuple<std::string, int, int>;

struct Rect {
    int x0, y0, x1, y1;
    void norm() {
        x0 > x1 ? std::swap(x0, x1) : void(0);
        y0 > y1 ? std::swap(y0, y1) : void(0);
    }
    int getW() { return x1 - x0; }
    int getH() { return y1 - y0; }
    int getArea() { return getW() * getH(); }
    std::optional<Rect> tryMerge(const Rect &r) {
        auto [bx0, by0, bx1, by1] = r;
        if (y0 == by0 && y1 == by1 && x1 + 1 == bx0)
            return Rect({x0, y0, bx1, by1});
        if (by0 == y0 && by1 == y1 && bx1 + 1 == x0)
            return Rect({bx0, by0, x1, y1});
        if (x0 == bx0 && x1 == bx1 && y1 + 1 == by0)
            return Rect({x0, y0, bx1, by1});
        if (bx0 == x0 && bx1 == x1 && by1 + 1 == y0)
            return Rect({bx0, by0, x1, y1});
        return std::nullopt;
    }
};

struct ResCount {
    int CLB = 0, DSP = 0, BRAM = 0;
    void operator+=(const ResCount &rhs) {
        this->CLB += rhs.CLB;
        this->DSP += rhs.DSP;
        this->BRAM += rhs.BRAM;
    }
    ResCount operator+(const ResCount &rhs) {
        return ResCount({this->CLB + rhs.CLB, this->DSP + rhs.DSP, this->BRAM + rhs.BRAM});
    }
    ResCount operator-(const ResCount &rhs) {
        return ResCount({this->CLB - rhs.CLB, this->DSP - rhs.DSP, this->BRAM - rhs.BRAM});
    }
    bool operator>(const ResCount &rhs) {
        return this->CLB > rhs.CLB && this->DSP > rhs.DSP && this->BRAM > rhs.BRAM;
    }
};

class ResImage {
  private:
    std::string _device;
    ResCount _deviceRes;
    int _W = 0;
    int _H = 0;
    std::vector<TileInfo> _tiles;
    std::vector<std::vector<ResCount>> _iiamge;
    void parseTileName(const std::string &tile_name);
    void build();

  public:
    ResImage();
    ResImage(const std::string &device);
    ~ResImage();
    ResCount query(Rect r);
    int getW() { return this->_W; }
    int getH() { return this->_H; }
    ResCount getRes() { return this->_deviceRes; }
};

#endif