#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_number_lines(char *pinput, ssize_t input_length);
void split_input(char *pinput, ssize_t input_length, int *left, int *right);
void slice(char *result, char *source, ssize_t start, ssize_t end);

int main()
{
  char test_input[] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
  printf("%s", test_input);
  int lines = get_number_lines(test_input);
  int *left = calloc(lines, sizeof(int));
  int *right = calloc(lines, sizeof(int));
  split_input(test_input, left, right);
}

int get_number_lines(char *pinput, ssize_t input_length)
{
  // get number of lines in the input
  int lines = 0;
  for (int i = 0; i < strlen(pinput); i++) {
    if (pinput[i] == '\n') {
      ++lines;
    }
  }
  printf("%d\n", lines);
  return lines;
}

void split_input(char *pinput, ssize_t input_length, int *left, int *right)
{
  int cur_line = 0;
  char *pinput_copy = pinput;
  for (int pos = 0; pos < input_length; pos++) {
    char *psubstr;
    // get slice of string from pos up to newline for each line
    if (pos == 0) {
      // get pointer to the first occurrence of a newline
      psubstr = strstr(pinput_copy, "\n");
    } else {
      slice(pinput_copy, pinput_copy, pos, input_length - pos);
      psubstr = strstr(pinput_copy, "\n");
    }
    // get the length of this slice
    ssize_t slice_length = psubstr - &pinput_copy[0];
    // create substring variable of len slice_length
    char *substring = malloc(slice_length);
    // copy slice into substring
    slice(substring, pinput_copy, pos, slice_length);
    /*     strncpy(substring, pinput_copy + pos, slice_length); */
    printf("%s\n", substring);

    // find first occurrence of space on in the slice(strstr)
    //
    // get chars from start of line (char following newline or pos 0) to first
    // occurrence of space - gives left column.
    //
    // add 2 to current pos (3 spaces inbetween columns 1(currentPos)+2=3)
    // get from new Pos up to newline - gives right column

    // move the position along slice_length bytes (additional +1 on loop
    // iteration)
    pos += slice_length;
    // iterate cur_line
    cur_line++;
  }
}

void slice(char *result, char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
}
