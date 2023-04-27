#include <stdio.h>
#include <inttypes.h>
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name) 
{
    FILE *in_file = fopen(in_file_name, "rb");
    FILE *out_file = fopen(out_file_name, "wb");
    if (!in_file || !out_file)
        return -1;
    
    uint32_t code_point;
    CodeUnit code_unit;
    while (fscanf(in_file, "%" SCNx32, &code_point) != EOF)
    {
        printf("%" PRIx32, code_point);
        printf("\n");
        if (encode(code_point, &code_unit) < 0)
        {
        printf("Error encode\n");
        return -1;
        }

        write_code_unit(out_file, &code_unit);
        // printf("%x\n", decode(&code_unit));
        // break;
    }
    
    
   
    
    fclose(in_file);
    fclose(out_file);
    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in_file = fopen(in_file_name, "rb");
    FILE *out_file = fopen(out_file_name, "wb");
    if (!in_file || !out_file)
        return -1;

    uint32_t code_point;
    CodeUnit code_unit;

    while (read_code_unit(in_file, &code_unit) != EOF) {
    code_point = decode(&code_unit);
    if (code_point == -1)
      continue;
    printf("%" PRIx32, code_point);
      printf("\n");
    write_code_unit(out_file, &code_unit);

    }
    fclose(in_file);
    fclose(out_file);
    return 0;
}

int test_file(const char *str, const char *out_file_name) {
  FILE *out_file = fopen(out_file_name, "ab");
  if (!out_file)
    return -1;
  uint32_t code_point;
  CodeUnit code_unit;
  sscanf(str, "%" SCNx32, &code_point);
  encode(code_point, &code_unit);
  fwrite(code_unit.code, sizeof(uint8_t), code_unit.len, out_file);
  fclose(out_file);
  return 0;
}



