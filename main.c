/*
DC 2020 HOMEWORK #4
THIS CODE WAS IMPLEMENTED BY IM SEUNGWOO, 15010909
macOS 10.15.5, 64-bit, XCode C Compiler
 */
#include <stdio.h>
#include <stdlib.h>

#define poly 0x7 // Polynomial is x8+x2+x1+1,
// which is 0000 0000 0000 0001 0000 0111

#define PRINT_DEBUG_OPT 1

// Storage to hold input data in Hexadecimal
unsigned char input_val[5] = {0x00, 0x00, 0x00, 0x00, 0x00}; // input value is an unsigned 32-bit integer

// a flag to check crc was made
int is_crc_made = 0;

// IMPLEMENTATION WITHOUT CRC TABLE
unsigned char cal_crc(const unsigned char *input_data, const int dat_len) {
    unsigned char crc8_val = 0x00; //Initial CRC8 Value is 0x00
    int i, j;

    for (i = 0; i < dat_len + 1; i++) {
        crc8_val ^= input_data[i];

        for (j = 0; j < 8; j++) {
            // Calc XOR Only when MSB is bit 1, otherwize, just SHIFT it
            crc8_val = (crc8_val & 0x80) != 0 ? (unsigned char) ((crc8_val << 1) ^ poly) : crc8_val << 1;
        }
    }
    return crc8_val;
}

void get_val(void) {
    unsigned int val = 0;

    printf("PLEASE ENTER INPUT DATA IN 32 bit HEXADECIMAL\n");
    scanf("%x", &val);

    // Value Check, if input value is larger than 0xFFFFFFFF, make error
    if (val > 0xFFFFFFFF) {
        printf("Input Value is too Large to calculate\n");
        exit(1);
    }

    if (PRINT_DEBUG_OPT) printf("input_val = %x \n", val);

    // DO NOT TOUCH LAST CHAR POINT VAL FOR CRC
    // ex)... in Case of input value is 0xAABBCCDD
    for (int i = 0; i < 4; ++i) {
        input_val[i] = (unsigned char) (val >> (3 - i) * 8);
        // in case of input value is 0xAABBCCDD...
        // input_val[0] = 0xAA
        // input_val[1] = 0xBB
        // input_val[2] = 0xCC
        // input_val[3] = 0xDD
        // input_val[4] = 0x0, RESERVED for CRC-8 bit
    }
    is_crc_made = 1;
}

void flip_message(void) {
    int order;
    int start_val, flip_num, flip_val;
    int j, k;
    printf("TYPE ORDER HERE (0 ~ 39 or 99)... 99 means burst error mode\n");
    scanf("%d", &order);
    printf("order: %d\n", order);

    // Raise Error when this function called before
    if (!is_crc_made) {
        printf("CALL 'cal_crc' function FIRST!\n");
        exit(1);
    }

    if (order == 99) {
        printf("PLEASE TYPE START INDEX AND FLIP NUMS (A, B)\n");
        scanf("%d, %d", &start_val, &flip_num);

        for (int i = start_val; i < start_val + flip_num; i++) {
            j = i / 8;
            k = i % 8;
            if (PRINT_DEBUG_OPT) printf("%d, %d\n", j, k);
            input_val[j] ^= 1 << k; // Toggle bit!
        }
    } else if (order >= 0 && order < 40) {
        while (order--) {
            printf("PLEASE TYPE A FLIP INDEX YOU WANT\n");
            scanf("%d", &flip_val);

            if (PRINT_DEBUG_OPT) printf("flip val : %d\n", flip_val);
            j = flip_val / 8;
            k = flip_val % 8;
            if (PRINT_DEBUG_OPT) printf("%d, %d\n", j, k);
            input_val[j] ^= 1 << k; // Toggle bit!
        }
    } else {
        printf("End Flip Process without flipped bit(s)\n");
        return;
    } // End Flip Process without flipping bit
}

void flip_message_auto(const int s_idx, const int nums) {
    const int start_val = s_idx;
    const int flip_num = nums;

    for (int i = start_val; i < start_val + flip_num; i++) {
        const int j = i / 8;
        const int k = i % 8;
        input_val[j] ^= 1 << k; // Toggle bit!
    }
}

int main(void) {
    // Process Input Value then Store it into "input_val[]"
    get_val(); // Get Integer Value and then split it to 4 8bit characters
    const int end_idx = sizeof(input_val) / sizeof(unsigned char) - 1;

    const unsigned char crc8_val = cal_crc(input_val, 3);
    printf("CALCULATED CRC is 0x%.2x\n", crc8_val);

    input_val[end_idx] = crc8_val; //Insert CRC-8 bit into RESERVED Storage

    // print CRC_appended Message
    for (int i = 0; i <= end_idx; i++) {
        if (i == 0) printf("The CRC-appended Message is 0x");
        printf("%x", input_val[i]);
        if (i == end_idx) printf("\n");
    }
    unsigned char crc_check = cal_crc(input_val, end_idx);

    if (PRINT_DEBUG_OPT) {
        printf("LAST INDEX : %d \n", end_idx);
        printf("DATA IS LOSSLESS? : %x \n", crc_check);
    }

    for (int i = 0; i < 4; i++) {
        if (PRINT_DEBUG_OPT) {
            printf("%d Th Value : %x \n", i, input_val[i]);
        }
    }

    flip_message();
    if (PRINT_DEBUG_OPT) {
        for (int i = 4; i >= 0; i--) {
            printf("%x \n", input_val[i]);
        }
    }

    // print CRC_appended Message with Noise
    for (int i = 0; i <= end_idx; i++) {
        if (i == 0) printf("The Noised CRC-appended Message is 0x");
        printf("%x", input_val[i]);

        if (i == end_idx) printf("\n");
    }

    crc_check = cal_crc(input_val, end_idx);
    if (PRINT_DEBUG_OPT) {
        printf("is correct? %d \n", crc_check);
    }

    if (!crc_check) {
        printf("CRC CHECK SUCCESSFUL!, CRC Remainder : 0x%.2x\n", crc_check);
    } else {
        printf("CRC CHECK FAIL!, CRC Remainder : 0x%.2x\n", crc_check);
    }

    return 0;
}

