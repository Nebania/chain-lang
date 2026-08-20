#pragma once

namespace SysMath {
    // Basic & Trigonometry
    double sin(double rad);
    double cos(double rad);
    double tan(double rad);
    double sqrt(double val);
    double pow(double base, double exp);
    double abs(double val);
    
    // Constants
    double pi();
    double e(); // Euler

    // Inverse Trigonometry
    double asin(double val);
    double acos(double val);
    double atan(double val);
    double atan2(double y, double x);

    // Hyperbolic
    double sinh(double rad);
    double cosh(double rad);
    double tanh(double rad);

    // Exponential & Logarithm
    double exp(double val);
    double log(double val);     // Natural log (ln)
    double log10(double val);
    double log2(double val);

    // Rounding & Utility
    double ceil(double val);
    double floor(double val);
    double round(double val);
    double min(double a, double b);
    double max(double a, double b);
}