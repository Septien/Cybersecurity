#include "RSA.h"

/**/
void RSA::computeN(long int p, long int q)
{
    this->n = p * q;
}

/* The pair (e, n) corresponds to the public-key part of the protocol */
void RSA::computeE(long int p, long int q)
{
    // Value of the Euler function (phi)
    this->phin = (p - 1) * (q - 1);
    // Find e: starting at two and ending at phin, get the first found
    this->e = 2;
    while (e < phin)
    {
        if (gcd(e, phin) == 1)
            break;
        e++;
    }
}

/* The pair (d, n) corresponds to the secret-key part of the protocol */
void RSA::computeD(long int p, long int q)
{
    // Compute the multiplicative inverse of e modulo phin
    std::vector<long int> coeff = extended_gcd(this->e, this->phin);
    if (coeff[0] / 1 % 1 == 0)
    {
        this->d = (coeff[1] * (1 / coeff[0])) % this->phin;
    }
}

/* Use the Euclide algorithm to compute the gcd of a and b */
long int RSA::gcd(long int a, long int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

/* The version of the Euclide algorithm that returns the coefficients x, y that make
d = gcd(a, b) = ax + by.
*/
std::vector<long int> RSA::extended_gcd(long int a, long int b)
{
    if (b == 0)
        return std::vector<long int>{a, 1, 0};
    // Returns: (d', x', y')
    std::vector<long int> coeffP = extended_gcd(b, a % b);
    // The coefficients: (d, x, y) = (d', x', x' - int(a/b)y')
    std::vector<long int> coeff{ coeffP[0], coeffP[2], coeff[1] - (int(a / b) * coeff[2]) };
    return coeff;
}

/* Encrypts the messages contained at m using the secret key. */
long int RSA::encrypt(long int m)
{
    return modularExp(m, this->e, this->phin);
}

/* Decrypts the cipher text c using the public key. */
long int RSA::decrypt(long int c)
{
    return modularExp(c, this->d, this->phin);
}

/* Get the computed public key */
std::vector<long int> RSA::getPublicKey()
{
    return std::vector<long int>{this->d, this->phin};

}

/* Get the computed secret key */
std::vector<long int> RSA::getSecretKey()
{
    return std::vector<long int>{this->e, this->phin};
}

long int RSA::modularExp(long int a, long int b, long int n)
{
    // Get number of bits
    int k = std::numeric_limits<long int>::digits;
    long int c = 0;
    long int d = 1;
    long int bk = pow(2, k-1);
    for (int i = k-1; i >= 0; i--)
    {
        c *= 2;
        d = (d * d) % n;
        // Test if bit log(b) is set at m
        if (a & bk)
        {
            c++;
            d = (d * a) % n;
        }
        // Displace one position
        bk >>= 1;
    }
    return d;
}
