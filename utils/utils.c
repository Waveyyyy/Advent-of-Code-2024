#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

ssize_t read_file(char **buffer, char *file_name)
{
  FILE *file_ptr;
  // open file we want to read
  file_ptr = fopen(file_name, "r");

  // check if the file exists
  if (file_ptr == NULL) {
    perror("fopen failed to open the file");
    return -1;
  }

  // use fseek to go to the end of the stream (file_ptr)
  fseek(file_ptr, 0L, SEEK_END);
  // use ftell to get the position of the stream, now the end of the file per
  // fseek function.
  // this gives us the length of the file in bytes
  ssize_t len_bytes = ftell(file_ptr);
  if (len_bytes < 0) {
    perror("fseek/ftell failed");
    // close the file_ptr before returning
    fclose(file_ptr);
    return -1;
  }

  // finally, reset the stream position to the start of the file
  fseek(file_ptr, 0L, SEEK_SET);

  // get enough memory for the buffer to be able to hold the file contents
  *buffer = (char *)calloc((len_bytes + 1), sizeof(char));
  // check for memory allocation error
  if (*buffer == NULL) {
    perror("calloc failed to alloc memory");
    fclose(file_ptr);
    return -1;
  }

  size_t bytes_read = fread(*buffer, sizeof(char), len_bytes, file_ptr);
  // check for errors with fread
  if (ferror(file_ptr)) {
    perror("fread failed to read into the buffer");
    free(*buffer);
    fclose(file_ptr);
    return -1;
  }
  if (bytes_read != (size_t)len_bytes) {
    fprintf(stderr, "Warning: short read (%zu bytes read, expected %zd)\n",
            bytes_read, len_bytes);
  }

  fclose(file_ptr);
  (*buffer)[len_bytes] = '\0'; // NULL terminate the buffer

  return len_bytes;
}
