#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_number_lines(char *pinput, ssize_t input_length);
void split_input(char *pinput, ssize_t input_length, char **rows);
void slice(char *result, char *source, ssize_t start, ssize_t end);
int solve_part1(char *input, ssize_t input_length);
int split_row_to_digits(char *rows, int *output);
int check_safety(char **rows, ssize_t lines);

int main()
{
  char test_input[] =
    "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9\n";
  ssize_t test_input_length = strlen(test_input);
  solve_part1(test_input, test_input_length);
}

int solve_part1(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  char **rows = malloc(lines * sizeof(char *));
  split_input(input, input_length, rows);
  int result = check_safety(rows, lines);
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
  /*   printf("%d\n", lines); */
  return lines;
}

void split_input(char *pinput, ssize_t input_length, char **rows)
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
      // create substring variable of len slice_length
      substring = malloc(slice_length);
      // copy slice into substring
      slice(substring, remaining_rows, 0, slice_length);
    }
    rows[cur_line] = substring;
    pos += slice_length;
    cur_line++;
  }
}

void slice(char *result, char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
}

int split_row_to_digits(char *row, int *output)
{
  /*   printf("row: %s\n", row); */
  char *digit;
  digit = strtok(row, " ");
  int ctr = 0;
  while (digit != NULL) {
    char *endp = NULL;
    /*     printf("digit: %s\n", digit); */
    output[ctr] = strtol(digit, &endp, 10);
    if (endp == digit) {
      return 1;
    }
    /*     printf("digits[%d] = %d\n", ctr, output[ctr]); */
    digit = strtok(NULL, " ");
    ctr++;
  }
  for (int i = 0; i < (strlen(row) - 1) / 2; i++) {
    printf("%d", output[i]);
  }
  return 0;
}

int check_safety(char **rows, ssize_t lines)
{
}
