#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>
using namespace std;

int pc1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

int pc2[48] = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int left_shifts[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

uint64_t apply_pc1(uint64_t key) {
    uint64_t result = 0;
    for (int i = 0; i < 56; i++) {
        int bit_position = pc1[i] - 1;
        result |= ((key >> (63 - bit_position)) & 1) << (55 - i);
    }
    return result;
}

uint64_t apply_pc2(uint64_t left, uint64_t right) {
    uint64_t combined = (left << 28) | right;
    uint64_t result = 0;
    for (int i = 0; i < 48; i++) {
        int bit_position = pc2[i] - 1;
        result |= ((combined >> (55 - bit_position)) & 1) << (47 - i);
    }
    return result;
}

uint64_t left_shift(uint64_t val, int shift_amount) {
    return ((val << shift_amount) | (val >> (28 - shift_amount))) & 0x0FFFFFFF;
}

vector<uint64_t> generate_subkeys(uint64_t key) {
    uint64_t key56 = apply_pc1(key);
    uint64_t left = (key56 >> 28) & 0x0FFFFFFF;
    uint64_t right = key56 & 0x0FFFFFFF;
    vector<uint64_t> subkeys;
    for (int i = 0; i < 16; i++) {
        left = left_shift(left, left_shifts[i]);
        right = left_shift(right, left_shifts[i]);
        subkeys.push_back(apply_pc2(left, right));
    }
    return subkeys;
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;
    vector<uint64_t> subkeys = generate_subkeys(key);
    for (int i = 0; i < subkeys.size(); i++) {
        cout << "Subkey " << i + 1 << ": " << bitset<48>(subkeys[i]) << endl;
    }
    return 0;
}
