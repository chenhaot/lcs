
#include "lcs.h"


int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("usage:: ./main filename_1 filename_2\n");
        return 0;
    }
    if (argc == 2)
      lcsdiff(argv[1]);
    else
      lcsdiff(argv[1], argv[2]);
    return 0;
}

