#include "../include/link_math.h"
#include <cmath>
#include <algorithm>

namespace SysMath {
    double sin(double rad) { return std::sin(rad); }
    double cos(double rad) { return std::cos(rad); }
    double tan(double rad) { return std::tan(rad); }
    double sqrt(double val) { return std::sqrt(val); }
    double pow(double base, double exp) { return std::pow(base, exp); }
    double abs(double val) { return std::abs(val); }
    
    double pi() { return 3.14159265358979323846; }
    double e() { return 2.71828182845904523536; }

    double asin(double val) { return std::asin(val); }
    double acos(double val) { return std::acos(val); }
    double atan(double val) { return std::atan(val); }
    double atan2(double y, double x) { return std::atan2(y, x); }

    double sinh(double rad) { return std::sinh(rad); }
    double cosh(double rad) { return std::cosh(rad); }
    double tanh(double rad) { return std::tanh(rad); }

    double exp(double val) { return std::exp(val); }
    double log(double val) { return std::log(val); }
    double log10(double val) { return std::log10(val); }
    double log2(double val) { return std::log2(val); }

    double ceil(double val) { return std::ceil(val); }
    double floor(double val) { return std::floor(val); }
    double round(double val) { return std::round(val); }

    double min(double a, double b) { return std::min(a, b); }
    double max(double a, double b) { return std::max(a, b); }
}