#include "ResImage.hpp"

ResImage::ResImage() : ResImage("zu9eg.txt") {}

ResImage::ResImage(const std::string &device) : _device(device) {
    std::ifstream ifs;
    ifs.open(device, std::ios::in);
    std::string tile_name;
    while (std::getline(ifs, tile_name)) {
        parse_tile_name(tile_name);
    }
    ifs.close();
    build();
}

ResImage::~ResImage() {}

void ResImage::parse_tile_name(const std::string &tile_name) {
    static const std::regex pattern(R"(^([A-Z0-9]+)(?:_[LR])?_X([0-9]+)Y([0-9]+)$)");
    std::smatch m;
    if (!std::regex_match(tile_name, m, pattern))
        return;

    static std::unordered_map<std::string, std::string> category = {
        {"CLEL", "CLB"}, {"CLER", "CLB"}, {"DSP", "DSP"}, {"BRAM", "BRAM"}};

    std::string prefix = m[1];
    int x = std::stoi(m[2]);
    int y = std::stoi(m[3]);

    if (category.count(prefix)) {
        this->_W = std::max(this->_W, x);
        this->_H = std::max(this->_H, y);
        _tiles.push_back({category[prefix], x, y});
    }
}

void ResImage::build() {
    std::vector<std::vector<ResCount>> deviceMap(_W + 1, std::vector<ResCount>(_H + 1, {0, 0, 0}));
    for (auto &tile : _tiles) {
        auto [cat, x, y] = tile;
        ResCount rc = cat == "CLB"  ? ResCount({1, 0, 0})
                    : cat == "DSP"  ? ResCount({0, 1, 0})
                    : cat == "BRAM" ? ResCount({0, 0, 1})
                                    : ResCount({0, 0, 0});
        deviceMap[x][y] += rc;
    }

    _iiamge.assign(_W + 1, std::vector<ResCount>(_H + 1));
    _iiamge[0][0] = deviceMap[0][0];
    for (int i = 1; i <= _W; i++) {
        _iiamge[i][0] = _iiamge[i - 1][0] + deviceMap[i][0];
    }
    for (int j = 1; j <= _H; j++) {
        _iiamge[0][j] = _iiamge[0][j - 1] + deviceMap[0][j];
    }
    for (int i = 1; i <= _W; i++) {
        for (int j = 1; j <= _H; j++) {
            _iiamge[i][j] =
                _iiamge[i - 1][j] + _iiamge[i][j - 1] - _iiamge[i - 1][j - 1] + deviceMap[i][j];
        }
    }
}

ResCount ResImage::query(Rect r) {
    r.norm();
    auto get = [&](int x, int y) -> ResCount {
        return x < 0 || y < 0                 ? ResCount({0, 0, 0})
             : x > this->_W || y > this->_H   ? _iiamge[this->_W][this->_H]
                                              : _iiamge[x][y];
    };
    auto A = get(r.x1, r.y1);
    auto B = get(r.x0 - 1, r.y1);
    auto C = get(r.x1, r.y0 - 1);
    auto D = get(r.x0 - 1, r.y0 - 1);
    return A - B - C + D;
}