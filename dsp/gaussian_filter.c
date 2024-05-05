#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SRATE 1000
#define TIME_LEN 3001
#define P 15
#define NOISE_AMP 5
#define FWHM 25
#define K 100

double interp1(double *x, double *y, int n, double xi) {
    int i;
    double slope, intercept;

    for (i = 1; i < n; i++) {
        if (xi <= x[i]) {
            slope = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
            intercept = y[i - 1] - slope * x[i - 1];
            return slope * xi + intercept;
        }
    }
    return y[n - 1];
}

void gaussian_kernel(double *gauswin, int len, double fwhm) {
    int i;
    double gtime[len];
    for (i = 0; i < len; i++) {
        gtime[i] = 1000 * (-K + i) / SRATE;
        gauswin[i] = exp(-4 * log(2) * gtime[i] * gtime[i] / (fwhm * fwhm));
    }
}

int main() {
    FILE *output_file;
    output_file = fopen("output_data.txt", "w");
    if (output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    double time[TIME_LEN];
    double signal[TIME_LEN];
    double ampl[TIME_LEN];
    double noise[TIME_LEN];
    double gauswin[2 * K + 1];
    double filtsigG[TIME_LEN];
    int i, prePeakHalf, pstPeakHalf;
    double empFWHM;

    // Generate time vector
    for (i = 0; i < TIME_LEN; i++) {
        time[i] = i / (double) SRATE;
    }

    // Generate signal with noise
    for (i = 0; i < TIME_LEN; i++) {
        noise[i] = NOISE_AMP * rand() / (double) RAND_MAX;
        ampl[i] = interp1((double[P]){0}, (double[P]){0}, P, (double) rand() / RAND_MAX * 30);
        signal[i] = ampl[i] + noise[i];
    }

    // Generate Gaussian kernel
    gaussian_kernel(gauswin, 2 * K + 1, FWHM);

    // Calculate FWHM
    for (i = K; i < 2 * K + 1; i++) {
        if (gauswin[i] >= 0.5) {
            pstPeakHalf = i;
            break;
        }
    }
    for (i = K - 1; i >= 0; i--) {
        if (gauswin[i] >= 0.5) {
            prePeakHalf = i;
            break;
        }
    }
    empFWHM = 1000 * (gauswin[pstPeakHalf] - gauswin[prePeakHalf]);

    // Gaussian filtering
    for (i = K; i < TIME_LEN - K; i++) {
        double sum = 0;
        for (int j = -K; j <= K; j++) {
            sum += signal[i + j] * gauswin[j + K];
        }
        filtsigG[i] = sum;
    }

    // Print results
    fprintf(output_file, "FWHM: %.2f ms\n", empFWHM);
    fprintf(output_file, "Time\tOriginal Signal\tGaussian-filtered Signal\n");
    for (i = 0; i < TIME_LEN; i++) {
        fprintf(output_file, "%.3f\t%.3f\t%.3f\n", time[i], signal[i], filtsigG[i]);
    }

    fclose(output_file);
    printf("Data written to output_data.txt\n");

    return 0;
}

