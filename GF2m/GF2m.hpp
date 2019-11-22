/*
* A simple implementation of Galois Fields for p = 2, and m multiple of 8.
*/
#ifndef GF2M_HPP
#define GF2M_HPP

#include <vector>

class GF2M {
private:
    std::vector<uint8_t> mx;
    std::vector<uint8_t> fx;
    unsigned int m;             // Number of bits, up to 2^64 - 1 bits
    unsigned int mB;            // Number of bytes

    std::vector<uint8_t> multiplyByX();

public:
    GF2M();
    GF2M(unsigned int, std::vector<uint8_t>&, std::vector<uint8_t>&);

    void setM(unsigned int);
    void setMx(std::vector<uint8_t>&);
    void setFx(std::vector<uint8_t>&);

    // Operators
    GF2M operator+(GF2M &);
    GF2M operator-(GF2M &);
    GF2M operator*(GF2M &);
    GF2M operator/(GF2M &);
    GF2M operator%(GF2M &);
    GF2M getInverse();
    GF2M gcd(GF2M &, GF2M &);
};

#endif // GF2M_HPP