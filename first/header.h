#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

enum {
    MaxCodeLength = 4,
    MaxNumbers = 1000000
};
void print_bin(unsigned char x);
size_t encode_varint(uint32_t value, uint8_t *buf);
uint32_t decode_varint(const uint8_t **bufp);
uint32_t generate_number();
void writing_numbers_in_files(const char *uncompress_path, const char *compress_path, uint32_t count);
uint32_t *read_uncompress(const char *uncompress_path);
uint32_t *read_compress(const char *compress_path);