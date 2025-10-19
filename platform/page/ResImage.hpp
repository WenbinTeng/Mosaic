#ifndef __RES_IMAGE_H__
#define __RES_IMAGE_H__

#include <fstream>
#include <string>
#include <vector>
#include <regex>

using TileInfo = std::tuple<std::string, int, int>;

struct Rect {
    int x0, y0, x1, y1;
    void norm() {
        x0 > x1 ? std::swap(x0, x1) : void(0);
        y0 > y1 ? std::swap(y0, y1) : void(0);
    }
    int getW() { return x1 - x0; }
    int getH() { return y1 - y0; }
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
};

class ResImage {
  private:
    std::string _device;
    int _W = 0;
    int _H = 0;
    std::vector<TileInfo> _tiles;
    std::vector<std::vector<ResCount>> _iiamge;
    void parse_tile_name(const std::string &tile_name);
    void build();

  public:
    ResImage();
    ResImage(const std::string &device);
    ~ResImage();
    ResCount query(Rect r);
};

#endif