#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "rsa_cryptosys.hpp"

using namespace std;

int main()
{
    srand(time(NULL));
    RSA rsa = RSA();
    unsigned long int p = 5581;
    unsigned long int q = 4547;
    rsa.computeN(p, q);
    rsa.computeE(p, q);
    rsa.computeD(p, q);
    vector<unsigned long int> pK = rsa.getPublicKey();
    vector<unsigned long int> sK = rsa.getSecretKey();

    //
    cout << "Para los numeros primos: " << p << " " << q << ".\n";
    cout << "La llave publica es: " << pK[0] << ", " << pK[1] << endl;
    cout << "La llave privada es: " << sK[0] << ", " << sK[1] << endl;

    // Generar mensajes
    vector<unsigned long int> m;
    int n = 5;
    for (int i = 0; i < n; i++)
        m.push_back(rand()%10000000);

    // Despliega
    cout << "Mensaje a encriptar: ";
    for (int i = 0; i < n; i++)
        cout << m[i] << ", ";

    // Encripta y despliega
    cout << "\nMensaje encriptado: ";
    vector<unsigned long int> c;
    for (unsigned int i = 0; i < m.size(); i++)
    {
        c.push_back(rsa.encrypt(m[i]));
        cout << c.back() << ", ";
    }

    // Desencripta y despliega
    cout << "\nMensaje desencriptado: ";
    vector<unsigned long int> mp;
    for (unsigned int i = 0; i < c.size(); i++)
    {
        unsigned long int cm = rsa.decrypt(c[i]);
        mp.push_back(cm);
        cout << mp.back() << ", ";
    }
    cout << endl;
    return 0;
}
