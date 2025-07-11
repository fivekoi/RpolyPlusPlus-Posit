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

#ifndef RPOLY_PLUS_PLUS_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_
#define RPOLY_PLUS_PLUS_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_

#include "src/reals.h"

#include "libs/Eigen/Core"

namespace rpoly_plus_plus {

// A three-stage algorithm for finding roots of polynomials with real
// coefficients as outlined in: "A Three-Stage Algorithm for Real Polynomaials
// Using Quadratic Iteration" by Jenkins and Traub, SIAM 1970. Please note that
// this variant is different than the complex-coefficient version, and is
// estimated to be up to 4 times faster.
//
// The algorithm works by computing shifts in so-called "K-polynomials" that
// exaggerate the roots. Once a root is found (or in the real-polynomial case, a
// pair of roots) then it is divided from the polynomial and the process is
// repeated.
bool FindPolynomialRootsJenkinsTraub(const VectorReal& polynomial,
                                     VectorReal* real_roots,
                                     VectorReal* complex_roots);

}  // namespace rpoly_plus_plus

#endif  // RPOLY_PLUS_PLUS_FIND_POLYNOMIAL_ROOTS_JENKINS_TRAUB_H_
