#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LINES 100000

int main() {
    float file_sum = 0.0;
    float memory_sum = 0.0;
    char *filename = "p3_numbers.txt";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    // Read file into memory
    float numbers[NUM_LINES];
    for (int i = 0; i < NUM_LINES; i++) {
        fscanf(file, "%f", &numbers[i]);
    }

    // Close the file
    fclose(file);

    // Calculate time to read from file
    clock_t start_file = clock();
    // 1. Open file
    // 2. Iterate through lines in file and sum numbers into file_sum
    // 3. Close file
    clock_t end_file = clock();

    // Calculate time to read from memory
    clock_t start_mem = clock();
    // 1. Iterate through numbers variable and sum numbers into memory_sum
    clock_t end_mem = clock();

    // Print results
    printf("Read %d numbers from file %s\n", NUM_LINES, filename);
    printf("Product from file: %lf\n", file_sum);
    printf("Product from memory: %lf\n", memory_sum);
    printf("Time to read from file: %lf seconds\n",
           (double)(end_file - start_file) / CLOCKS_PER_SEC);
    printf("Time to read from memory: %lf seconds\n",
           (double)(end_mem - start_mem) / CLOCKS_PER_SEC);

    return 0;
}
