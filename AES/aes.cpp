#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include "aes.hpp"

AES_128::AES_128()
{
    N = 4;
    R = 44;
    sk = std::vector<uint32_t> (N, 0);
    state = std::vector<std::vector<uint8_t>> (4, std::vector<uint8_t>(4, 0));
    round_keys = std::vector<uint32_t> (R, 0);
}

void AES_128::generate_sk()
{
    srand(time(0));
    for (int i = 0; i < N; i++) {
        sk[i] = (uint32_t)(rand());
    }
}

std::vector<uint8_t> AES_128::encrypt(std::vector<uint8_t> &m)
{
    // Initialize the state with the message
    if (m.size() != 16) {
        return {};
    }
    for (int i = 0; i < m.size(); i++) {
        state[(int)(i / 4)][i % 4] = m[i];
    }

    key_expansion();
    add_round_key(0);
    for (int i = 1; i <= 10; i++) {
        sub_bytes();
        shift_rows();
        if ( i < 10) {
            mix_columns();
        }
        add_round_key(i);
    }

    std::vector<uint8_t> c;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c.push_back(state[i][j]);
        }
    }
    return c;
}

std::vector<uint8_t> AES_128::decrypt(std::vector<uint8_t> &c)
{
    // Initialize the state with the encripted message
    if (c.size() != 16) {
        return {};
    }
    for (int i = 0; i < c.size(); i++) {
        state[(int)(i / 4)][i % 4] = c[i];
    }
    // No key expansion phase
    add_round_key(10);
    for (int i = 9; i > 0; i--) {
        shift_rows_inv();
        sub_bytes_inv();
        add_round_key(i);
        mix_columns_inv();
    }
    shift_rows_inv();
    sub_bytes_inv();
    add_round_key(0);

    std::vector<uint8_t> m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.push_back(state[i][j]);
        }
    }

    return m;
}

void AES_128::key_expansion()
{
    for (int i = 0; i < N; i++) {
        round_keys[i] = sk[i];
    }
    for (int i = N; i < R; i++) {
        uint32_t w = round_keys[i - N], wi;
        if (i % N == 0) {
            wi = w ^ S(rot(w)) ^ rcon[(i / N) - 1];
        } else {
            wi = w ^ S(round_keys[i - 1]);
        }
        round_keys[i] = wi;
    }
}

void AES_128::add_round_key(int round)
{
    // Get the round key
    std::vector<std::vector<uint8_t>> key;
    int row = N * round;
    for (int i = row; i < row + 4; i++) {
        key.push_back({
            (uint8_t)((round_keys[i] & 0xff000000) >> 24),
            (uint8_t)((round_keys[i] & 0x00ff0000) >> 16),
            (uint8_t)((round_keys[i] & 0x0000ff00) >> 8),
            (uint8_t)(round_keys[i] & 0x000000ff)
        });
    }
    for (int i = 0; i < key.size(); i++) {
        for (int j = 0; j < key[i].size(); j++) {
            state[i][j] = state[i][j] ^ key[i][j];
        }
    }
}

void AES_128::sub_bytes()
{
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            state[i][j] = S_single(state[i][j]);
        }
    }
}

void AES_128::shift_rows()
{
    // Shift second row by 1 position to the left
    uint8_t w = state[1][0];
    for (int i = 0; i < state.size(); i++) {
        state[1][i] = state[1][i + 1];
    }
    state[1][3] = w;
    // Shift thrid row by 2 pos to the left
    uint8_t w2 = state[2][0];
    w = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = w2;
    state[2][3] = w;
    // Shift 4th row by 3 pos to the left
    w = state[3][3];
    for (int i = state.size() - 1; i > 0; i--) {
        state[3][i] = state[3][i - 1];
    }
    state[3][0] = w;
}

uint8_t GMul(uint8_t u, uint8_t v) {
    uint8_t p = 0;

    for (int i = 0; i < 8; ++i) {
        if (u & 0x01) {    //
            p ^= v;
        }

        int flag = (v & 0x80);
        v <<= 1;
        if (flag) {
            v ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
        }

        u >>= 1;
    }

    return p;
}

void AES_128::mix_columns()
{
    std::vector<std::vector<uint8_t>> M = {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}
    };
    std::vector<std::vector<uint8_t>> temp(4, std::vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = state[i][j];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = GMul(M[i][0], temp[0][j]) ^ GMul(M[i][1], temp[1][j])
                          ^ GMul(M[i][2], temp[2][j]) ^ GMul(M[i][3], temp[3][j]);
        }
    }
}

void AES_128::mix_columns_inv()
{
    std::vector<std::vector<uint8_t>> M = {
        {0x0E, 0x0B, 0x0D, 0x09},
        {0x09, 0x0E, 0x0B, 0x0D},
        {0x0D, 0x09, 0x0E, 0x0B},
        {0x0B, 0x0D, 0x09, 0x0E}
    };
    std::vector<std::vector<uint8_t>> temp(4, std::vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = state[i][j];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = GMul(M[i][0], temp[0][j]) ^ GMul(M[i][1], temp[1][j])
                          ^ GMul(M[i][2], temp[2][j]) ^ GMul(M[i][3], temp[3][j]);
        }
    }
}

void AES_128::shift_rows_inv()
{
    // Shift second row by 1 position to the right
    uint8_t w = state[1][3];
    for (int i = state.size() - 1; i > 0; i--) {
        state[1][i] = state[1][i - 1];
    }
    state[1][0] = w;
    // Shift thrid row by 2 pos to the right
    uint8_t w2 = state[2][0];
    w = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = w2;
    state[2][3] = w;
    // Shift 4th row by 3 pos to the right
    w = state[3][0];
    for (int i = 0; i < state.size() - 1; i++) {
        state[3][i] = state[3][i + 1];
    }
    state[3][3] = w;
}

void AES_128::sub_bytes_inv()
{
    for (int i = 0; i < state.size(); i++) {
        for (int j = 0; j < state[i].size(); j++) {
            state[i][j] = inv_S_single(state[i][j]);
        }
    }
}

uint8_t AES_128::S_single(uint8_t c)
{
    return s_box[(c & 0xf0) >> 4][c & 0x0f];
}

uint8_t AES_128::inv_S_single(uint8_t c)
{
    return inv_s_box[(c & 0xf0) >> 4][c & 0x0f];
}

uint32_t AES_128::S(uint32_t word)
{
    std::vector<uint8_t> w = {
        (uint8_t)((word & 0xff000000) >> 24),
        (uint8_t)((word & 0x00ff0000) >> 16),
        (uint8_t)((word & 0x0000ff00) >> 8),
        (uint8_t)(word & 0x000000ff)
    };
    for (int i = 0; i < w.size(); i++) {
        w[i] = S_single(w[i]);
    }
    return ((uint32_t)w[0] << 24) | ((uint32_t)w[1] << 16) | ((uint32_t)w[2] << 8) | (uint32_t)w[3];;
}

uint32_t AES_128::S_inv(uint32_t word)
{
    std::vector<uint8_t> w = {
        (uint8_t)((word & 0xff000000) >> 24),
        (uint8_t)((word & 0x00ff0000) >> 16),
        (uint8_t)((word & 0x0000ff00) >> 8),
        (uint8_t)(word & 0x000000ff)
    };
    for (int i = 0; i < w.size(); i++) {
        w[i] = inv_S_single(w[i]);
    }
    return ((uint32_t)w[0] << 24) | ((uint32_t)w[1] << 16) | ((uint32_t)w[2] << 8) | (uint32_t)w[3];
}

uint32_t AES_128::rot(uint32_t word)
{
    std::vector<uint8_t> w = {
        (uint8_t)((word & 0xff000000) >> 24),
        (uint8_t)((word & 0x00ff0000) >> 16),
        (uint8_t)((word & 0x0000ff00) >> 8),
        (uint8_t)(word & 0x000000ff)
    };
    uint8_t f = w.front();
    for (int i = 0; i < w.size() - 1; i++) {
        w[i] = w[i + 1];
    }
    w[w.size() - 1] = f;
    return ((uint32_t)w[0] << 24) | ((uint32_t)w[1] << 16) | ((uint32_t)w[2] << 8) | (uint32_t)w[3];
}
