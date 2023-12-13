
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char program_name[100];
    int num_runs;

    printf("Enter program name: ");
    scanf("%s", program_name);
flag:
    printf("Enter number of runs (1-255): ");
    scanf("%d", &num_runs);

    if (num_runs < 1 || num_runs > 255) {
        printf("Number of runs must be between 1 and 255\n");
        goto flag;
    }

    for (int i = 0; i < num_runs; i++) {
        printf("Running %s, iteration %d\n", program_name, i+1);
        char command[200];
        sprintf(command, "./%s", program_name);
        int ret = system(command);
        if (ret != 0) {
            printf("Error running %s, iteration %d\n", program_name, i+1);
            return 1;
        }
    }

    printf("Finished running %s %d times\n", program_name, num_runs);
    return 0;
}