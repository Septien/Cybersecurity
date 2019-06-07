#include <iostream>
#include <vector>
#include <math.h>
#include "rsa_cryptosys.hpp"

using namespace std;

int main()
{
    RSA rsa = RSA();
    unsigned long int p = 3391;
    unsigned long int q = 2671;
    rsa.computeN(p, q);
    rsa.computeE(p, q);
    rsa.computeD(p, q);
    vector<unsigned long int> pK = rsa.getPublicKey();
    vector<unsigned long int> sK = rsa.getSecretKey();

    cout << "Para los numeros primos: " << p << " " << q << ".\n";
    cout << "La llave publica es: " << pK[0] << ", " << pK[1] << endl;
    cout << "La llave privada es: " << sK[0] << ", " << sK[1] << endl;
    //unsigned long int m = 42;
    vector<unsigned long int> m{6, 7, 42};
    cout << "Mensaje a encriptar: " << m[0] << " " << m[1] << " " << m[2] << endl;
    cout << "Mensaje encriptado: ";
    /*unsigned long int c = 0;
    c = rsa.encrypt(m);
    cout << c << endl;
    cout << "Mensaje descencriptado: ";
    unsigned long int mp = rsa.decrypt(c);
    cout << mp << endl;*/
    vector<unsigned long int> c;
    for (unsigned int i = 0; i < m.size(); i++)
    {
        c.push_back(rsa.encrypt(m[i]));
        cout << c.back() << " ";
    }
    cout << "\nMensaje descenciptado: ";
    vector<unsigned long int> mp;
    for (unsigned int i = 0; i < c.size(); i++)
    {
        unsigned long int cm = rsa.decrypt(c[i]);
        mp.push_back(cm);
        cout << mp.back() << " ";
    }
    cout << endl;
    return 0;
}
