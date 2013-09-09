#include <stdio.h>

main() {
    float  integral;
    float  a, b;
    int    n;
    float  h;
    float  x;
    int    i;

    float f(float x);

    printf("Enter a, b, and n\n");
    scanf("%f %f %d", &a, &b, &n);

    h = (b-a)/n;
    integral = (f(a) + f(b));
    x = a;
    for (i = 1; i <= n-1; i++) {
        x = x + h;
        integral = integral + (i%2?4:2)*f(x);
    }
    integral = integral*h/3.0;

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %f\n", a, b, integral);
}

float f(float x) {
    float return_val;
    return_val = x*x;
    return return_val;
}
