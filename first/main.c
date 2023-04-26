#include "header.h"


int main(int argv, char **argc)
{
    
    const char compress[] = "compressed.dat"; 
    const char uncompress[] = "uncompressed.dat";

    writing_numbers_in_files(uncompress, compress, MaxNumbers);
    uint32_t *uncom_buff = read_uncompress(uncompress);
    printf("read uncompress.txt: ");

    for (int i = 0; i < 4; i++)
        printf("%d ", uncom_buff[i]);
    printf("\n\n");

    uint32_t *com_buff = read_compress(compress);

    printf("read compress.txt: ");
    for (int i = 0; i < 4; i++)
        printf("%d ", com_buff[i]);
    printf("\n\n");
    return 0;
}