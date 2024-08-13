#include <stdlib.h>
#include "fast_transformers_utils.h"

static int calculate_reflect_offset(int i, int w)
{
    return abs((i + w) % (2 * w) - w);
}

TRANSFORMERS_API void pad_reflect(float *input, int length, float *padded, int padded_length)
{
    int left, right;

    left = right = (padded_length - length) / 2;

    int w = length - 1;

    // Copy original array into the middle of the padded array
    for (int i = 0; i < length; ++i)
    {
        padded[left + i] = input[i];
    }

    // Add reflected values to the left
    for (int i = 1; i <= left; ++i)
    {
        padded[left - i] = input[calculate_reflect_offset(i, w)];
    }

    // Add reflected values to the right
    for (int i = 1; i <= right; ++i)
    {
        padded[w + left + i] = input[calculate_reflect_offset(w - i, w)];
    }
}