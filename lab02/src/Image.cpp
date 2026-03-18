#include "Image.h"

unsigned char Image::clamp(int value) {
    if(value < 0) return 0;
    if(value > 255) return 255;
    return static_cast<unsigned char>(value);
}

Image::Image(int w, int h) : width(w), height(h), data(new Pixel[w * h]) {
    for (int i = 0; i < width * height; i++) {
        data[i] = {0, 0, 0, 0};
    }
}

Image::~Image() {
    delete[] data;
}

Image::Image(const Image& other) : width(other.width), height(other.height), data(new Pixel[other.width * other.height]) {
        for(int i = 0; i < width * height; i++) {
            data[i] = other.data[i];
        }
}

Image& Image::operator=(const Image& other) {
    if(this != &other) {
        delete[] data;
        width = other.width;
        height = other.height;
        data = new Pixel[width * height];
        for (int i = 0; i < width * height; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

Image::Image(Image&& other) noexcept : width(other.width), height(other.height), data(other.data) {
    other.width = 0;
    other.height = 0;
    other.data = nullptr;
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        delete[] data;
        width = other.width;
        height = other.height;
        data = other.data;

        other.width = 0;
        other.height = 0;
        other.data = nullptr;
    }
    return *this;
}

Pixel& Image::operator()(int x, int y) {
    return data[y * width + x];
}

const Pixel& Image::operator()(int x, int y) const {
    return data[y * width + x];
}

bool Image::operator==(const Image& other) const {
    if(width != other.width || height != other.height) return false;
    for(int i = 0; i < width * height; i++) {
        if(!(data[i] == other.data[i])) return false;
    }
    return true;
}

Image Image::operator+(const Image& other) const {
    Image result(*this);
    result += other;
    return result;
}

Image& Image::operator+=(const Image& other) {
    for(int i = 0; i < width * height; i++) {
        data[i].r = clamp(data[i].r + other.data[i].r);
        data[i].g = clamp(data[i].g + other.data[i].g);
        data[i].b = clamp(data[i].b + other.data[i].b);
        data[i].a = clamp(data[i].a + other.data[i].a);
    }
    return *this;
}

Image Image::operator-(const Image& other) const {
    Image result(*this);
    result -= other;
    return result;
}

Image& Image::operator-=(const Image& other) {
    for(int i = 0; i < width * height; i++) {
        data[i].r = clamp(data[i].r - other.data[i].r);
        data[i].g = clamp(data[i].g - other.data[i].g);
        data[i].b = clamp(data[i].b - other.data[i].b);
        data[i].a = clamp(data[i].a - other.data[i].a);
    }
    return *this;
}

Image Image::operator*(double factor) const {
    Image result(*this);
    result *= factor;
    return result;
}

Image& Image::operator*=(double factor) {
    for(int i = 0; i < width * height; i++) {
        data[i].r = clamp(static_cast<int>(data[i].r * factor));
        data[i].g = clamp(static_cast<int>(data[i].g * factor));
        data[i].b = clamp(static_cast<int>(data[i].b * factor));
        data[i].a = clamp(static_cast<int>(data[i].a * factor));
    }
    return *this;
}

Image Image::operator|(const Image& other) const {
    int newWidth = width + other.width;
    int newHeight = std::max(height, other.height);
    Image result(newWidth, newHeight);

    for(int y = 0; y < newHeight; y++) {
        for(int x = 0; x < newWidth; x++) {
            if(x < width && y < height) {
                result(x, y) = (*this)(x, y);
            } 
            else if(x >= width && y < other.height) {
                result(x, y) = other(x - width, y);
            }
        }
    }
    return result;
}

Image& Image::operator|=(const Image& other) {
    *this = *this | other;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Image& img) {
    for(int y = 0; y < img.height; y++) {
        for(int x = 0; x < img.width; x++) {
            os << img(x, y) << " ";
        } 
        os << "\n";
    }
    return os;
}

