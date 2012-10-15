
#include "lcs.h"


int main(int argc, char **argv)
{
    if(argc != 3) {
        printf("usage:: ./main filename_1 filename_2\n");
        return 0;
    }
    lcsdiff(argv[1], argv[2]);
    return 0;
}

