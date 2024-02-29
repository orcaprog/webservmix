// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>

// int main() {
//     // const char *relativePath = "sdklfjlksjdflj/////";

//     // // Allocate a buffer to store the absolute pathname
//     // char resolvedPath[PATH_MAX];

//     // // Call realpath to resolve the pathname
//     // char *result = realpath(relativePath, resolvedPath);

//     // // Check if realpath was successful
//     //     printf("Resolved Path: %s\n", resolvedPath);
//     // if (result != NULL) {
//     //     printf("Original Path: %s\n", relativePath);
//     //     printf("result Path: %s\n", result);

//     //     // Do further processing with the resolved path

//     //     // Remember to free the allocated memory
//     //     // free(result);
//     // } else {
//     //     perror("realpath");
//     //     // Handle error, if any
//     // }

    
//     return 0;
// }
// string::find_first_not_of
#include <stdio.h>
#include <stdlib.h>

int main() {    
    const char *str = "1234567890123459845";
    char *endptr;
    size_t value = (size_t)strtod(str, &endptr);

    if (*endptr != '\0') {
        // The conversion failed, handle error
        printf("Conversion failed at position: %ld\n", endptr - str);
    } else {
        // Conversion successful, use the 'value'
        printf("Converted value: %zu\n", value);
    }

    return 0;
}