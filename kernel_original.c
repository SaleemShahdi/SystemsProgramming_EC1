#include <stdio.h>
#include <math.h>
#include <stdlib.h>


typedef struct node{
    int data;
    struct node * next;
} node;

typedef struct LL{
    node  * head;
    node * last;
} LL;

void add(int a, LL * linkedList) {
    node * current = malloc(sizeof(node));
    current->data = a;
    current->next = NULL;
    if (linkedList->head == NULL) {
        linkedList->head = current;
        linkedList->last = current;
    } else {
        linkedList->last->next = current;
        linkedList->last = current;
    }
}

void freeNodes(node * head) {
    if (head->next == NULL) {
        free(head);
        head = NULL;
    } else {
        freeNodes(head->next);
        free(head);
        head = NULL;
    }
}

void freeLinkedList(LL * linkedList) {
    node * current = linkedList->head;
    /* while(current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("\ndone\n"); */
    if (linkedList->head != NULL) {
        freeNodes(linkedList->head);
    }
    free(linkedList);
    linkedList = NULL;

}

float getEuclideanDistance(float * components, float * centroidcomponents, int pixelSize) {
    float result = 0;
    for (int i = 0; i <= pixelSize - 1; i++) {
        result = result + pow((components[i] - centroidcomponents[i]), 2);
    }
    result = sqrt(result);
    return result;
}

int findClosest(float * centroids, int num_centroids, float * components, int pixelSize) {
    int index;
    // float centroidcomponents [pixelSize];
    float * centroidcomponents = malloc(pixelSize * sizeof(float));
    float min;
    for (int i = 0; i <= (num_centroids - 1) * pixelSize; i = i + pixelSize) {
        if (i >= (num_centroids - 3) * 3) {
            printf("%d\n", i);
        }
        for (int c = 0; c <= pixelSize - 1; c++) {
            centroidcomponents[c] = centroids[i + c];
        }
        float current = getEuclideanDistance(components, centroidcomponents, pixelSize);
        if (i == 0) {
            min = current;
            index = i;
        } else if (current < min) {
            min = current;
            index = i;
        }
    }
    free(centroidcomponents);
    return index;
}

int convertToInt(float * centroids, int closestCentroidIndex, int pixelSize) {
    // printf("%d\n", closestCentroidIndex);
    int result = 0;
    for (int i = 0; i <= pixelSize - 1; i++) {
        float intermediate = pow(centroids[closestCentroidIndex + i], 3*(pixelSize - 1 - i));
        result = result + (int)(intermediate);
    }
    return result;
}

LL * getPixels(float * centroids, int centroidIndex, int * labels, int num_pixels, int pixelSize) {
    LL * linkedList = malloc(sizeof(LL));
    linkedList->head = NULL;
    linkedList->last = NULL;
    for (int i = 0; i <= num_pixels - 1; i++) {
        if (convertToInt(centroids, centroidIndex, pixelSize) == labels[i]) {
            add(i, linkedList);
        }
    }
    return linkedList;
}

int * getPixelsArray(float * centroids, int centroidIndex, int * labels, int num_pixels, int pixelSize, int * pixelListSize){
    /* int count = 0;
    for (int i = 0; i <= num_pixels - 1; i++) {
        if (convertToInt(centroids, centroidIndex, pixelSize) == labels[i]) {
            count = count + 1;
        }
    }
    int * result = malloc(count * sizeof(int));
    *pixelListSize = count;
    int index = 0;
    for (int i = 0; i <= num_pixels - 1; i++) {
        if (convertToInt(centroids, centroidIndex, pixelSize) == labels[i]) {
            result[index] = i;
            index = index + 1;
        }
    } */
   int count = 0;
   for (int i = 0; i <= num_pixels - 1; i++) {
        if (labels[i] == centroidIndex) {
            count = count + 1;
        }
    }
    int * result = malloc(count * sizeof(int));
    *pixelListSize = count;
    int index = 0;
    for (int i = 0; i <= num_pixels - 1; i++) {
        if (labels[i] == centroidIndex) {
            result[index] = i;
            index = index + 1;
        }
    }
    return result;
}

void kmeans_clustering(float *pixels, int num_pixels, int num_centroids, int max_iters, int seed, float *centroids, int *labels) {
    srand(seed);
    int pixelSize = 3;
    for (int i = 0; i <= pixelSize * (num_centroids - 1); i++) {
        centroids[i] = (float)(rand() % (256));

    }
    // float components [pixelSize]; // red = components[0], green = components[1], blue = components[2]
    float * components = malloc(pixelSize * sizeof(float));
    for (int i = 0; i <= max_iters - 1; i++) {
        for (int j = 0; j <= (num_pixels - 1) * pixelSize; j = j + pixelSize) {
            for (int c = 0; c <= pixelSize - 1; c++) {
                components[c] = pixels[j+c];
            }
            int closestCentroidIndex = findClosest(centroids, num_centroids, components, pixelSize);
            int labelindex = j / 3;
            // labels[labelindex] = convertToInt(centroids, closestCentroidIndex, pixelSize);
            labels[labelindex] = closestCentroidIndex;
        }
        for (int j = 0; j <= (num_centroids - 1) * pixelSize; j = j + pixelSize) {
            // LL * listofpixels = getPixels(centroids, j, labels, num_pixels, pixelSize);
            int pixelListSize = 0;
            int * listofpixels = getPixelsArray(centroids, j, labels, num_pixels, pixelSize, &pixelListSize);
            for (int c = 0; c <= pixelSize - 1; c++) {
                components[c] = 0;
            }
            /* int count = 0;
            node * current = listofpixels->head;
            while (current != NULL) {
                int temp = current->data;
                for (int c = 0; c <= pixelSize - 1; c++) {
                    components[c] = components[c] + pixels[temp+c];
                }
                count = count + 1;
                current = current->next;
            } */
            for (int q = 0; q <= pixelListSize - 1; q++) {
                int temp = listofpixels[q];
                for (int c = 0; c <= pixelSize - 1; c++) {
                    components[c] = components[c] + pixels[temp+c];
                }
            }
            for (int c = 0; c <= pixelSize - 1; c++) {
                components[c] = components[c] / pixelListSize;
                centroids[j+c] = components[c];
            }
            free(listofpixels);
            

        }
        printf("%d\n", i);

    }
    for (int i = 0; i <= 4; i++) {
        printf("Color component value: %f\n", pixels[i]);
    }
    free(components);
}

