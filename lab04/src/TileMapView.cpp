#include "TileMapView.h" 

TileMapView::TileMapView(TileMap& map, int minR, int maxR, int minC, int maxC)
    : _map(map), _minR(minR), _maxR(maxR), _minC(minC), _maxC(maxC) {
    _viewRows = maxR - minR + 1;
    _viewCols = maxC - minC + 1;
}

Tile& TileMapView::operator()(int r, int c) {
    return _map(_minR + r, _minC + c);
}

const Tile& TileMapView::operator()(int r, int c) const {
    return _map(_minR + r, _minC + c);
}

Tile& TileMapView::operator[](int index) {
    return (*this)(index / _viewCols, index % _viewCols);
}

const Tile& TileMapView::operator[](int index) const {
    return (*this)(index / _viewCols, index % _viewCols);
}


TileMapView::Iterator::Iterator(TileMapView& view, int index) 
    : _view(view), _currentIndex(index) {}
    
bool TileMapView::Iterator::operator==(const Iterator& other) const {
    return _currentIndex == other._currentIndex;
}

bool TileMapView::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

TileMapView::Iterator& TileMapView::Iterator::operator++() {
    ++_currentIndex;
    return *this;
}

TileMapView::Iterator& TileMapView::Iterator::operator--() {
    --_currentIndex;
    return *this;
}

Tile& TileMapView::Iterator::operator*() {
    return _view[_currentIndex];
}

TileMapView::Iterator TileMapView::begin() { return Iterator(*this, 0); }
TileMapView::Iterator TileMapView::end() { return Iterator(*this, _viewRows * _viewCols); }



