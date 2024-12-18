#include "../utils/utils.h"
#include <assert.h>
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
  assert(solve_part1(test_input, test_input_length) == 2);

  char *aoc_input;
  read_file(&aoc_input, "input.txt");
  ssize_t input_length = strlen(aoc_input);
  solve_part1(aoc_input, input_length);
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
  // get the first token (first digit)
  digit = strtok(row, " ");
  int ctr = 0;
  // while there are still tokens available, continue looping
  while (digit != NULL) {
    char *endp = NULL;
    /*     printf("digit: %s\n", digit); */
    // convert the digit to an integer and add to the output array
    output[ctr] = strtol(digit, &endp, 10);
    if (endp == digit) {
      return -1;
    }
    /*     printf("digits[%d] = %d\n", ctr, output[ctr]); */
    digit = strtok(NULL, " ");
    if (ctr == (&output)[1] - output) {
      output = realloc(output, (ctr + 1) * sizeof(int));
    }
    ctr++;
  }
  return ctr;
}

int check_safety(char **rows, ssize_t lines)
{
  int safe_reports = 0;
  int *digits;
  // iterate over each row
  for (int i = 0; i < lines; i++) {
    digits = malloc(10 * sizeof(int));
    // get only the digits in a given row
    int digit_length = split_row_to_digits(rows[i], digits);
    if (digit_length == -1) {
      printf("issue when converting digit char to integer\n");
    }
    int has_decreased = 0;
    int has_increased = 0;
    // iterate over the digits array, and check if it is considered 'safe'
    for (int j = 0; j < digit_length; j++) {
      printf("j=%d : j+1=%d\n", digits[j], digits[j + 1]);
      // if the on the last digit in the row, we can consider the report 'safe'
      if (j+1 == digit_length) {
        safe_reports++;
        break;
      }
      // if any 2 adjacent digits are equal, it is 'unsafe'
      if (digits[j] == digits[j + 1]) {
        break;
      }
      int difference = 0;
      // check which is greater, the current value at index j or the next value
      // at j+1 before calculating the difference between the two
      if (digits[j] < digits[j + 1]) {
        difference = digits[j + 1] - digits[j];
        has_decreased++;
      } else {
        difference = digits[j] - digits[j + 1];
        has_increased++;
      }
      // if the difference is NOT between 1 and 3 (inclusive) then it is 'unsafe'
      if (!(difference >= 1 && difference <= 3)) {
        break;
      }
      // if the row has both increased and decreased, it is 'unsafe'
      if (has_increased != 0 && has_decreased != 0) {
        break;
      }
    }
  }
  printf("%d\n", safe_reports);
  return safe_reports;
}
