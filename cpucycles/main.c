/*
*   Count the number of cpu cycles a piece of code takes to execute, using asm primitives.
*   It works both on linux and unix.
*   Reference: https://stackoverflow.com/questions/13772567/how-to-get-the-cpu-cycle-count-in-x86-64-from-c
*/

#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

uint64_t rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t) hi << 32 | lo);
}

int main(int argc, char **argv)
{
    int i, count = 0;
    uint64_t first, last;
    first = rdtsc();
    printf("Counting...\n");
    for (i = 0; i < 1000; i++)
    {
        count++;
    }
    last = rdtsc();
    printf("Counts: \n%llu, %llu\n", first, last);
    printf("Difference: %llu", last - first);
    return 0;
}
