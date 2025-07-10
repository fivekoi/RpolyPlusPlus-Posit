#ifndef REALS_H
#define REALS_H

#include "universal/number/posit/posit.hpp"
#include "universal/math/functions.hpp"

#include "libs/Eigen/Core"

// double, sw::universal::posit<64, 4>
using Real = sw::universal::posit<64, 4>;

using VectorReal = Eigen::Matrix<Real, -1, 1>;
using VectorReal3D = Eigen::Matrix<Real, 3, 1>;
using VectorReal3cD = Eigen::Matrix<std::complex<Real>, 3, 1>;

#endif