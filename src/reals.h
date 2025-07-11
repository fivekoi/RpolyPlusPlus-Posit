#ifndef REALS_H
#define REALS_H

#include "libs/universal/include/universal/math/mathlib_shim.hpp"
#include "libs/universal/include/universal/math/complex/manipulators.hpp"
#include "libs/Eigen/Core"

// Determines which numerical representation to use
// REAL_POSIT64, REAL_POSIT16, REAL_DOUBLE, REAL_FLOAT 
#define REAL_POSIT16
// Will also define flag for which math to use (for ops like sqrt and abs): SW_MATH or STL_MATH

#ifdef REAL_POSIT64
    #include "libs/universal/include/universal/number/posit/posit.hpp"
    using Real = sw::universal::posit<64, 2>;

    #define SW_MATH
#endif
#ifdef REAL_POSIT16
    #include "libs/universal/include/universal/number/posit/posit.hpp"
    using Real = sw::universal::posit<16, 2>;

    #define SW_MATH
#endif
#ifdef REAL_DOUBLE
    using Real = double;

    #define SW_MATH
#endif
#ifdef REAL_FLOAT
    using Real = float;

    #define SW_MATH
#endif
#ifdef REAL_BFLOAT16
    #include "libs/universal/include/universal/number/bfloat/bfloat.hpp"
    using Real = sw::universal::bfloat16;

    #define SW_MATH
#endif


using VectorReal = Eigen::Matrix<Real, -1, 1>;
using VectorReal3D = Eigen::Matrix<Real, 3, 1>;
using VectorReal3cD = Eigen::Matrix<std::complex<Real>, 3, 1>;

#endif