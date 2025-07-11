#ifndef REALS_H
#define REALS_H

#include "libs/universal/include/universal/math/mathlib_shim.hpp"
#include "libs/universal/include/universal/math/complex/manipulators.hpp"
#include "libs/Eigen/Core"

// Determines which numerical system to use for Real
// REAL_POSIT32, REAL_DOUBLE, REAL_BFLOAT16
#define REAL_POSIT32
// Will define SW_MATH or STL_MATH depending on need

// TODO: need to change it to normal math functions for bfloats and others probably, but can be universal for posits?
#ifdef REAL_POSIT32
    #include "libs/universal/include/universal/number/posit/posit.hpp"
    using Real = sw::universal::posit<32, 2>;

    #define SW_MATH
#endif
#ifdef REAL_DOUBLE
    using Real = double;

    #defie STL_MATH
#endif
#ifdef REAL_BFLOAT16
    #include "libs/universal/include/universal/number/bfloat/bfloat.hpp"
    using Real = sw::universal::bfloat16;

    #define STL_MATH
#endif



using VectorReal = Eigen::Matrix<Real, -1, 1>;
using VectorReal3D = Eigen::Matrix<Real, 3, 1>;
using VectorReal3cD = Eigen::Matrix<std::complex<Real>, 3, 1>;

#endif