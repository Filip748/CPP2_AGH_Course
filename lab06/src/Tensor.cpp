#include "Tensor.h"
#include <iostream>

TensorView::TensorView(int *data, const std::vector<size_t>& dim, const std::vector<size_t>& ste)
    : m_data(data), m_dimension(dim), m_step(ste) {}

TensorView TensorView::operator[](size_t index) {
    std::vector<size_t> newDim(m_dimension.begin() + 1, m_dimension.end());
    std::vector<size_t> newStep(m_step.begin() + 1, m_step.end());
    return TensorView(m_data + index * m_step[0], newDim, newStep);
}

int& TensorView::operator()(const std::vector<size_t>& index) {
    size_t offset = 0;
    for(size_t i = 0; i < index.size(); i++) {
        offset += index[i] * m_step[i];
    }
    return m_data[offset];
}

const int& TensorView::operator()(const std::vector<size_t>& index) const {
    size_t offset = 0;
    for(size_t i = 0; i < index.size(); i++) {
        offset += index[i] * m_step[i];
    }
    return m_data[offset];
}

int& TensorView::operator()(size_t index) {
    return m_data[index * m_step[0]];
}

const int& TensorView::operator()(size_t index) const {
    return m_data[index * m_step[0]];
}

Tensor TensorView::asTensor() const {
    Tensor t(m_dimension, false);
    size_t size = 1;
    for (size_t d : m_dimension) {
        size *= d;
    }
    for(size_t i = 0; i < size; i++) {
        t.m_data[i] = m_data[i];
    }
    return t;
}

Tensor::Tensor(const std::vector<size_t>& dim, bool zeroInit)
    : m_dimension(dim), m_size(1) {
    for (size_t d : m_dimension) {
        m_size *= d;
    }
    calculateStep();
    m_data = zeroInit ? new int[m_size]() : new int[m_size];
}

Tensor::~Tensor() { delete[] m_data; }

Tensor::Tensor(const Tensor& other) 
    : m_dimension(other.m_dimension), m_step(other.m_step), m_size(other.m_size) {
        m_data = new int[m_size];
        for(size_t i = 0; i < m_size; i++) {
            m_data[i] = other.m_data[i];
        }
    }

Tensor& Tensor::operator=(const Tensor& other) {
    if(this != &other) {
        delete[] m_data;
        m_dimension = other.m_dimension;
        m_step = other.m_step;
        m_size = other.m_size;
        m_data = new int[m_size];
        for(size_t i = 0; i < m_size; i++) {
            m_data[i] = other.m_data[i];
        }
    }
    return *this;
}

void Tensor::calculateStep() {
    m_step.resize(m_dimension.size());
    if(m_dimension.empty()) { return; }
    m_step.back() = 1;
    for(int i = static_cast<int>(m_dimension.size()) - 2; i >= 0; i--) {
        m_step[i] = m_step[i + 1] * m_dimension[i + 1];
    }
}

TensorView Tensor::operator[](size_t index) {
    std::vector<size_t> newDim(m_dimension.begin() + 1, m_dimension.end());
    std::vector<size_t> newStep(m_step.begin() + 1, m_step.end());
    return TensorView(m_data + index * m_step[0], newDim, newStep);
}

int& Tensor::operator()(const std::vector<size_t>& index) {
    size_t offset = 0;
    for(size_t i = 0; i < index.size(); i++) {
        offset += index[i] * m_step[i];
    }
    return m_data[offset];
}

const int& Tensor::operator()(const std::vector<size_t>& index) const {
    size_t offset = 0;
    for(size_t i = 0; i < index.size(); i++) {
        offset += index[i] * m_step[i];
    }
    return m_data[offset];
}

int& Tensor::operator()(size_t index) {
    return m_data[index * m_step[0]];
}

const int& Tensor::operator()(size_t index) const {
    return m_data[index * m_step[0]];
}

void Tensor::printDebugInfo() const {
    std::cout << "wymiary - ";
    for (size_t i = 0; i < m_dimension.size(); ++i) std::cout << m_dimension[i] << (i == m_dimension.size() - 1 ? "" : ", ");
    std::cout << "\nstridy - ";
    for (size_t i = 0; i < m_step.size(); ++i) std::cout << m_step[i] << (i == m_step.size() - 1 ? "" : ", ");
    std::cout << "\ndane:\n";
    for (size_t i = 0; i < m_size; ++i) std::cout << m_data[i] << (i == m_size - 1 ? "" : ", ");
    std::cout << "\n";
}
