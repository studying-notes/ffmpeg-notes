#include <stdio.h>
#include "libavcodec/avcodec.h"

int main() {
    printf("%s\n", avcodec_configuration());
    printf("Hello, World!\n");
    return 0;
}
