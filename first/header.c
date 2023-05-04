#include "header.h"

void print_bin(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        printf("%d", (x >> (7 - i)) & 0x1);
    printf("\n");
}

size_t encode_varint(uint32_t value, uint8_t *buf) {
  assert(buf != NULL);
  uint8_t *cur = buf;
  while (value >= 0x80) {
    const uint8_t byte = (value & 0x7f) | 0x80;
    *cur = byte;
    value >>= 7;
    ++cur;
  }
  *cur = value;
  ++cur;
  return cur - buf;
}

uint32_t decode_varint(const uint8_t **bufp) {
  const uint8_t *cur = *bufp;
  uint8_t byte = *cur++;
  uint32_t value = byte & 0x7f;
  size_t shift = 7;
  while (byte >= 0x80) {
    byte = *cur++;
    value += (byte & 0x7f) << shift;
    shift += 7;
  }
  *bufp = cur;
  return value;
}

/*
 * Диапазон             Вероятность
 * -------------------- -----------
 * [0; 128)             90%
 * [128; 16384)         5%
 * [16384; 2097152)     4%
 * [2097152; 268435455) 1%
 */
uint32_t generate_number() {
  const int r = rand();
  const int p = r % 100;
  if (p < 90) {
    return r % 128;
  }
  if (p < 95) {
    return r % 16384;
  }
  if (p < 99) {
    return r % 2097152;
  }
  return r % 268435455;
}

void writing_numbers_in_files(const char *uncompress_path, const char *compress_path, uint32_t len)
{
    FILE *uncompress = fopen(uncompress_path, "wb");
    FILE *compress = fopen(compress_path, "wb");
    
    for (int i = 0; i < 1; i++)
    {
        uint8_t buf[MaxCodeLength];
        uint32_t num = generate_number();
        uint32_t num2 = 0xa5;
        size_t size = encode_varint(num2, buf);

        fwrite(&num2, sizeof(uint32_t), 1, uncompress);
        fwrite(buf, sizeof(uint8_t), size, compress);
    }
    fclose(compress);
    fclose(uncompress);
    
}

uint32_t *read_uncompress(const char *uncompress_path)
{
    FILE *uncompress = fopen(uncompress_path, "r");
    if (!uncompress)
      return NULL;
    
    fseek(uncompress, 0, SEEK_END);
    size_t size = ftell(uncompress);
    uint32_t *buf = malloc(size);

    if (!buf)
        return NULL;

    fseek(uncompress, 0, SEEK_SET);
    fread(buf, sizeof(uint32_t), size, uncompress);
  
    fclose(uncompress);
    printf("size uncompress.txt: %ld\n", size);
    return buf;
}

uint32_t *read_compress(const char *compress_path)
{
  FILE *compress = fopen(compress_path, "rb");
  if (!compress)
    return NULL;
  fseek(compress, 0, SEEK_END);
  size_t size = ftell(compress);
  fseek(compress, 0, SEEK_SET);

  uint8_t *buff = malloc(size);

  if (!buff)
    return NULL;

  const uint8_t *cur = buff;

  fread(buff, sizeof(uint8_t), size, compress);
  fclose(compress);
  uint32_t *result = malloc(size * sizeof(uint32_t));
  int i = 0;
  while (cur < buff + size)
  {
    result[i] = decode_varint(&cur);
    i++;
  }
  free(buff);
  printf("size compress.txt: %ld\n", size);
  return result;
}