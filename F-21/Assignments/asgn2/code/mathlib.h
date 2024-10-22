#pragma once

#define EPSILON 1e-14

static inline double absolute(double x) {
    return x < 0.0 ? -x : x;
}

double e(void);

int e_terms(void);

double pi_euler(void);

int pi_euler_terms(void);

double pi_bbp(void);

int pi_bbp_terms(void);

double pi_madhava(void);

int pi_madhava_terms(void);

double pi_viete(void);

int pi_viete_factors(void);

double sqrt_newton(double);

int sqrt_newton_iters(void);
