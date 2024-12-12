#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

ssize_t read_file(char **buffer, char *file_name)
{
  FILE *file_ptr;
  // open file we want to read
  file_ptr = fopen(file_name, "r");

  // check if the file exists
  if (file_ptr == NULL)
  {
    return -1;
  }

  // use fseek to go to the end of the stream (file_ptr)
  fseek(file_ptr, 0L, SEEK_END);
  // use ftell to get the position of the stream, now the end of the file per
  // fseek function.
  // this gives us the length of the file in bytes
  ssize_t len_bytes = ftell(file_ptr);

  // finally, reset the stream position to the start of the file
  fseek(file_ptr, 0L, SEEK_SET);

  // get enough memory for the buffer to be able to hold the file contents
  *buffer = (char*)calloc(len_bytes, sizeof(char));

  // check for memory allocation error
  if (*buffer == NULL)
  {
    return -1;
  }

  fread(*buffer, sizeof(char), len_bytes, file_ptr);
  fclose(file_ptr);

  return len_bytes;
}
