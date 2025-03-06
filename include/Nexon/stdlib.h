#ifndef NEXON_STDLIB_H
#define NEXON_STDLIB_H

#include <cmath>
#include <vector>
#include <string>
#include <complex>
#include <sstream>
#include <iostream>
#include <iomanip>

// Nexon Standard Library – production‑ready implementations of basic math and physics functions.
namespace NexonStd {

    inline double sqrt(double x) { return std::sqrt(x); }
    inline double sin(double x)  { return std::sin(x); }
    inline double cos(double x)  { return std::cos(x); }
    inline double tan(double x)  { return std::tan(x); }
    inline double log(double x)  { return std::log(x); }
    inline double exp(double x)  { return std::exp(x); }

    inline double gravitationalForce(double m1, double m2, double distance) {
        const double G = 6.67430e-11;
        return (G * m1 * m2) / (distance * distance);
    }

    class Vector3 {
    public:
        double x, y, z;
        Vector3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) { }
        Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
        Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
        Vector3 operator*(double scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        double dot(const Vector3 &other) const { return x * other.x + y * other.y + z * other.z; }
        double magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    };

    template<typename T>
    class BigArray {
    public:
        BigArray(size_t n) : data(n) { }
        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }
        size_t size() const { return data.size(); }
    private:
        std::vector<T> data;
    };

    template<typename T>
    std::string vectorToString(const std::vector<T>& vec) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); i++) {
            oss << std::fixed << std::setprecision(2) << vec[i];
            if (i != vec.size() - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    template<typename T>
    T average(const std::vector<T>& vec) {
        T sum = 0;
        for (const auto &v : vec) sum += v;
        return sum / vec.size();
    }

    template<typename T>
    T standardDeviation(const std::vector<T>& vec) {
        T avg = average(vec);
        T variance = 0;
        for (const auto &v : vec) variance += (v - avg) * (v - avg);
        return sqrt(variance / vec.size());
    }
}
#endif // NEXON_STDLIB_H
