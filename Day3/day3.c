#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_part1(char *input, ssize_t length);
int get_number_lines(const char *pinput, ssize_t input_length);
void split_rows(const char *pinput, ssize_t input_length, char **rows);
void slice(char *result, const char *source, ssize_t start, ssize_t end);
int calculate_result_of_muls(char **rows, int lines);
int get_valid_muls(const char *row, int length, char ***valid_muls);

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
  // free malloc memory for rows
  for (int i = 0; i < lines; i++) {
    free(rows[i]);
  }
  free(rows);
  return result;
}

int get_number_lines(const char *pinput, ssize_t input_length)
{
  // get number of lines in the input
  int lines = 0;
  for (int i = 0; i < input_length; i++) {
    if (pinput[i] == '\n') {
      ++lines;
    }
  }
  // check if the input is NON-EMPTY, but there are no newlines
  if (lines == 0 && input_length > 0) {
    lines = 1;
  }
  /*   printf("%d\n", lines); */
  return lines;
}

int calculate_result_of_muls(char **rows, int lines)
{
  char **valid_muls = NULL;
  int num_muls;
  int valid_muls_counter = 0;
  for (int i = 0; i < lines; i++) {
    int row_len = strlen(rows[i]);
    num_muls = get_valid_muls(rows[i], row_len, &valid_muls);

    if (num_muls < 0) {
      continue;
    }
    valid_muls_counter += num_muls;
  }
  if (valid_muls != NULL) {
    for (int i = 0; i < valid_muls_counter; i++) {
      free(valid_muls[i]);
    }
    free(valid_muls);
  }
}

int get_valid_muls(const char *row, int length, char ***valid_muls)
{
  int num_mul = 0;
  int pos = 0;
  while (pos < length) {
    // check for the start of a valid mul instruction
    char *substr = strstr(row + pos, "mul(");
    if (substr == NULL) // if no mul instructions were found, then we can break
    {
      fprintf(stderr, "No mul instructions\n");
      break;
    }
    // check for the end of a valid mul instruction
    char *substr_end = strstr(substr, ")");
    if (substr_end == NULL) // if no end to a mul struction was found, it is
    // invalid and we can move on
    {
      fprintf(stderr, "Unmatched parenthesis for mul instruction\n");
      // free memory on error
      for (int i = 0; i < num_mul; i++) {
        free((*valid_muls)[i]);
      }
      free(*valid_muls);
      return -1;
    }

    ssize_t slice_length = substr_end - substr + 1;
    char *valid_mul = malloc(slice_length + 1);
    if (valid_mul == NULL) {
      perror("Malloc failed to alloc memory");
      exit(1);
    }
    slice(valid_mul, substr, 0, slice_length);

    char *next_mul = strstr(valid_mul + 1, "mul(");
    if (next_mul != NULL) {
      fprintf(stderr, "Mul instruction found within parentheses: %s\n",
              valid_mul);
      // free valid_mul before going to next iteration
      free(valid_mul);
      pos = substr + 4 - row;
      continue;
    }
    num_mul++;
    *valid_muls = realloc(*valid_muls, num_mul * sizeof(char *));
    if (valid_muls == NULL) {
      perror("Malloc failed to alloc memory");
      exit(1);
    }
    (*valid_muls)[num_mul - 1] = valid_mul;

    pos = substr_end - row + 1;
  }
  return num_mul;
}

void split_rows(const char *pinput, ssize_t input_length, char **rows)
{
  int cur_line = 0;
  ssize_t pos = 0;
  // loop until pos is >= input_length
  while (pos < input_length) {
    char *psubstr;
    ssize_t slice_length;
    char *substring;

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

    // allocate enough memory for the slice containing the desired row
    substring = malloc(slice_length + 1); // one extra for null terminator
    if (substring == NULL) {
      perror("Malloc failed to alloc memory");
      exit(1);
    }
    // copy the row into the substring variable
    slice(substring, pinput + pos, 0, slice_length);
    // add the substring to rows array at the current line number
    rows[cur_line] = substring;
    // increment pos by slice_length + 1 to skip the newline
    pos += slice_length + 1;
    cur_line++;
  }
}

void slice(char *result, const char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
  result[end] = '\0';
}
