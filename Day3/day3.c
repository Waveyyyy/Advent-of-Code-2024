#include "../utils/utils.h"
#include <stdio.h>
#include <string.h>

int solve_part1(char *input, ssize_t length);
int get_number_lines(char *pinput, ssize_t input_length);
void split_rows(char *pinput, ssize_t input_length, char **rows);
void slice(char *result, char *source, ssize_t start, ssize_t end);

int main()
{
  char test_input[] =
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
  int test_input_length = strlen(test_input);
  printf("part1 result!!! %d\n", solve_part1(test_input, test_input_length));
}

int solve_part1(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  char **rows = malloc(lines * sizeof(char *));
  split_rows(input, input_length, rows);
  int result = 0;

  return result;
}

int get_number_lines(char *pinput, ssize_t input_length)
{
  // get number of lines in the input
  int lines = 0;
  for (int i = 0; i < input_length; i++) {
    if (pinput[i] == '\n') {
      ++lines;
    }
  }
  return lines;
}

void split_rows(char *pinput, ssize_t input_length, char **rows)
{
  int cur_line = 0;
  ssize_t pos = 0;
  // loop until pos is >= input_length
  while (pos < input_length) {
    char *psubstr;
    ssize_t slice_length;
    char *substring;

    printf("pinput + pos (%zd): %s\n", pos, pinput + pos);
    // get the first newline character after pinput + pos
    psubstr = strchr(pinput + pos, '\n');
    if (psubstr == NULL) // in this case, we are on the last line (one that has
    // no newline)
    {
      // if input_length is 71, pos will be 0, therefore slice length is the
      // length of the whole line
      slice_length = input_length - pos;
    } else {
      // if there is a newline we want to set the slice length to be the
      // distance between where the newline occurs (the end of our line) and our
      // current position within the string
      slice_length = psubstr - (pinput + pos);
    }
    // increment pos by slice_length + 1
    pos += slice_length + 1;
  }
}

void slice(char *result, char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
}
