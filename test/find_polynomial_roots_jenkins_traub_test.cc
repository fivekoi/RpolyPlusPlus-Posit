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

#include <algorithm>
#include <vector>
#include "gtest/gtest.h"

#include "src/reals.h"
#include "src/find_polynomial_roots_jenkins_traub.h"
#include "src/polynomial.h"
#include "test/test_utils.h"

namespace rpoly_plus_plus {

namespace {

// For IEEE-754 doubles, machine precision is about 2e-16.
const Real kEpsilon = 1e-10; // TODO: it seems like these tests just comparae to expected double precision
const Real kEpsilonLoose = 1e-8;

// Return the constant polynomial p(x) = 1.23.
VectorReal ConstantPolynomial(Real value) {
  VectorReal poly(1);
  poly(0) = value;
  return poly;
}

// Return the polynomial p(x) = poly(x) * (x - root).
VectorReal AddRealRoot(const VectorReal& poly, Real root) {
  VectorReal poly2(poly.size() + 1);
  poly2.setZero();
  poly2.head(poly.size()) += poly;
  poly2.tail(poly.size()) -= root * poly;
  return poly2;
}

// Return the polynomial
// p(x) = poly(x) * (x - real - imag*i) * (x - real + imag*i).
VectorReal AddComplexRootPair(const VectorReal& poly, Real real,
                            Real imag) {
  VectorReal poly2(poly.size() + 2);
  poly2.setZero();
  // Multiply poly by x^2 - 2real + abs(real,imag)^2
  poly2.head(poly.size()) += poly;
  poly2.segment(1, poly.size()) -= 2 * real * poly;
  poly2.tail(poly.size()) += (real * real + imag * imag) * poly;
  return poly2;
}

// Sort the entries in a vector.
// Needed because the roots are not returned in sorted order.
VectorReal SortVector(const VectorReal& in) {
  VectorReal out(in);
  std::sort(out.data(), out.data() + out.size());
  return out;
}

// Run a test with the polynomial defined by the N real roots in roots_real.
// If use_real is false, NULL is passed as the real argument to
// FindPolynomialRoots. If use_imaginary is false, NULL is passed as the
// imaginary argument to FindPolynomialRoots.
template <int N>
void RunPolynomialTestRealRoots(const Real (&real_roots)[N], bool use_real,
                                bool use_imaginary, Real epsilon) {
  VectorReal real;
  VectorReal imaginary;
  VectorReal poly = ConstantPolynomial(1.23);
  for (int i = 0; i < N; ++i) {
    poly = AddRealRoot(poly, real_roots[i]);
  }
  VectorReal* const real_ptr = use_real ? &real : NULL;
  VectorReal* const imaginary_ptr = use_imaginary ? &imaginary : NULL;
  bool success = FindPolynomialRootsJenkinsTraub(poly, real_ptr, imaginary_ptr);

  EXPECT_EQ(success, true);
  if (use_real) {
    EXPECT_EQ(real.size(), N);
    real = SortVector(real);
    test::ExpectArraysNear(N, real.data(), real_roots, epsilon);
  }
  if (use_imaginary) {
    EXPECT_EQ(imaginary.size(), N);
    const VectorReal zeros = VectorReal::Zero(N);
    test::ExpectArraysNear(N, imaginary.data(), zeros.data(), epsilon);
  }
}

}  // namespace

TEST(Polynomial, MyTest) {
  VectorReal polynomial(4);
  VectorReal roots_re(3);
  VectorReal roots_im(3);

  polynomial(0) = 1;
  polynomial(1) = -7;
  polynomial(2) = 14;
  polynomial(3) = -8;

  bool success = FindPolynomialRootsJenkinsTraub(polynomial, &roots_re, &roots_im);
  std::cout << roots_re << '\n';
  std::cout << roots_im << '\n';

  EXPECT_EQ(success, true);
}

// TEST(Polynomial, InvalidPolynomialOfZeroLengthIsRejected) {
//   // Vector poly(0) is an ambiguous constructor call, so
//   // use the constructor with explicit column count.
//   VectorReal poly(0, 1);
//   VectorReal real;
//   VectorReal imag;
//   bool success = FindPolynomialRootsJenkinsTraub(poly, &real, &imag);

//   EXPECT_EQ(success, false);
// }

// TEST(Polynomial, ConstantPolynomialReturnsNoRoots) {
//   VectorReal poly = ConstantPolynomial(1.23);
//   VectorReal real;
//   VectorReal imag;
//   bool success = FindPolynomialRootsJenkinsTraub(poly, &real, &imag);

//   EXPECT_EQ(success, true);
//   EXPECT_EQ(real.size(), 0);
//   EXPECT_EQ(imag.size(), 0);
// }

// TEST(Polynomial, LinearPolynomialWithPositiveRootWorks) {
//   const Real roots[1] = { 42.42 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, LinearPolynomialWithNegativeRootWorks) {
//   const Real roots[1] = { -42.42 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, QuadraticPolynomialWithPositiveRootsWorks) {
//   const Real roots[2] = { 1.0, 42.42 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, QuadraticPolynomialWithOneNegativeRootWorks) {
//   const Real roots[2] = { -42.42, 1.0 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, QuadraticPolynomialWithTwoNegativeRootsWorks) {
//   const Real roots[2] = { -42.42, -1.0 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, QuadraticPolynomialWithCloseRootsWorks) {
//   const Real roots[2] = { 42.42, 42.43 };
//   RunPolynomialTestRealRoots(roots, true, false, kEpsilonLoose);
// }

// TEST(Polynomial, QuadraticPolynomialWithComplexRootsWorks) {
//   VectorReal real;
//   VectorReal imag;

//   VectorReal poly = ConstantPolynomial(1.23);
//   poly = AddComplexRootPair(poly, 42.42, 4.2);
//   bool success = FindPolynomialRootsJenkinsTraub(poly, &real, &imag);

//   EXPECT_EQ(success, true);
//   EXPECT_EQ(real.size(), 2);
//   EXPECT_EQ(imag.size(), 2);
//   EXPECT_NEAR(static_cast<double>(real(0)), 42.42, static_cast<double>(kEpsilon));
//   EXPECT_NEAR(static_cast<double>(real(1)), 42.42, static_cast<double>(kEpsilon));
//   EXPECT_NEAR(static_cast<double>(sw::universal::abs(imag(0))), 4.2, static_cast<double>(kEpsilon));
//   EXPECT_NEAR(static_cast<double>(sw::universal::abs(imag(1))), 4.2, static_cast<double>(kEpsilon));
//   EXPECT_NEAR(static_cast<double>(sw::universal::abs(imag(0) + imag(1))), 0.0, static_cast<double>(kEpsilon));
// }

// TEST(Polynomial, QuarticPolynomialWorks) {
//   const Real roots[4] = { 1.23e-4, 1.23e-1, 1.23e+2, 1.23e+5 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilonLoose);
// }

// TEST(Polynomial, QuarticPolynomialWithTwoClustersOfCloseRootsWorks) {
//   const Real roots[4] = { 1.23e-1, 2.46e-1, 1.23e+5, 2.46e+5 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilonLoose);
// }

// TEST(Polynomial, QuarticPolynomialWithTwoZeroRootsWorks) {
//   const Real roots[4] = { -42.42, 0.0, 0.0, 42.42 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilonLoose);
// }

// TEST(Polynomial, QuarticMonomialWorks) {
//   const Real roots[4] = { 0.0, 0.0, 0.0, 0.0 };
//   RunPolynomialTestRealRoots(roots, true, true, kEpsilon);
// }

// TEST(Polynomial, NullPointerAsImaginaryPartWorks) {
//   const Real roots[4] = { 1.23e-4, 1.23e-1, 1.23e+2, 1.23e+5 };
//   RunPolynomialTestRealRoots(roots, true, false, kEpsilonLoose);
// }

// TEST(Polynomial, NullPointerAsRealPartWorks) {
//   const Real roots[4] = { 1.23e-4, 1.23e-1, 1.23e+2, 1.23e+5 };
//   RunPolynomialTestRealRoots(roots, false, true, kEpsilon);
// }

// TEST(Polynomial, BothOutputArgumentsNullWorks) {
//   const Real roots[4] = { 1.23e-4, 1.23e-1, 1.23e+2, 1.23e+5 };
//   RunPolynomialTestRealRoots(roots, false, false, kEpsilon);
// }

// TEST(Polynomial, JenkinsTraubManyRoots) {
//   static const int N = 25;
//   VectorReal poly = ConstantPolynomial(1.23);
//   VectorReal roots = VectorReal::Random(N);
//   roots = SortVector(roots);

//   for (int i = 0; i < N; ++i) {
//     poly = AddRealRoot(poly, roots[i]);
//   }

//   VectorReal real;
//   bool success = FindPolynomialRootsJenkinsTraub(poly, &real, NULL);
//   real = SortVector(real);
//   EXPECT_EQ(success, true);
//   EXPECT_EQ(real.size(), N);
//   for (int i = 0; i < real.size(); i++) {
//     EXPECT_NEAR(static_cast<double>(EvaluatePolynomial(poly, real[i])), 0, static_cast<double>(kEpsilonLoose));
//   }
// }

// TEST(Polynomial, HardPolynomial1) {
//   VectorReal polynomial(11);
//   VectorReal roots_re(10);
//   VectorReal roots_im(10);

//   polynomial(10) = -52412.8655144021;
//   polynomial(9) = -28342.548095425875;
//   polynomial(8) = 20409.84088622263;
//   polynomial(7) = 25844.743360023815;
//   polynomial(6) = 11474.831044766257;
//   polynomial(5) = 1909.2968243041091;
//   polynomial(4) = -692.3579951742573;
//   polynomial(3) = -562.5089223272787;
//   polynomial(2) = -105.89974320540716;
//   polynomial(1) = 18.62488243410351;
//   polynomial(0) = 5.576312106019016;

//   EXPECT_TRUE(
//       FindPolynomialRootsJenkinsTraub(polynomial, &roots_re, &roots_im));
// }

// TEST(Polynomial, HardPolynomial2) {
//   VectorReal polynomial(20);
//   VectorReal roots_re(19);
//   VectorReal roots_im(19);

//   polynomial(19) = -3.3501738067312306e8;
//   polynomial(18) = -6.884086124142883e8;
//   polynomial(17) = 7.702813653628246e8;
//   polynomial(16) = 8.451429594854779e8;
//   polynomial(15) = -7.822417923012168e8;
//   polynomial(14) = -2.0621500003041908e8;
//   polynomial(13) = 2.71193932055516e8;
//   polynomial(12) = 2191206.652049609;
//   polynomial(11) = -4.3103846059516795e7;
//   polynomial(10) = 3893518.9815099635;
//   polynomial(9) = 4037788.101972703;
//   polynomial(8) = -541891.2574823081;
//   polynomial(7) = -260979.552665553;
//   polynomial(6) = 38001.29427556511;
//   polynomial(5) = 12074.712839195976;
//   polynomial(4) = -1512.0183242937462;
//   polynomial(3) = -388.5049059868163;
//   polynomial(2) = 27.301047297669705;
//   polynomial(1) = 6.8768381102442575;
//   polynomial(0) = 0;

//   EXPECT_TRUE(
//       FindPolynomialRootsJenkinsTraub(polynomial, &roots_re, &roots_im));
// }

// // This test polynomial was provided by a user.
// TEST(Polynomial, JenkinsTraub4Roots1) {
//   const Real roots[4] = {1.0843989379558703, 1.0844294564653463,
//                            1.3072756126590779, 1.4643848994415114};
//   RunPolynomialTestRealRoots(roots, true, false, kEpsilonLoose);
// }

// This test polynomial was provided by a user.
// TEST(Polynomial, JenkinsTraub4Roots2) {
//   static const int N = 4;
//   for (int j = 0; j < 10000; ++j) {
//     VectorReal poly = ConstantPolynomial(1.23);
//     VectorReal roots = VectorReal::Random(N);
//     for (int i = 0; i < N; ++i) {
//         roots(i) *= 0.5;
//         roots(i) += 1.0;
//     }

//     roots = SortVector(roots);

//     for (int i = 0; i < N; ++i) {
//       poly = AddRealRoot(poly, roots[i]);
//     }

//     VectorReal real;
//     const bool success = FindPolynomialRootsJenkinsTraub(poly, &real, NULL);
//     EXPECT_EQ(success, true);
//     real = SortVector(real);

//     EXPECT_EQ(real.size(), N);
//     for (int i = 0; i < real.size(); i++) {
//       EXPECT_NEAR(static_cast<double>(EvaluatePolynomial(poly, real[i])), 0, static_cast<double>(kEpsilonLoose));
//     }
//   }
// }

}  // namespace rpoly_plus_plus
