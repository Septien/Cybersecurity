#include <vector>
#include <iostream>
#include <cstdio>
#include "aes.hpp"

using namespace std;

int main(void) {
    AES_128 aes;

    aes.generate_sk();
    vector<uint8_t> m = {
        0X01, 0X02, 0X03, 0X04, 0X05, 0X06, 0X07, 0X08, 0X09, 0X0A, 0X0B, 0X0C, 0X10, 0X20, 0X30, 0X40
    };
    cout << "Texto plano:\n";
    for (int i = 0; i < m.size(); i++) {
        printf("%.2x", m[i]);
    }
    cout << endl;
    vector<uint8_t> c = aes.encrypt(m);
    cout << "Texto encriptado:\n";
    for (int i = 0; i < c.size(); i++) {
        printf("%.2x", c[i]);
    }
    cout << endl;
    vector<uint8_t> m2 = aes.decrypt(c);
    cout << "Texto desencriptado:\n";
    for (uint8_t _m : m2) {
        printf("%.2x", _m);
    }
    cout << endl;
    return 0;
}
