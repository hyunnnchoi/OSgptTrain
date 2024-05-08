#include <stdio.h>
#include "train-load.h"
int main() {
    initializeSemaphores();
    Load();
    Train();

    return 0;
}
