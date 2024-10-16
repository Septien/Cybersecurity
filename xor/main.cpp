#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <time.h>

using namespace std;

void fill_vector(vector<uint8_t> &v)
{
    for (int i = 0; i < v.size(); i++) {
        v[i] = rand();
    }
}

void print_vector(vector<uint8_t> &v)
{
    for (uint8_t e : v) {
        printf("%.2x", e);
    }
}

int main(void)
{
    srand(time(0));
    int N = 16;
    vector<uint8_t> sk(N, 0);
    vector<uint8_t> m(N, 0);
    vector<uint8_t> m2(N, 0);
    fill_vector(sk);
    fill_vector(m);

    // Encrypt
    vector<uint8_t> c(N, 0);
    for (int i = 0; i < N; i++) {
        c[i] = m[i] ^ sk[i];
    }

    // Decrypt
    for (int i = 0; i < N; i++) {
        m2[i] = c[i] ^ sk[i];
    }

    cout << "Clave:" << endl;
    print_vector(sk);
    cout << "\nTexto plano:" << endl;
    print_vector(m);
    cout << "\nTexto encriptado:" << endl;
    print_vector(c);
    cout << "\nTexto desencriptado:" << endl;
    print_vector(m2);
    cout << endl;

    return 0;
}