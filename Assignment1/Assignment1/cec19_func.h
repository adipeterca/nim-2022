#pragma once

void Lennard_Jones(double*, int, double*); /* Lennard Jones */
void Hilbert(double*, int, double*); /* Hilbert */
void Chebyshev(double*, int, double*); /* Chebyshev */
void schaffer_F7_func(double*, double*, int, double*, double*, int, int); /* Schwefel's F7 */
void ackley_func(double*, double*, int, double*, double*, int, int); /* Ackley's */
void rastrigin_func(double*, double*, int, double*, double*, int, int); /* Rastrigin's  */
void weierstrass_func(double*, double*, int, double*, double*, int, int); /* Weierstrass's  */
void schwefel_func(double*, double*, int, double*, double*, int, int); /* Schwefel's */
void escaffer6_func(double*, double*, int, double*, double*, int, int); /* Expanded Scaffer¡¯s F6  */
void happycat_func(double*, double*, int, double*, double*, int, int); /* HappyCat */
void griewank_func(double*, double*, int, double*, double*, int, int); /* Griewank's  */

void shiftfunc(double*, double*, int, double*);
void rotatefunc(double*, double*, int, double*);
void sr_func(double*, double*, int, double*, double*, double, int, int); /* shift and rotate */
void asyfunc(double*, double* x, int, double);
void oszfunc(double*, double*, int);

void cec19_test_func(double*, double*, int, int, int);