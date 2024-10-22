// BSD 2-Clause License
//
// Copyright (c) 2021, Darrell Long
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <math.h>
#include <stdio.h>

double integrate(double (*f)(double), double a, double b, double n) {
    double h = (b - a) / n; // Interval width
    double sum = 0.0; // Accumulator
    sum += f(a); // First term
    double tmp = 0.0; // Even terms
    for (int j = 1; j <= floor(n / 2 - 1); j += 1) {
        tmp += f(a + 2 * j * h);
    }
    sum += 2.0 * tmp;
    tmp = 0.0; // Odd terms
    for (int j = 1; j <= floor(n / 2); j += 1) {
        tmp += f(a + (2 * j - 1) * h);
    }
    sum += 4.0 * tmp;
    sum += f(b); // Last term
    sum *= h / 3.0;
    return sum;
}

double g(double x) { return exp(-x * x); }

double h(double x) { return sqrt(1 - x * x); }

double l(double x) { return sqrt(x) * exp(-x); }

int main(void) {
    double p = integrate(g, -10.0, 10.0, 100);
    double 𝛑 = p * p;
    printf("𝛑 = %22.20lf (𝚫 = % 22.20lf)\n", 𝛑, M_PI - 𝛑);
    p = integrate(h, 0.0, 1.0, 10000000);
    𝛑 = 4 * p;
    printf("𝛑 = %22.20lf (𝚫 = % 22.20lf)\n", 𝛑, M_PI - 𝛑);
    p = integrate(l, 0.0, 30.0, 3000000);
    𝛑 = 4 * p * p;
    printf("𝛑 = %22.20lf (𝚫 = % 22.20lf)\n", 𝛑, M_PI - 𝛑);
    return 0;
}
