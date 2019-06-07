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
    void computeN(long int p, long int q);
    void generateE(long int p, long int q);
    void generateD(long int p, long int q);
    long int gcd(long int a, long int b);
    std::vector<long int> extended_gcd(long int a, long int b);
    long int encrypt(long int m);
    long int decrypt(long int c);

    std::vector<long int> getPublicKey();
    std::vector<long int> getSecretKey();
private:
    long int e;
    long int n;
    long int d;
    long int phin;

    long int modularExp(long int a, long int b, long int n);
}

#endif // #RSA_CRYPTOSYS
