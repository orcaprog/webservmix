#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    const char *relativePath = "./rootdir";

    // Allocate a buffer to store the absolute pathname
    char resolvedPath[PATH_MAX];

    // Call realpath to resolve the pathname
    char *result = realpath(relativePath, resolvedPath);

    // Check if realpath was successful
    if (result != NULL) {
        printf("Original Path: %s\n", relativePath);
        printf("Resolved Path: %s\n", resolvedPath);
        printf("result Path: %s\n", result);

        // Do further processing with the resolved path

        // Remember to free the allocated memory
        // free(result);
    } else {
        perror("realpath");
        // Handle error, if any
    }

    return 0;
}
