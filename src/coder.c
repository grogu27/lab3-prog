#include <stdio.h>
#include <assert.h>
#include "coder.h"

int encode(uint32_t code_point, CodeUnit *code_unit)
{
    if (code_point < 0x80)     // 7 значащих бит < 0x80 = 1000 0000
    {
        code_unit->len = 1;    // 1 байт
        code_unit->code[0] = code_point; 
    }
    else if (code_point < 0x800)    // 11 значащих бит < 0x800 = 1000 0000 0000
    {
        code_unit->len = 2;    // 2 байт
        code_unit->code[0] = 0xC0 | (code_point >> 6);   // 0xC0 -> 1100 0000
        code_unit->code[1] = 0x80 | (code_point & 0x3F); // 0x3F -> 0011 1111
    }
    else if (code_point < 0x10000)   // 16 значащих бит < 0x10000 -> 1000 0000 0000
    {
        code_unit->len = 3;   // 3 байта
        code_unit->code[0] = 0xE0 | (code_point >> 12); // 0xE0 -> 11100000
        code_unit->code[1] = 0x80 | ((code_point >> 6) & 0x3F);
        code_unit->code[2] = 0x80 | (code_point & 0x3F);
    }
    else if (code_point < 0x200000)  // 21 значащих бит < 0x200000 -> 10 0000 0000 0000 0000 0000
    {
        code_unit->len = 4;
        code_unit->code[0] = 0xF0 | (code_point >> 18); // 0xF0 -> 11110000
        code_unit->code[1] = 0x80 | ((code_point >> 12) & 0x3F);
        code_unit->code[2] = 0x80 | ((code_point >> 6) & 0x3F);
        code_unit->code[3] = 0x80 | (code_point & 0x3F);
    }
    else 
        return -1;
    return 0;
}


int write_code_unit(FILE *out, const CodeUnit *code_unit)
{
    int res = fwrite(code_unit->code, sizeof(uint8_t), code_unit->len, out);
    return res;
}

int get_byte_type(uint8_t byte) {
  int type;
  if ((byte >> 7) == 0) // 0xxxxxxx
    type = 0;
  else if (((byte >> 6) & 1) == 0) // 10xxxxxx
    type = 1;
  else if (((byte >> 6) & 1) == 1 && ((byte >> 5) & 1) == 0) // 110xxxxx
    type = 2;
  else if (((byte >> 6) & 1) == 1 && ((byte >> 5) & 1) == 1 &&
           ((byte >> 4) & 1) == 0) //  1110xxxx
    type = 3;
  else if (((byte >> 6) & 1) == 1 && ((byte >> 5) & 1) == 1 &&
           ((byte >> 4) & 1) == 1 && ((byte >> 3) & 1) == 0) // 11110xxx
    type = 4;
  else
    return -1;
  return type;
}

uint32_t decode(const CodeUnit *code_unit)
{
    int type = get_byte_type(code_unit->code[0]);
    uint32_t code_point = 0;
    if (type == 0)
        code_point = code_unit->code[0];
    else if (type == 2)
        code_point = ((code_unit->code[0] & 0x1F) << 6) |
                    (code_unit->code[1] & 0x3F); // 0x1F -> 11111 0x3F-> 111111
    else if (type == 3)
        code_point = ((code_unit->code[0] & 0x0F) << 12) |
                    ((code_unit->code[1] & 0x3F) << 6) | // 0x0F -> 1111
                    (code_unit->code[2] & 0x3F);
    else if (type == 4)
        code_point = ((code_unit->code[0] & 0x07) << 18) |
                    ((code_unit->code[1] & 0x3F) << 12) | // 0x07 -> 111
                    ((code_unit->code[2] & 0x3F) << 6) |
                    (code_unit->code[3] & 0x3F);
    else
        return -1;
    return code_point;
    
}

int read_next_code_unit(FILE *in, CodeUnit *code_unit)
{
    uint8_t byte;
    int read_success = 0;
    while (!read_success){
        fread(&byte, sizeof(uint8_t), 1, in);
        if (feof(in))
            return -1;
        int type = get_byte_type(byte);
        if (type == -1 || type == 1)
            continue;

        code_unit->code[0] = byte;
        code_unit->len = 1;
        read_success = 1;

        for (int i = 1; i < type; i++) {
            fread(&byte, sizeof(uint8_t), 1, in);
            if (feof(in))
                return -1;

            if (get_byte_type(byte) != 1){
                read_success = 0;
                break;
            }
            code_unit->code[i] = byte;
            code_unit->len += 1;
        }
  }
  return 0;
}
