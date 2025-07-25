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

#ifndef RPOLY_PLUS_PLUS_TEST_TEST_UTILS_H_
#define RPOLY_PLUS_PLUS_TEST_TEST_UTILS_H_

#include "libs/Eigen/Core"
#include "gtest/gtest.h"

#include "src/reals.h"

namespace rpoly_plus_plus {
namespace test {

// Assert that values of the two matrices are nearly the same.
template <typename Derived>
void ExpectMatricesNear(const Eigen::MatrixBase<Derived>& a,
                        const Eigen::MatrixBase<Derived>& b,
                        Real tolerance) {
  ASSERT_EQ(a.rows(), b.rows());
  ASSERT_EQ(a.cols(), b.cols());
  for (int i = 0; i < a.rows(); i++)
    for (int j = 0; j < a.cols(); j++)
      ASSERT_NEAR(a(i, j), b(i, j), tolerance)
          << "Entry (" << i << ", " << j << ") did not meet the tolerance!";
}

void ExpectArraysNear(int n,
                      const Real* a,
                      const Real* b,
                      Real tolerance) {
  ASSERT_GT(n, 0);
  for (int i = 0; i < n; i++) {
    EXPECT_NEAR(static_cast<double>(a[i]), static_cast<double>(b[i]), static_cast<double>(tolerance)) << "i = " << i;
  }
}

// Expects that for all i = 1,.., n - 1
//
//   |p[i] / max_norm_p - q[i] / max_norm_q| < tolerance
//
// where max_norm_p and max_norm_q are the max norms of the arrays p
// and q respectively.
bool ArraysEqualUpToScale(int n, const Real* p, const Real* q,
                          Real tolerance) {
  Eigen::Map<const VectorReal> p_vec(p, n);
  Eigen::Map<const VectorReal> q_vec(q, n);

  // Use the cos term in the dot product to determine equality normalized for
  // scale.
  const Real cos_diff = p_vec.dot(q_vec) / (p_vec.norm() * q_vec.norm());
  return sw::universal::abs(cos_diff) >= 1.0 - tolerance;
}

}  // namespace test
}  // namespace rpoly_plus_plus
#endif  // RPOLY_PLUS_PLUS_TEST_TEST_UTILS_H_
