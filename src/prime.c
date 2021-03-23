#include <math.h>

#include "prime.h"

// Return if int x is a prime number or not. 0 = True, 1 = False, -1 = undefined
int is_prime(const int x){
    if(x < 2) { return -1;}
    if(x < 4) {return 1;}
    if ((x % 2) == 0) { return 0;}
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2) {
        if (x % i == 0){
            return 0;
        }
    }
    return 1;
}

// Return the next prime number after submitted integer
int next_prime(int x){
    while (is_prime(x) != 1){
        ++x;
    }
    return x;
}