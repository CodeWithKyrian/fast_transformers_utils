#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    // Example parameters
    int frameLength = 256;
    int fftLength = 256;
    int hopLength = 128;
    int numFrames = 10;
    int numFrequencyBins = fftLength / 2 + 1;

    return 0;
}

// cmake -S . -B build
// cmake --build build
// (cd build; cpack -C Release)  
