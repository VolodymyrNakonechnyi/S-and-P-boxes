
#include <iostream>
#include <cstddef>
#include <bitset>

int* invertSBox(const int sBox[]) {
    const int length = 16;
    int* invertedSBox = new int[length];

    for (int i = 0; i < length; ++i) {
        invertedSBox[sBox[i]] = i;
    }

    return invertedSBox;
}



bool is8BitNumber(int num) {
    int mask = 0b11111111;

    return (num & mask) == num;
}

int* splitBinary(int num) {
    static int result[2];

    result[0] = num & 0b00001111;

    result[1] = (num >> 4) & 0b00001111;

    return result;
}

int sBlock_8bit(int message, int sBox[]) {
    if (!is8BitNumber(message)) {
        return -1;
    }

    int* resultArray = splitBinary(message);
    int substituted[2];

    for (int i = 0; i < 2; i++) {
        substituted[i] = sBox[resultArray[i]];
    }

    substituted[0] &= 0b00001111;
    substituted[1] &= 0b00001111;

    int result = (substituted[1] << 4) | substituted[0];
    return result;
}

int pBlock_Straight_8bit(int message, int pBox[]) {
    if (!is8BitNumber(message)) {
        return -1;
    }
    int result = 0;

    for (int i = 0; i < 8; i++) {
        int bitPosition = 8 - pBox[i];
        int bitValue = (message >> (bitPosition - 1)) & 1;
        result = (result << 1) | bitValue;
    }

    return result;
}


int encryption(int message, int sBox[], int pBox[]) {
    return pBlock_Straight_8bit(sBlock_8bit(message, sBox), pBox);
}

int decryption(int encodeMessage, int invsBox[], int pBox[]) {
    return sBlock_8bit(pBlock_Straight_8bit(encodeMessage, pBox), invsBox);
}






/*
void invertSBox(int sBox[], int invertedSBox[]) {
    for (int i = 0; i < 16; ++i) {
        int value = sBox[i];
        invertedSBox[value] = i;
    }
}

void invertPBox(int pBox[], int invertedPBox[]) {
    for (int i = 0; i < 8; ++i) {
        int value = pBox[i];
        invertedPBox[value - 1] = i + 1; // P-Box values are 1-indexed
    }
}
*/




int main()
{
    int num = 0b10101011;
    
    //int key = 0b10110101;

    int sBox[16] = { 6, 12, 10, 5, 8, 3, 15, 0, 14, 7, 13, 1, 2, 9, 4, 11 };
    int pBox[8] = { 6, 3, 2, 4, 8, 5, 7, 1 };


    int encrypted_num = encryption(num, sBox, pBox);
    std::cout << std::bitset<8>(encrypted_num) << std:: endl;

    int* invsBox = invertSBox(sBox);

    int decrypted_num = decryption(encrypted_num, invsBox, pBox);
    std::cout << std::bitset<8>(decrypted_num);

    return 0;
}

