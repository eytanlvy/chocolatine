// The MIT License (MIT)

// Copyright (c) 2017 Fábio Maia

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// Modified by @Yael2718 on 22/05/2024

#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include <stdlib.h>
#include <stdbool.h>

typedef struct polynomial_s Polynomial;

Polynomial  *polynomial_new                     (size_t degree);
Polynomial  *polynomial_copy                    (Polynomial *p);
void         polynomial_destroy                 (Polynomial **p);

size_t       polynomial_get_degree              (Polynomial *p);
void         polynomial_set_coefficient         (Polynomial *p, size_t i, double a);
double       polynomial_get_coefficient         (Polynomial *p, size_t i);

bool         polynomial_is_zero                 (Polynomial *p);
bool         polynomial_equals                  (Polynomial *p1, Polynomial *p2);

double       polynomial_evaluate                (Polynomial *p, double x);
Polynomial  *polynomial_add                     (Polynomial *p1, Polynomial *p2);
Polynomial  *polynomial_subtract                (Polynomial *p1, Polynomial *p2);
Polynomial  *polynomial_multiply                (Polynomial *p1, Polynomial *p2);
Polynomial  *polynomial_multiply_by_constant    (Polynomial *p, double c);

Polynomial  *polynomial_symmetric               (Polynomial *p);
Polynomial  *polynomial_derivative              (Polynomial *p);
Polynomial  *polynomial_indefinite_integral     (Polynomial *p, double c);
double       polynomial_definite_integral       (Polynomial *p, double a, double b);

void         polynomial_mod                     (Polynomial *p, int n);
Polynomial  *polynomial_multiply_mod            (Polynomial *p1, Polynomial *p2, int n);
Polynomial  *polynomial_divide_mod              (Polynomial *p1, Polynomial *p2, int n, Polynomial* q);
Polynomial  *polynomial_inverse_mod             (Polynomial *p, int n);

#endif /* __POLYNOMIAL_H__ */
