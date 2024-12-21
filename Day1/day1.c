#include "../utils/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_number_lines(char *pinput, ssize_t input_length);
int split_input(char *pinput, ssize_t input_length, int *left, int *right);
void slice(char *result, char *source, ssize_t start, ssize_t end);
void sort_array(int *array, int array_length);
int calculate_result(int *left, int *right, ssize_t lines);
int solve_part1(char *input, ssize_t input_length);
int solve_part2(char *input, ssize_t input_length);

int main()
{
  char test_input[] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
  ssize_t test_input_length = strlen(test_input);
  assert(solve_part1(test_input, test_input_length) == 11);
  char *aoc_input;
  read_file(&aoc_input, "input.txt");
  ssize_t input_length = strlen(aoc_input);

  int result = solve_part1(aoc_input, input_length);
  if (result != -1) {
    printf("Part 1 result! %d\n", result);
  } else {
    printf("something went wrong with conversion of chars to ints");
    free(aoc_input);
    return 1;
  }

  assert(solve_part2(test_input, test_input_length) == 31);
  result = solve_part2(aoc_input, input_length);
  if (result != -1) {
    printf("Part 2 result! %d\n", result);
  } else {
    printf("something went wrong with conversion of chars to ints");
    free(aoc_input);
    return 1;
  }
  free(aoc_input);
}

int solve_part1(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  int *left = calloc(lines, sizeof(int));
  int *right = calloc(lines, sizeof(int));

  if (split_input(input, input_length, left, right) == 1) {
    printf("unable to convert characters to integer\n");
    return -1;
  }
  sort_array(left, lines);
  sort_array(right, lines);

  int result = calculate_result(left, right, lines);
  free(left);
  free(right);
  return result;
}

int solve_part2(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  int *left = calloc(lines, sizeof(int));
  int *right = calloc(lines, sizeof(int));

  if (split_input(input, input_length, left, right) == 1) {
    printf("unable to convert characters to integer\n");
    return -1;
  }

  // initialise array for holding the number of times each integer in the left
  // column appears in the right column
  int *occurrences = calloc(lines, sizeof(int));
  int result = 0;

  // iterate over the left column
  for (int i = 0; i < lines; i++) {
    // iterate over the right column for each iteration of the left column
    for (int j = 0; j < lines; j++) {
      // compare the integer in the left column with each integer in the right
      // column, if they are the same add one to the occurrence at that index
      if (left[i] == right[j]) {
        occurrences[i]++;
      }
    }
    // calculate the result by multiplying the value in the left column by how
    // many times it occurrs in the right column and adding it to a running
    // total
    result += left[i] * occurrences[i];
  }

  free(left);
  free(right);
  free(occurrences);
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

int split_input(char *pinput, ssize_t input_length, int *left, int *right)
{
  int cur_line = 0;
  for (int pos = 0; pos < input_length; pos++) {
    char *psubstr;
    char *remaining_rows;
    ssize_t slice_length;
    char *substring;
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
      // get a pointer to the first newline in the remaining rows
      psubstr = strstr(remaining_rows, "\n");
      // get the length of the slice from 0 to the first newline. this is the
      // entire row.
      slice_length = psubstr - &remaining_rows[0];
      // create substring variable of len slice_length
      substring = malloc(slice_length);
      // copy slice into substring
      slice(substring, remaining_rows, 0, slice_length);
      free(remaining_rows);
    }
    printf("%s\n", substring);

    // find first occurrence of space on in the slice(strstr)
    char *first_sep = strstr(substring, " ");
    // get the length of the integer left column
    ssize_t leftcol_len = first_sep - &substring[0];
    // get the length of the integer in the right column
    ssize_t rightcol_len = &substring[slice_length] - (first_sep + 3);

    // get chars from start of line (char following newline or pos 0) to first
    // occurrence of space - gives left column.
    char *left_chars = malloc(leftcol_len);
    slice(left_chars, substring, 0, leftcol_len);

    // add 2 to current pos (3 spaces inbetween columns 1(currentPos)+2=3)
    // get from new Pos up to newline - gives right column
    char *right_chars = malloc(rightcol_len);
    slice(right_chars, substring, slice_length - rightcol_len, slice_length);

    // convert each columns character string into integers and store in
    // respective arrays
    char *endp = NULL;
    left[cur_line] = strtol(left_chars, &endp, 10);
    if (endp == left_chars) {
      return 1;
    }
    endp = NULL;
    right[cur_line] = strtol(right_chars, &endp, 10);
    if (endp == right_chars) {
      return 1;
    }

    // move the position along slice_length bytes (additional +1 on loop
    // iteration)
    pos += slice_length;
    // iterate cur_line
    cur_line++;
    free(substring);
    free(left_chars);
    free(right_chars);
  }
  return 0;
}

void slice(char *result, char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
}

void sort_array(int *array, int array_length)
{
  int current_value;
  // iterate over the array array_length times
  for (int i = 0; i < array_length; i++) {
    // this loop iterates over the array, one position ahead of the outer loop
    for (int j = i + 1; j < array_length; j++) {
      // if the value at the current index is greater than the value at the next
      // index, swap the positions of the items
      if (array[i] > array[j]) {
        current_value = array[i];
        array[i] = array[j];
        array[j] = current_value;
      }
    }
  }
}

int calculate_result(int *left, int *right, ssize_t lines)
{
  int total = 0;
  // iterate lines number of times
  for (int i = 0; i < lines; i++) {
    // compare the value at i in the left array against the value at i in the
    // right array
    if (left[i] < right[i]) {
      // subtract left from right and add to total if right is larger
      total += (right[i] - left[i]);
    } else {
      // otherwise subtract right from left and add to total
      total += (left[i] - right[i]);
    }
  }
  return total;
}
