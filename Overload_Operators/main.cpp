#include <iostream>
#include <stdexcept>

class SafeArray {
private:
    int* data;
    int size;

public:
    SafeArray(int s) : size(s) {
        data = new int[s]{0};
    }
    
    ~SafeArray() {
        delete[] data;
    }

    int getSize() const { return size; }

    // --- OPERATOR PRZYPISANIA ---
    SafeArray& operator=(const SafeArray& other) {
        if(this == &other) {
            return *this;
        }

        delete[] data;

        size = other.getSize();
        data = new int[size];

        for(int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    // --- OPERATOR [] ---
    int& operator[](int index) {
        if(index < 0 || index >= size) {
            throw std::out_of_range("Index out of range!");
        }
        return data[index];
    }
};

class Multiplier {
private:
    double factor;

public:
    Multiplier(double f) : factor(f) {}

    double getFactor() const { return factor; }

    // --- OPERATOR WYWOLANIA FUNKCJI () ---
    double operator()(double value) const {
        return value * factor;
    }
};

class Vector2D {
public:
    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    double fun() const { return x * x + y * y; }
    void print() const { std::cout << "[" << x << ", " << y << "]\n"; }

    // --- STRUMIENIE --- << >>
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
    friend std::istream& operator>>(std::istream& is, Vector2D& v);

    // --- ARYTMETYKA --- + -
    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);

    // --- MNOZENIE PRZEZ SKALAR --- *
    friend Vector2D operator*(const Vector2D& v, double s);
    friend Vector2D operator*(double s, const Vector2D& v);

    // --- RELACYJNE --- == != < > <= >=
    friend bool operator==(const Vector2D& l, const Vector2D& r) {
        return (l.x == r.x) && (l.y == r.y);
    }

    friend bool operator<(const Vector2D& l, const Vector2D& r) {
        return l.fun() < r.fun();
    }

    // --- ZLOZONE PRZYPISANIA --- += -= *=
    Vector2D& operator+=(const Vector2D& r) {
        this->x += r.x;
        this->y += r.y;
        return *this;
    }

    // --- JEDNOARGUMETOWE I INKREMENTACJA --- ! - (-v) ++ --
    Vector2D operator-() const {
        return Vector2D(-x, -y);
    }

    bool operator!() const {
        return (x == 0.0 && y == 0.0);
    }

    // ++v
    Vector2D& operator++() {
        ++x;
        ++y;
        return *this;
    }

    // v++
    Vector2D operator++(int) {
        Vector2D temp(*this);
        ++(*this);
        return temp;
    }

private:
    double x;
    double y;
};

// ======================================================================

// --- STRUMIENIE ---
std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}

std::istream& operator>>(std::istream& is, Vector2D& v) {
    is >> v.x >> v.y;
    return is;
}

// --- ARYTMETYKA ---
Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

// --- MNOZENIE PRZEZ SKALAR ---
Vector2D operator*(const Vector2D& v, double s) {
    return Vector2D(v.x * s, v.y * s); 
}

Vector2D operator*(double s, const Vector2D& v) {
    return Vector2D(v.x * s, v.y * s); 
}

class VectorPointer {
private:
    Vector2D* internalPointer;

public:
    VectorPointer(Vector2D* ptr = nullptr) : internalPointer(ptr) {}
    
    ~VectorPointer() {
        delete internalPointer; 
    }

    Vector2D* getInternalPointer() const { return internalPointer; }

    // --- OPERATOR WYLUSKANIA (*) ---
    Vector2D& operator*() {
        return *internalPointer;
    }

    // --- OPERATOR STRZALKI ---
    Vector2D* operator->() {
        return internalPointer;
    }

};


int main() {

    Vector2D v1(2.0, 3.0);
    Vector2D v2(1.0, 4.0);

    std::cout << "v1: " << v1 << "\n";
    std::cout << "v2: " << v2 << "\n";
    std::cout << "v1 + v2: " << v1 + v2 << "\n";
    std::cout << "v1 * 5: " << v1 * 5.0 << "\n";
    std::cout << "5 * v1: " << 5.0 * v1 << "\n";

    SafeArray arr(5);
    arr[0] = 10;     
    arr[1] = 20;
    std::cout << "Array[0]: " << arr[0] << "\n";

    Multiplier timesFive(5.0);
    std::cout << "10 * 5 = " << timesFive(10.0) << "\n"; 

    VectorPointer ptr(new Vector2D(3.0, 4.0)); 
    
    std::cout << "Vector X (via ->): " << ptr->getX() << "\n";
    
    Vector2D copy = *ptr; 
    copy.print();

    return 0;
}