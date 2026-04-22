#pragma once

#include <vector>

class Tensor;

class TensorView {
private:
    int *m_data;
    std::vector<size_t> m_dimension;
    std::vector<size_t> m_step;
public:
    TensorView(int *data, const std::vector<size_t>& dim, const std::vector<size_t>& ste);

    TensorView operator[](size_t index);
    int& operator()(const std::vector<size_t>& index);
    const int& operator()(const std::vector<size_t>& index) const;

    int& operator()(size_t index);
    const int& operator()(size_t index) const;
    
    Tensor asTensor() const;
};

class Tensor {
private:
    std::vector<size_t> m_dimension;
    std::vector<size_t> m_step;
    int *m_data;
    size_t m_size;

    void calculateStep();

public:
    Tensor(const std::vector<size_t>& dim, bool zeroInit = false);

    ~Tensor();
    Tensor(const Tensor& other);
    Tensor& operator=(const Tensor& other);

    TensorView operator[](size_t index);

    int& operator()(const std::vector<size_t>& indices);
    const int& operator()(const std::vector<size_t>& indices) const;

    int& operator()(size_t index);
    const int& operator()(size_t index) const;

    void printDebugInfo() const;

    friend class TensorView;
};