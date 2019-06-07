/*
    This code implements the RSA Cryptosystem, as specified by the original paper.
*/

#ifndef RSA_CRYPTOSYS
#define RSA_CRYPTOSYS

#include <vector>
#include <limits>
#include <math.h>

class RSA {
public:
    void computeN(unsigned long int p, unsigned long int q);
    void computeE(unsigned long int p, unsigned long int q);
    void computeD(unsigned long int p, unsigned long int q);
    unsigned long int gcd(unsigned long int a, unsigned long int b);
    std::vector<unsigned long int> extended_gcd(unsigned long int a, unsigned long int b);
    unsigned long int encrypt(unsigned long int m);
    unsigned long int decrypt(unsigned long int c);

    std::vector<unsigned long int> getPublicKey();
    std::vector<unsigned long int> getSecretKey();
private:
    unsigned long int e;
    unsigned long int n;
    unsigned long int d;
    unsigned long int phin;

    unsigned long int modularExp(unsigned long int a, unsigned long int b, unsigned long int n);
};

#endif // #RSA_CRYPTOSYS
