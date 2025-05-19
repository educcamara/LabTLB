#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef __linux__
#include <sched.h>
#endif

int get_page_size();

double get_time_diff(struct timeval start, struct timeval end);

int main(int argc, char **argv) {

#ifdef __linux__
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);
#endif


    if (argc != 3) {
        fprintf(stderr, "usage: %s <n_pages> <n_iterations>\n", argv[0]);
    }
    const int PAGESIZE = get_page_size();
    if (PAGESIZE == -1) {
        fprintf(stderr, "Failed to get page size\n");
        return 1;
    }

    const int n_pages = atoi(argv[1]);
    const int n_iterations = atoi(argv[2]);

    int jump = PAGESIZE/sizeof(int); // number of integers in a page
    int *array = malloc(n_pages * PAGESIZE);
    if (!array) {
        perror("malloc failed");
        return -1;
    }

    // Main loop
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < n_iterations; i++) {
        for (int j = 0; j < n_pages; j++) {
            array[j * jump] += 1;
        }
    }
    gettimeofday(&end, NULL);
    double time_diff = get_time_diff(start, end);

    // Average time per access
    double avg_time = time_diff / ((double)n_iterations * n_pages);

    printf("%.16lf\n", avg_time);

    free(array);
    return 0;
}


int get_page_size() {
    const char *command = "getconf PAGESIZE";
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "popen failed\n");
        return -1;
    }

    char buffer[32];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fprintf(stderr, "fgets failed\n");
        pclose(fp);
        return -1;
    }

    int page_size = atoi(buffer);
    return page_size;
}

double get_time_diff(struct timeval start, struct timeval end) {
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
}