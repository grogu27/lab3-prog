#include "header.h"


int main(int argv, char **argc)
{
    if (argc[1])
    {
        FILE *test = fopen("test.txt", "wb");
        uint32_t code_point;
        uint8_t buf_test[4];
        sscanf(argc[1], "%d", &code_point);
        int len = encode_varint(code_point, buf_test);
        fwrite(buf_test, sizeof(uint8_t), len, test);
        fclose(test);
    }
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