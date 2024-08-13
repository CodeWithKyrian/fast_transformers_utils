#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fast_transformers_utils.h"
#include "kiss_fft.h"
#include "kiss_fftr.h"



static void do_remove_dc_offset(float *buffer, int length)
{
    float sum = 0;
    for (int j = 0; j < length; ++j)
    {
        sum += buffer[j];
    }
    float mean = sum / length;
    for (int j = 0; j < length; ++j)
    {
        buffer[j] -= mean;
    }
}

static void apply_preemphasis(float *buffer, int length, float preemphasis)
{
    if (preemphasis > 0)
    {
        for (int j = length - 1; j >= 1; --j)
        {
            buffer[j] -= preemphasis * buffer[j - 1];
        }
        buffer[0] *= 1 - preemphasis;
    }
}

static void apply_window(float *buffer, int length, float *window)
{
    for (int j = 0; j < length; ++j)
    {
        buffer[j] *= window[j];
    }
}

TRANSFORMERS_API void spectrogram(
    float *waveform, int waveform_length, float *spectrogram, int spectrogram_length, int hop_length,
    int fft_length, float *window, int window_length, int d1, int d1_max, float power, int center,
    float preemphasis, float *mel_filters, int num_mel_filters, int num_frequency_bins, float mel_floor,
    int log_mel, int remove_dc_offset, int do_pad, int transpose)
{
    // Initialize KISS FFT
    kiss_fftr_cfg cfg = kiss_fftr_alloc(fft_length, 0, NULL, NULL);
    kiss_fft_cpx *output_buffer = (kiss_fft_cpx *)malloc((fft_length / 2 + 1) * sizeof(kiss_fft_cpx));
    float *input_buffer = (float *)malloc(fft_length * sizeof(float));
    float *magnitudes = (float *)malloc(d1 * num_frequency_bins * sizeof(float));

    for (int i = 0; i < d1; ++i)
    {
        int offset = i * hop_length;

        for (int j = 0; j < window_length; ++j)
        {
            input_buffer[j] = waveform[offset + j];
        }

        if (remove_dc_offset)
        {
            do_remove_dc_offset(input_buffer, window_length);
        }

        if (preemphasis > 0)
        {
            apply_preemphasis(input_buffer, window_length, preemphasis);
        }

        apply_window(input_buffer, window_length, window);

        kiss_fftr(cfg, input_buffer, output_buffer);

        for (int j = 0; j < num_frequency_bins; ++j)
        {
            float real = output_buffer[j].r;
            float imag = output_buffer[j].i;
            float magnitude = real * real + imag * imag;
            magnitudes[i * num_frequency_bins + j] = magnitude;
        }
    }

    if (power > 0 && power != 2)
    {
        for (int i = 0; i < d1; ++i)
        {
            for (int j = 0; j < num_frequency_bins; ++j)
            {
                magnitudes[i * num_frequency_bins + j] = pow(magnitudes[i * num_frequency_bins + j], 2 / power);
            }
        }
    }

    int shape[2];
    shape[0] = transpose ? d1_max : num_mel_filters;
    shape[1] = transpose ? num_mel_filters : d1_max;

    for (int i = 0; i < num_mel_filters; ++i)
    {
        for (int j = 0; j < d1; ++j)
        {
            float sum = 0;
            for (int k = 0; k < num_frequency_bins; ++k)
            {
                sum += mel_filters[i * num_frequency_bins + k] * magnitudes[j * num_frequency_bins + k];
            }
            spectrogram[transpose ? j * num_mel_filters + i : i * d1 + j] = fmax(mel_floor, sum);
        }
    }

    if (power > 0 && log_mel)
    {
        int o = fmin(spectrogram_length, d1 * num_mel_filters);

        if (log_mel == LOG_MEL_LOG10)
        {
            for (int i = 0; i < o; ++i)
            {
                spectrogram[i] = log10(spectrogram[i]);
            }
        }
        else if (log_mel == LOG_MEL_LOG)
        {
            for (int i = 0; i < o; ++i)
            {
                spectrogram[i] = log(spectrogram[i]);
            }
        }
    }

    free(input_buffer);
    free(output_buffer);
    free(magnitudes);

    free(cfg);
}
