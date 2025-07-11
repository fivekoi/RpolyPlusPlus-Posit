// Copyright (C) 2015 Chris Sweeney. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of Chris Sweeney nor the names of its contributors may
//       be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "src/reals.h"
#include "src/polynomial.h"

#include "libs/Eigen/Core"
#include <cmath>

namespace rpoly_plus_plus {

// Remove leading terms with zero coefficients.
VectorReal RemoveLeadingZeros(const VectorReal& polynomial_in) {
  int i = 0;
  while (i < (polynomial_in.size() - 1) && polynomial_in(i) == Real(0)) {
    ++i;
  }
  return polynomial_in.tail(polynomial_in.size() - i);
}

VectorReal DifferentiatePolynomial(const VectorReal& polynomial) {
  const int degree = polynomial.rows() - 1;

  // Degree zero polynomials are constants, and their derivative does
  // not result in a smaller degree polynomial, just a degree zero
  // polynomial with value zero.
  if (degree == 0) {
    return VectorReal::Zero(1);
  }

  VectorReal derivative(degree);
  for (int i = 0; i < degree; ++i) {
    derivative(i) = (degree - i) * polynomial(i);
  }

  return derivative;
}

VectorReal MultiplyPolynomials(const VectorReal& poly1, const VectorReal& poly2) {
  VectorReal multiplied_poly = VectorReal::Zero(poly1.size() + poly2.size() - 1);;
  for (int i = 0; i < poly1.size(); i++) {
    for (int j = 0; j < poly2.size(); j++) {
      multiplied_poly.reverse()(i + j) +=
          poly1.reverse()(i) * poly2.reverse()(j);
    }
  }
  return multiplied_poly;
}

VectorReal AddPolynomials(const VectorReal& poly1, const VectorReal& poly2) {
  if (poly1.size() > poly2.size()) {
    VectorReal sum = poly1;
    sum.tail(poly2.size()) += poly2;
    return sum;
  } else {
    VectorReal sum = poly2;
    sum.tail(poly1.size()) += poly1;
    return sum;
  }
}

Real FindRootIterativeNewton(const VectorReal& polynomial,
                               const Real x0,
                               const Real epsilon,
                               const int max_iterations) {
  Real root = x0;
  const VectorReal derivative = DifferentiatePolynomial(polynomial);
  Real prev;
  for (int i = 0; i < max_iterations; i++) {
    prev = root;
    root -= EvaluatePolynomial(polynomial, root) /
            EvaluatePolynomial(derivative, root);
    if (
      #ifdef SW_MATH
      sw::universal::abs(prev - root) < epsilon
      #endif

    ) {
      break;
    }
  }
  return root;
}

}  // namespace rpoly_plus_plus
