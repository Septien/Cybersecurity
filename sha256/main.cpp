#include <iostream>
#include <vector>
#include <string>

using namespace std;

/* Right-rotates x by n position*/
unsigned int rightrotate(unsigned int x, int n)
{
    unsigned int y = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        y = 0x0001 & x;
        y = y << 30;
        x = x >> 1;
        x = y | x;
    }
    return x;
}

/*An implemantation of the sha-256 algorithm, following the wikipedia entrance. */
vector<unsigned int> sha256(string msg)
{
    // Initialize the hash values (first 32 bits of the fractional parts of the
    // square roots of the first 8 primes 2..19)
    unsigned int h0, h1, h2, h3, h4, h5, h6, h7;
    h0 = 0x6a09e667;
    h1 = 0xbb67ae85;
    h2 = 0x3c6ef372;
    h3 = 0xa54ff53a;
    h4 = 0x510e527f;
    h5 = 0x9b05688c;
    h6 = 0x1f83d9ab;
    h7 = 0x5be0cd19;

    // Initialize array of round constants (first 32 bits of the fractional parts of
    // the cube roots of the first 64 primes 2..311)
    vector<unsigned int> k = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                              0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                              0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                              0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                              0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                              0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                              0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                              0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

    /// Make the size of the message a multiple of 512 by appeding zeros to it.
    // Get the number of bits on the message
    unsigned long long int L = msg.length() * sizeof(char) * 8;
    int l = L % 512; // Get the residuo of l modulo 512
    int K = 512 - 64 - l;
    uint8_t c = 0x80;
    string msg512 = msg;
    msg512 += c;
    K -= 8;
    const uint8_t tZero = 0x00;
    for (; K > 0; K -= 8)
    {
        msg512 += tZero;
    }
    int i, j;
    // Append L as a 64 unsigned integer
    unsigned long long int f = 0x00000000000000FF;
    for (i = 0; i < 8; i++)
    {
        c = ((L >> (i * 8)) & f);
        msg512 += c;
    }

    string submsg;
    vector<unsigned int> w;
    unsigned int s0, s1;
    for (i = 0; i < msg512.length(); i += 64)
    {
        // Split in chunks of 512 bits (64 bytes)
        submsg = msg512.substr(i, i + 64);
        // Init w to zero
        w.assign(64, 0);
        // Save the chunk on the first 16 positions
        for (j = 0; j < 16; j++)
        {
            char m = submsg[j * 4];
            w[j] = m;
            m = submsg[(j * 4) + 1];
            w[j] = w[j] << 8;
            w[j] = w[j] | m;
            m = submsg[(j * 4) + 2];
            w[j] = w[j] << 8;
            w[j] = w[j] | m;
            m = submsg[(j * 4) + 3];
            w[j] = w[j] << 8;
            w[j] = w[j] | m;
        }

        // Extend the first 16 words to the remaining 48 words w[16..63] of the message schedule array
        for (j = 16; j < 64; j++)
        {
            s0 = rightrotate(w[j - 15], 7) ^ rightrotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
            s1 = rightrotate(w[j - 2], 17) ^ rightrotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        // Initialize working variables to current hash value;
        unsigned int a, b, c, d, e, f, g, h;
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        f = h5;
        g = h6;
        h = h7;
        
        // Compression function main loop
        unsigned int S1, ch, temp1, S0, maj, temp2;
        for (i = 0; i < 64; i++)
        {
            S1 = rightrotate(e, 7) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
            ch = (e & f) ^  (( ~e ) & g);
            temp1 = h + S1 + ch + k[i] + w[i];
            S0 = rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
            maj = (a & b) ^ (a & c) ^ (b & c);
            temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
        h5 = h5 + f;
        h6 = h6 + g;
        h7 = h7 + h;
    }

    // Add the compressed chunk to the current hash value
    vector<unsigned int> v{h0, h1, h2, h3, h4, h5, h6, h7};
    return v;
}

int main(int argc, char **argv)
{
    string msg = "The quick brown fox jumps over the lazy dog.";
    cout << "The message is:\n" << msg << endl;
    vector<unsigned int> v = sha256(msg);
    cout << "The encrypted message is:\n0x";
    for (int i = 0; i < 8; i++)
    {
        cout << std::hex << v[i];
    }  
    cout << endl;
    return 0;
}
