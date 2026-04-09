#include "TileMap.h"
#include "TileMapView.h"

TileMap::TileMap(int rows, int cols)
    : _rows(rows), _cols(cols), _data(rows * cols) {}

Tile& TileMap::operator()(int r, int c) { return _data[r * _cols + c]; }
const Tile& TileMap::operator()(int r, int c) const {
    return _data[r * _cols + c];
}

Tile& TileMap::operator[](int index) { return _data[index]; }
const Tile& TileMap::operator[](int index) const { return _data[index]; }

TileMapView TileMap::createView() {
    return createView(0, _rows - 1, 0, _cols - 1);
}

TileMapView TileMap::createView(int minR, int maxR, int minC, int maxC) {
    return TileMapView(*this, minR, maxR, minC, maxC);
}