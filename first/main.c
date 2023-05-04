#include "header.h"


int main(int argv, char **argc)
{
    if (argc[1])
    {
        printf("Входное число: %s\n", argc[1]);
        const char test_compress[] = "test.txt"; 
        FILE *test = fopen("test.txt", "wb");
        uint32_t code_point;
        uint8_t buf_test[4];
        uint32_t code;
        sscanf(argc[1], "%d", &code_point);
        int len = encode_varint(code_point, buf_test);
        fwrite(buf_test, sizeof(uint8_t), len, test);

        uint32_t *com_buff = read_compress(test_compress);
        int i = 0;
        printf("%d\n", com_buff[i]);
        fclose(test);
    }
    else{
        const char compress[] = "compressed.dat"; 
        const char uncompress[] = "uncompressed.dat";

        writing_numbers_in_files(uncompress, compress, MaxNumbers);
        uint32_t *uncom_buff = read_uncompress(uncompress);
        printf("read uncompress.txt: ");

        for (int i = 0; i < 1; i++)
            printf("%d ", uncom_buff[i]);
        printf("\n\n");

        uint32_t *com_buff = read_compress(compress);

        printf("read compress.txt: ");
        for (int i = 0; i < 1; i++)
            printf("%d ", com_buff[i]);
        printf("\n\n");

        for (int i = 0; i < MaxNumbers; i++)
            if (uncom_buff[i] != com_buff[i]) {
                printf("Sequences of numbers from two files do not match.\n");
                return 1;
            }
        printf( "Sequences of numbers from two files match.\n");
        free(uncom_buff);
        free(com_buff);
    }

    return 0;
}