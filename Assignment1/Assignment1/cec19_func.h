#ifndef CEC19_H
#define CEC19_H

inline void Lennard_Jones(double*, int, double*); /* Lennard Jones */
inline void Hilbert(double*, int, double*); /* Hilbert */
inline void Chebyshev(double*, int, double*); /* Chebyshev */
inline void schaffer_F7_func(double*, double*, int, double*, double*, int, int); /* Schwefel's F7 */
inline void ackley_func(double*, double*, int, double*, double*, int, int); /* Ackley's */
inline void rastrigin_func(double*, double*, int, double*, double*, int, int); /* Rastrigin's  */
inline void weierstrass_func(double*, double*, int, double*, double*, int, int); /* Weierstrass's  */
inline void schwefel_func(double*, double*, int, double*, double*, int, int); /* Schwefel's */
inline void escaffer6_func(double*, double*, int, double*, double*, int, int); /* Expanded Scaffer¡¯s F6  */
inline void happycat_func(double*, double*, int, double*, double*, int, int); /* HappyCat */
inline void griewank_func(double*, double*, int, double*, double*, int, int); /* Griewank's  */

inline void shiftfunc(double*, double*, int, double*);
inline void rotatefunc(double*, double*, int, double*);
inline void sr_func(double*, double*, int, double*, double*, double, int, int); /* shift and rotate */
inline void asyfunc(double*, double* x, int, double);
inline void oszfunc(double*, double*, int);

void cec19_test_func(double*, double*, int, int, int);
#endif