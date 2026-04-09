#pragma once 

#include "TileMap.h"

class TileMapView {
public:
    TileMapView(TileMap& map, int minR, int maxR, int minC, int maxC);

    Tile& operator()(int r, int c);
    const Tile& operator()(int r, int c) const;

    Tile& operator[](int index);
    const Tile& operator[](int index) const;

    class Iterator {
        public:
            Iterator(TileMapView& view, int index);
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;
            Iterator& operator++();
            Iterator& operator--();
            Tile& operator*();
        private:
            TileMapView& _view;
            int _currentIndex;
    };

    Iterator begin();
    Iterator end();

private:
    TileMap& _map;
    int _minR, _maxR, _minC, _maxC;
    int _viewRows, _viewCols;
};