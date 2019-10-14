#include "rsa_cryptosys.hpp"
#include <iostream>

/**/
void RSA::computeN(unsigned long int p, unsigned long int q)
{
    this->n = p * q;
}

/* The pair (e, n) corresponds to the public-key part of the protocol */
void RSA::computeE(unsigned long int p, unsigned long int q)
{
    // Value of the Euler function (phi)
    this->phin = (p - 1) * (q - 1);
    // Find e: starting at two and ending at phin, get the first found
    this->e = 2;
    while (this->e < this->phin)
    {
        if (gcd(this->e, this->phin) == 1)
            break;
        e++;
    }
}

/* The pair (d, n) corresponds to the secret-key part of the protocol */
void RSA::computeD(unsigned long int p, unsigned long int q)
{
    // Compute the multiplicative inverse of e modulo phin
    std::vector<unsigned long int> coeff = extended_gcd(this->e, this->phin);
    if (coeff[0] / 1 % 1 == 0)
    {
        this->d = (coeff[1] * (1 / coeff[0])) % this->phin;
    }
}

/* Use the Euclide algorithm to compute the gcd of a and b */
unsigned long int RSA::gcd(unsigned long int a, unsigned long int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

/* The version of the Euclide algorithm that returns the coefficients x, y that make
d = gcd(a, b) = ax + by.
*/
std::vector<unsigned long int> RSA::extended_gcd(unsigned long int a, unsigned long int b)
{
    if (b == 0)
    {
        std::vector<unsigned long int> c = std::vector<unsigned long int>{a, 1, 0};
        return c;
    }
    // Returns: (d', x', y')
    std::vector<unsigned long int> coeffP = extended_gcd(b, a % b);
    // The coefficients: (d, x, y) = (d', x', x' - int(a/b)y')
    unsigned long int d = coeffP[0];
    unsigned long int x = coeffP[2];
    unsigned long int y = coeffP[1] - (int(a / b) * coeffP[2]);
    std::vector<unsigned long int> coeff{ d, x, y };
    return coeff;
}

/* Encrypts the messages contained at m using the secret key. */
unsigned long int RSA::encrypt(unsigned long int m)
{
    return modularExp(m, this->e, this->n);
}

/* Decrypts the cipher text c using the public key. */
unsigned long int RSA::decrypt(unsigned long int c)
{
    return modularExp(c, this->d, this->n);
}

/* Get the computed public key */
std::vector<unsigned long int> RSA::getPublicKey()
{
    return std::vector<unsigned long int>{this->e, this->n};

}

/* Get the computed secret key */
std::vector<unsigned long int> RSA::getSecretKey()
{
    return std::vector<unsigned long int>{this->d, this->n};
}

/*
*   Computes the operation a^b mod m, using the repeated-squaring algorithm.
*/
unsigned long int RSA::modularExp(unsigned long int a, unsigned long int b, unsigned long int m)
{
    unsigned long int c = 0;
    unsigned long int d = 1;
    size_t k = 8 * sizeof(e);
    unsigned long int mask = 1;
    mask = mask << (k - 1);
    int i;
    for (i = k - 1; i >= 0; i--)
    {
        c *= 2;
        d = (d * d) % m;
        if (b & mask) // is the ith bit 1?
        {
            c++;
            d = (d * a) % m;
        }
        mask = mask >> 1;   // Prepare mask for the next bit
    }
    return d;
}
