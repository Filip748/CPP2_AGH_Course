#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class Terrain {
    FIELDS,
    HILLS,
    MOUNTAINS
};

enum class Building {
    NONE, 
    CITY,
    FARM,
    LUMBERMILL,
    MINE
};

struct Tile {
    Terrain terrain = Terrain::FIELDS;
    bool nearRiver = false;
    bool seaAcces = false;
    bool hasForest = false;
    bool hasRoad = false;
    Building building  = Building::NONE;

    int getMoveCost() const {
        if(terrain == Terrain::FIELDS) return 5;
        if (terrain == Terrain::HILLS) return 15;
        if(terrain == Terrain::MOUNTAINS) return 0;
    }

    bool isPassable() const {
        return terrain != Terrain::MOUNTAINS;
    }

    bool operator==(const Tile& other) const {
        return terrain == other.terrain && nearRiver == other.nearRiver &&
            seaAcces == other.seaAcces && hasForest == other.hasForest &&
            hasRoad == other.hasRoad && building == other.building;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tile& tile) {
        os << "[T:" << (int)tile.terrain << "R:"<< tile.nearRiver << "F:" << tile.hasForest << "]";
        return os;
    }

};

class TileMapView;

class TileMap {
public:
    TileMap(int rows, int cols);

    Tile& operator()(int r, int c);
    const Tile& operator()(int r, int c) const;

    Tile& operator[](int index);
    const Tile& operator[](int index) const;

    TileMapView createView();
    TileMapView createView(int minR, int maxR, int minC, int maxC);

    int getRows() const { return _rows; }
    int getCols() const { return _cols; }

private:
    int _rows;
    int _cols;
    std::vector<Tile> _data;
};