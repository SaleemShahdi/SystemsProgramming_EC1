#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h> 

void kmeans_clustering(float* pixels, int num_pixels, int num_centroids, int max_iters, int seed, float* centroids, int* labels) {
    srand(seed);
    for (int i = 0; i < num_centroids; i++) {
        for (int c = 0; c <= 2; c++) {
            centroids[i * 3 + c] = (float)(rand() % 256);
        }
    }

    float* new_centroids = (float*)malloc(num_centroids * 3 * sizeof(float));
    int* counts = (int*)malloc(num_centroids * sizeof(int));

    for (int iter = 0; iter < max_iters; iter++) {
        for (int i = 0; i < num_pixels; i++) {
            float best_dist = 1e12;
            int best_index = 0;
            float px = pixels[i*3 + 0];
            float py = pixels[i*3 + 1];
            float pz = pixels[i*3 + 2];
            for (int j = 0; j < num_centroids; j++) {
                float cx = centroids[j*3 + 0];
                float cy = centroids[j*3 + 1];
                float cz = centroids[j*3 + 2];
                float dx = px - cx;
                float dy = py - cy;
                float dz = pz - cz;
                float dist = dx*dx + dy*dy + dz*dz;
                if (dist < best_dist) {
                    best_dist = dist;
                    best_index = j;
                }
            }
            labels[i] = best_index;
        }

        memset(new_centroids, 0, num_centroids * 3 * sizeof(float));
        memset(counts, 0, num_centroids * sizeof(int));

        for (int i = 0; i < num_pixels; i++) {
            int idx = labels[i];
            new_centroids[idx*3 + 0] += pixels[i*3 + 0];
            new_centroids[idx*3 + 1] += pixels[i*3 + 1];
            new_centroids[idx*3 + 2] += pixels[i*3 + 2];
            counts[idx]++;
        }

        for (int j = 0; j < num_centroids; j++) {
            if (counts[j] > 0) {
                float new_cx = new_centroids[j*3 + 0] / counts[j];
                float new_cy = new_centroids[j*3 + 1] / counts[j];
                float new_cz = new_centroids[j*3 + 2] / counts[j];
                centroids[j*3 + 0] = new_cx;
                centroids[j*3 + 1] = new_cy;
                centroids[j*3 + 2] = new_cz;
            }
        }
    }
    free(new_centroids);
    free(counts);
}