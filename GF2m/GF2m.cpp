#include "GF2M.hpp"

GF2M::GF2M()
{
    m = 0;
}

GF2M::GF2M(unsigned int m, std::vector<uint8_t> &mx, std::vector<uint8_t> &fx)
{
    if (m%8 != 0)
    {
        cout << "Number of bits must be a multiple of 8." << endl;
        return;
    }
    this->m = m;
    this->mB = m / 8;
    // Reserve memory for each vector
    this->mx.assign(this->mB, 0);
    this->fx.assign(this->mB, 0);

    this->mx = mx;
    this->fx = fx;
}

/*
*   Performs the operation: x * f(x)
*/
std::vector<uint8_t> GF2M::multiplyByX(std::vector<uint8_t> &fx)
{
    std::vector<uint8_t> rx;
    rx.assign(this->mB, 0);
    uint8_t top = this->fx.front();
    uint8_t mask = 1;
    mask = mask << 7;
    uint8_t msb, prevmsb = 0;
    uint8_t section;
    uint i;
    for (i = m - 1; i >= 0; i--)
    {
        // Get the current 8 bits
        section = fx.at(i);
        // Get the msb of the current section
        msb = section & mask;
        // Perform 1 bit displacement and append previous msb
        section = section << 1;
        section = section | prevmsb;
        prevmsb = msb >> 7;
        rx.at(i) = section;
    }

    // The msb is 1
    if (top & mask)
   {
        // Reduce the resulting polynomial modulo m(x)
        for (i = m - 1; i >= 0; i--)
        {
            rx.at(i) = rx.at(i) ^ this->mx.at(i);
        } 
   }

   return rx;
}

GF2M GF2M::operator+(GF2M &gx)
{
    std::vector<uint8_t> rx;
    rx.assign(this->m , 0);
    for (int i = 0; i < this->m; i++)
    {
        r[i] = this->fx ^ gx->fx; 
    }

    return GF2M(this->m, this->mx, rx);
}

GF2M GF2M::operator-(GF2M &gx)
{
    // Substraction and adition are the same modulo 2
    return operator+(gx);
}

GF2M GF2M::operator*(GF2M &gx)
{
    std::vector< std::vector<uint8_t> > rx;
    uint8_t section;
    uint8_t mask = 1;
    // Operate on the eight first bits, initially
    section = this->fx.back();
    std::vector<uint8_t> fpx;
    fpx.assign(this->mB, 0);
    if (mask & section)
    {
        rx.push_back(this->fx);
    }
    else
    {
        rx.push_back(fpx);
    }
    uint i, j;
    for (i = 1; i < 8; i++)
    {
        mask = mask << 1;
        if (section & mask)
        {
            fpx = fx;
            for (j = 1; j < i; j++)
            {
                fpx = multiplyByX(fpx);
            }
        }
    }

    for (i = mB - 2; i >= 0; i--)
    {
        mask = mask << 1;
        if (section & mask)
        {
            fpx = fx;
            for (j = 1; j < i; j++)
            {
                fpx = multiplyByX(fpx);
            }
        }
    }  
}