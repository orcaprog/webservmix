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
#include <iostream>       // std::cout
#include <string>         // std::string
#include <cstddef>        // std::size_t

int main ()
{
  std::string str ("//////dddddd///////");

  std::size_t found = str.find_first_not_of("/");

  if (found!=std::string::npos)
  {
    std::cout << "The first non-alphabetic character is " << str[found];
    std::cout << " at position " << found << '\n';
  }

  return 0;
}