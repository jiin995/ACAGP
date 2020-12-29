// Output from my cmake VERBOSE=1 command for building:
// c++ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native -g -O0 -std=c++0x -g -o sse.cpp.o -c sse.cpp
// c++ -march=native -g -O0 -std=c++0x -g sse.cpp.o -o sse -rdynamic -ldl -lpthread

// SSE
//
#include <stdio.h>
#include <stdlib.h>

// #include <mmintrin.h> // MMX
// #include <xmmintrin.h> // SSE
// #include <emmintrin.h> // SSE2
// #include <pmmintrin.h> // SSE3
// #include <tmmintrin.h> // SSSE3
// #include <nmmintrin.h> // SSE4.1
// #include <ammintrin.h> // SSE4.2
// #include <wmmintrin.h> // AES/PCMUL
// #include <immintrin.h> // AVX
#include <x86intrin.h>      // Pulls in all of the above based on compiler switches (-march)

// AVX, SSE intrinsics, etc.:
// http://chessprogramming.wikispaces.com/AVX

// Intrinsics for Advanced Vector Extensions:
// http://software.intel.com/sites/products/documentation/hpc/composerxe/en-us/2011Update/cpp/lin/intref_cls/common/intref_bk_advectorext.htm

// Intrinsics for Advanced Vector Extensions 2:
// http://software.intel.com/sites/products/documentation/hpc/composerxe/en-us/2011Update/cpp/lin/intref_cls/common/intref_bk_advectorext2.htm

#ifndef __AVX__
#error AVX not defined
#endif

int main( int argc, char *argv[] )
{
    float a = 16.0f;
    float b = 9.0f;

    __m128 SSE0 = _mm_setzero_ps();
    __m128 SSEa = _mm_set_ps1(a);   // _mm_load1_ps(&a);
    __m128 SSEb = _mm_set_ps1(b);   // _mm_load1_ps(&b);
    __m128 SSEv = _mm_add_ps(SSEa, SSEb);

    __m256 AVX0 = _mm256_setzero_ps();
    __m256 AVXa = _mm256_set1_ps(a);
    __m256 AVXb = _mm256_set1_ps(b);
    __m256 AVXv = _mm256_add_ps(AVXa, AVXb);

    __m64 MMX0 = _mm_setzero_si64();
    __m64 MMXa = _mm_setr_pi32(16, 16);
    __m64 MMXb = _mm_setr_pi32(9, 9);
    __m64 MMXv = _mm_add_pi32(MMXa, MMXb);

    float temp[4] __attribute__((aligned(16)));
    _mm_store_ps(&temp[0], SSEv);
    printf("tempsse is %.2f %.2f %.2f %.2f\n", temp[0], temp[1], temp[2], temp[3]);

    float temp2[8] __attribute((aligned(32)));
    _mm256_store_ps(&temp2[0], AVXv);
    printf("tempavx is %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        temp2[0], temp2[1], temp2[2], temp2[3],
        temp2[4], temp2[5], temp2[6], temp2[7]);

    printf("%d\n", _mm_cvtsi64_si32(MMXv));

    return 0;
}

