/*
 * Central Limit Theorem Printer
 * Prints a histogram appearing as rows of X's 
 * where the number of Xs in each row represents the number of times that an average over random samples 
 * fell into the range represented by that row.
 * Author: Omer Yurekli - yureklio@bc.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLES 10000
#define RUNS 50000
#define BINS 64
#define HISTOGRAM_SPAN 0.05
#define SCALE 32

double get_mean_of_uniform_random_samples() {
    double sum = 0.0;
    for (int i = 0; i < SAMPLES; ++i) {
	double sample = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        sum += sample;
    }
    return sum / SAMPLES;
}


double populate_values_and_get_mean(double *values) {
    double sum = 0.0;
    for (int i = 0; i < RUNS; ++i) {
	values[i] = get_mean_of_uniform_random_samples();
	sum += values[i];
    }
    return sum / RUNS;
}

double get_mean_squared_error(double *values, double mean) {
    double sum = 0.0;
    for (int i = 0; i < RUNS; ++i) {
	double error = values[i] - mean;
	sum += error * error;
    }
    return sum / RUNS;
}

void create_histogram(double *values, int *counts) {
    double bin_size = HISTOGRAM_SPAN / (double)BINS;
    int j;
    double bin_start;
    double bin_end;
    int i;

    for ( i = 0; i < BINS; ++i) {
	counts[i] = 0;
    }
    
    for (i = 0; i < RUNS; ++i) {
    	for (j = 0, bin_start = -(HISTOGRAM_SPAN / 2.0); j < BINS; ++j, bin_start += bin_size) {
            bin_end = bin_start + bin_size;
	    if (values[i] >= bin_start && (j == BINS - 1 ? values[i] <= bin_end : values[i] < bin_end)) {
                counts[j]++;
                break;
	    }
	
	}
    }
}

void print_histogram(int *counts) {
    double bin_size = HISTOGRAM_SPAN / BINS;
    int j;
    double bin_start;
    double bin_end;

    for (j = 0, bin_start = -(HISTOGRAM_SPAN / 2.0); j < BINS; ++j, bin_start += bin_size) {
        int print_count = counts[j] / SCALE;

	printf("%5.4f: ", bin_start);

	for (int k = 0; k < print_count; k++) {
            printf("X");
        }

        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    double *values = (double *) malloc(RUNS * sizeof(double));
    double mean = populate_values_and_get_mean(values);
    double error = get_mean_squared_error(values, mean);
    int *counts = (int *)calloc(BINS, sizeof(int));
    create_histogram(values, counts);
    print_histogram(counts);
    printf("Sample mean: %f  Sample variance: %f\n",mean, error);
    free(values);
    return 0;
}
