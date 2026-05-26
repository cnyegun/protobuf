#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>

typedef uint64_t u64;
typedef uint8_t u8;

typedef struct {
    u64 offset;
    u64 result;
} DecodeResult;

void print_u8_binary(u8 data) {
    u8 buf[8] = {0};
    for (int i = 0; i < 8; i++) {
        buf[7 - i] = (data >> i) & 0x1;
    }

    for (int i = 0; i < 8; i++) {
        printf("%d", buf[i]);
    }
}

void print_u64_binary(u64 data) {
    for (int i = 63; i >= 0; i--) {
        putchar((data >> i) & 0x01 ? '1' : '0');
    }
    putchar('\n');
}

int encode(u64 src, u8* dst, u64 len) {
    int bytes_written = 0;
    u8 buf[10] = {0};

    // print_u64_binary(src);
    for (int i = 0; src != 0; i++) {
        buf[i] = src & 0b01111111 ;
        bytes_written++;
        // print_u8_binary(buf[i]);
        // printf("\n");
        src >>= 7;
    }
    // printf("bytes_written: %d\n", bytes_written);

    for (int i = 0; i < bytes_written - 1; i++) {
        buf[i] |= 0b10000000;
    }

    // for (int i = 0; i < 10; i++) {
    //     print_u8_binary(buf[i]);
    // }
    // printf("\n");
    //
    if ((u64)bytes_written <= len) {
        memcpy(dst, buf, 10);
    }
    return bytes_written;
}

DecodeResult decode_pure(const u8* src, u64 offset) {
    u64 result = 0;
    u8 bit_shift = 0;
    u64 bytes_consumed = 0;
    const u8* arr = src + offset;

    for (int i = 0; i < 10; i++) {
        u8 continue_bit = arr[i] >> 7;
        // drop the continuation bit
        u64 tmp = arr[i];
        tmp &= 0b01111111;
        result |= tmp << bit_shift;
        bit_shift += 7;
        bytes_consumed++;
        if (continue_bit == 0) break;
    }
    return (DecodeResult){offset + bytes_consumed, result};
}

int decode(const u8* src, u64* dst) {
    int bytes_consumed = 0;
    u8 bit_shift = 0;
    u64 result = 0;

    for (int i = 0; i < 10; i++) {
        u8 continue_bit = src[i] >> 7;

        u64 tmp = src[i] & 0b01111111;
        result |= tmp << bit_shift;
        bit_shift += 7;
        bytes_consumed++;

        if (continue_bit == 0) break;
    }

    *dst = result;
    return bytes_consumed;
}

void test_decode_encode(u64 x) {
    u8 arr[10] = {0};
    u64 result = 0;
    encode(x, arr, 10);
    decode(arr, &result);
    //printf("assert(%ld == %ld)\n", x, result);
    assert(x == result);
}

int main() {
    for (u64 i = 0; i < 1000000; i++) {
        //printf("run %ld\n", i);
        test_decode_encode(i);
    }

    // u8 buf[10] = {0};
    // encode(129, buf, 10);
    //
    // printf("\n");
    // u64 x;
    // decode(buf, &x);
    // print_u64_binary(x);
    // printf("%ld\n", x);

    printf("All tests passed!\n");
}
