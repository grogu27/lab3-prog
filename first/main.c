#include "header.h"


int main(int argv, char **argc)
{
    
    const char *compress = "compressed.dat"; 
    const char *uncompress = "uncompressed.dat";

    writing_numbers(uncompress, compress, MaxNumbers);
    uint32_t *uncom_buff = read_uncompress(uncompress);
    //uint32_t *com_buff = read_compress(compress);

    return 0;
}