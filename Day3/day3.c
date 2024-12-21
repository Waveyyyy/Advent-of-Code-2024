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
  for (int i = 0; i < strlen(pinput); i++) {
    if (pinput[i] == '\n') {
      ++lines;
    }
  }
  return lines;
}

void split_rows(char *pinput, ssize_t input_length, char **rows)
{
  int cur_line = 0;
  for (int pos = 0; pos < input_length; pos++) {
    char *psubstr;
    char *remaining_rows;
    ssize_t slice_length;
    char *substring;
    /*     printf("pos: %d\n", pos); */
    // get slice of string from pos up to newline for each line
    if (pos == 0) {
      // get pointer to the first occurrence of a newline
      psubstr = strstr(pinput, "\n");
      // get the length of the slice of the first row
      slice_length = psubstr - &pinput[0];
      /*       printf("%p - %p = %ld", psubstr, &pinput[0], psubstr - &pinput[0]); */
      // create substring variable of len slice_length
      substring = malloc(slice_length);
      // copy slice into substring
      slice(substring, pinput, 0, slice_length);
    } else {
      // allocate enough memory to hold the remaining rows of input
      remaining_rows = malloc(input_length - pos);
      // add the remaining rows of input to remaining_rows
      slice(remaining_rows, pinput, pos, input_length - pos);
      /*       printf("remaining_rows:\n%s\n", remaining_rows); */
      // get a pointer to the first newline in the remaining rows
      psubstr = strstr(remaining_rows, "\n");
      /*       printf("length of slice: %p - %p = %lu\n", psubstr, &remaining_rows[0], */
      /*              psubstr - &remaining_rows[0]); */
      // get the length of the slice from 0 to the first newline. this is the
      // entire row.
      slice_length = psubstr - &remaining_rows[0];
      // if slice_length is less than zero, the splitting of input is done.
      if (slice_length < 0) {
        return;
      }
      // create substring variable of len slice_length
      substring = malloc(slice_length);
      // copy slice into substring
      slice(substring, remaining_rows, 0, slice_length);
    }
    /*     printf("substring: %s\n",substring); */
    rows[cur_line] = substring;
    /*     printf("rows[%d]: %s", cur_line, rows[cur_line]); */
    pos += slice_length;
    cur_line++;
  }
}

void slice(char *result, char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
}
