#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef unsigned long u64;
typedef unsigned char u8;

void print_byte(u8 data) {
    u8 buf[8] = { 0 };
    for (int i = 0; i < 8; i++) {
        buf[7 - i] = (data >> i) & 0x1;
    }

    for (int i = 0; i < 8; i++) {
        printf("%d", buf[i]);
    }
    printf("\n");
}


u8* encode(u64 data) {
    (void)data;
    return NULL;
}

u64 decode(u8* arr) {
    // We get an array of bytes that we don't know the range
    // OMEGA DANGERIOOOOOOS
    u64 ret = 0;
    u8 offset = 0;
    u8 continue_bit = 1;
    for (int i = 0; continue_bit; i++) {
        continue_bit = arr[i] >> 7;
        // drop the continuation bit
        arr[i] &= 0b01111111;
        u64 y = arr[i];
        ret |= y << offset;
        offset += 7;
        if (continue_bit == 0) break;
    }

    return ret;
}

int main() {
    u8 arr[] = {0x96, 0x01};
    printf("%zu\n", decode(arr));
}
