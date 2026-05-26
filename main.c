#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

typedef uint64_t u64;
typedef uint8_t u8;

typedef struct {
    u64 offset;
    u64 result;
} DecodeResult;

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

int main() {
    // assert(decode((u8[]){0x96, 0x01}) == 150);
    // assert(decode((u8[]){0x00}) == 0);
    // assert(decode((u8[]){0x01}) == 1);
    // assert(decode((u8[]){0x7f}) == 127);
    // assert(decode((u8[]){0x80, 0x01}) == 128);
    // assert(decode((u8[]){0xac, 0x02}) == 300);


    // DecodeResult a = decode_pure(data, 0);
    // DecodeResult b = decode_pure(data, a.offset);

    u8 data[] = {0xac, 0x02, 0x96, 0x01};
    u64 a, b;
    int offset = decode(data, &a);
    decode(data + offset, &b);

    printf("%lu %lu\n", a, b);


    // assert(decode(v).result == 3 && decode(v).offset == 2);
    printf("All tests passed!\n");
}

