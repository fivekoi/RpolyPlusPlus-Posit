#ifndef REALS_H
#define REALS_H

#include "libs/universal/include/universal/math/mathlib_shim.hpp"
#include "libs/universal/include/universal/math/complex/manipulators.hpp"
#include "libs/Eigen/Core"

#include "libs/universal/include/universal/number/posit/posit.hpp"

// double, sw::universal::posit<32, 2>
using Real = sw::universal::posit<64, 2>;

using VectorReal = Eigen::Matrix<Real, -1, 1>;
using VectorReal3D = Eigen::Matrix<Real, 3, 1>;
using VectorReal3cD = Eigen::Matrix<std::complex<Real>, 3, 1>;

#endif