#include "link_math.h"
#include <cmath>

namespace SysMath {
    double sin(double rad) { return std::sin(rad); }
    double cos(double rad) { return std::cos(rad); }
    double tan(double rad) { return std::tan(rad); }
    double sqrt(double val) { return std::sqrt(val); }
    double pow(double base, double exp) { return std::pow(base, exp); }
    double abs(double val) { return std::abs(val); }
    double pi() { return 3.14159265358979323846; }
}
