#pragma once

#include <iostream>
#include <algorithm>

struct Pixel { 
    unsigned char r, g, b, a;

    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    friend std::ostream& operator<<(std::ostream& os, const Pixel& p) {
        os << "(" << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ", " << (int)p.a << ")";
        return os;
    }
};

class Image {
public:
    Image(int w, int h);
    ~Image();
    
    // konstruktor kopiujacy
    Image(const Image& other);
    
    // operator przypsania
    Image& operator=(const Image& other);

    // Konstruktor przenoszący
    Image(Image&& other) noexcept;             
    Image& operator=(Image&& other) noexcept;

    Pixel& operator()(int x, int y);
    const Pixel& operator()(int x, int y) const;

    bool operator==(const Image& other) const;

    Image operator+(const Image& other) const;

    Image& operator+=(const Image& other);

    Image operator-(const Image& other) const;

    Image& operator-=(const Image& other);

    Image operator*(double factor) const;

    Image& operator*=(double factor);

    Image operator|(const Image& other) const;
    
    Image& operator|=(const Image& other);

    friend std::ostream& operator<<(std::ostream&, const Image& img);

private:
    int width;
    int height;
    Pixel* data;

    static unsigned char clamp(int value);
};