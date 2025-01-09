#include "../utils/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve_part1(char *input, ssize_t length);
int get_number_lines(const char *pinput, ssize_t input_length);
void split_rows(const char *pinput, ssize_t input_length, char **rows);
void slice(char *result, const char *source, ssize_t start, ssize_t end);
int calculate_result_of_muls(char **rows, int lines);
int get_valid_muls(const char *row, int length, int **valid_muls,
                   int *len_valid_muls);
int get_digits_and_mul(char *substr_start, char *substr_end);
int get_valid_muls_part2(const char *row, int length, int **valid_muls,
                         int *len_valid_muls, int *mul_active);
int calculate_result_of_muls_part2(char **rows, int lines);
int solve_part2(char *input, ssize_t length);
char *rev_strstr(const char *haystack, const char *needle);

int main()
{
  char test_input_part1[] =
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
  int test_input_part1_length = strlen(test_input_part1);

  char test_input_part2[] =
    "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
  int test_input_part2_length = strlen(test_input_part2);
  assert(solve_part1(test_input_part1, test_input_part1_length) == 161);
    assert(solve_part2(test_input_part2, test_input_part2_length) == 48);
  printf("part2 result!!! %d\n",
         solve_part2(test_input_part2, test_input_part2_length));

  char *aoc_input;
  read_file(&aoc_input, "input.txt");
  int aoc_input_length = strlen(aoc_input);
  printf("part1 result!!! %d\n", solve_part1(aoc_input, aoc_input_length));
  printf("part2 result!!! %d\n", solve_part2(aoc_input, aoc_input_length));
  free(aoc_input);
}

int solve_part1(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  char **rows = malloc(lines * sizeof(char *));
  split_rows(input, input_length, rows);
  int result = calculate_result_of_muls(rows, lines);
  // free malloc memory for rows
  for (int i = 0; i < lines; i++) {
    free(rows[i]);
  }
  free(rows);
  return result;
}

int solve_part2(char *input, ssize_t input_length)
{
  int lines = get_number_lines(input, input_length);
  char **rows = malloc(lines * sizeof(char *));
  split_rows(input, input_length, rows);
  int result = calculate_result_of_muls_part2(rows, lines);
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
  return lines;
}

int calculate_result_of_muls(char **rows, int lines)
{
  int *valid_muls = NULL;
  int num_muls;
  int valid_muls_counter = 0;
  for (int i = 0; i < lines; i++) {
    int row_len = strlen(rows[i]);
    num_muls =
      get_valid_muls(rows[i], row_len, &valid_muls, &valid_muls_counter);

    if (num_muls < 0) {
      continue;
    }
    valid_muls_counter += num_muls;
  }
  int result = 0;
  if (valid_muls != NULL) {
    for (int i = 0; i < valid_muls_counter; i++) {
      result += valid_muls[i];
    }
    free(valid_muls);
    valid_muls = NULL;
  }
  return result;
}

int get_valid_muls(const char *row, int length, int **valid_muls,
                   int *len_valid_muls)
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
        free(valid_muls[i]);
      }
      free(*valid_muls);
      valid_muls = NULL;
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
    next_mul = strstr(valid_mul + 1, ",");
    if (next_mul == NULL) {
      fprintf(stderr, "Comma not found within mul instruction: %s\n",
              valid_mul);
      // free valid_mul before going to next iteration
      free(valid_mul);
      pos = substr + 4 - row;
      continue;
    }
    int calc_result = get_digits_and_mul(substr, substr_end);
    if (calc_result == -1) {
      fprintf(stderr,
              "one side of mul instruction contains invalid digits: %s\n",
              valid_mul);
      // free valid_mul before going to next iteration
      free(valid_mul);
      pos = substr + 4 - row;
      continue;
    }
    num_mul++;
    *valid_muls =
      realloc(*valid_muls, (*len_valid_muls + num_mul) * sizeof(int));
    if (valid_muls == NULL) {
      perror("Malloc failed to alloc memory");
      exit(1);
    }
    (*valid_muls)[(*len_valid_muls + num_mul) - 1] = calc_result;

    pos = substr_end - row + 1;
    free(valid_mul);
  }
  return num_mul;
}

int get_digits_and_mul(char *substr_start, char *substr_end)
{
  // get pointer to where each marker character is, that is the following 3 - (,)
  char *open_paren = strchr(substr_start, '(') + 1;
  char *comma = strchr(substr_start, ',');
  char *close_paren = strchr(substr_start, ')');

  ssize_t left_len = comma - open_paren;
  char *left_str = malloc(left_len + 1);
  if (left_str == NULL) {
    perror("Malloc failed to alloc memory");
    exit(1);
  }
  // get the string from the opening parentheses to the comma.
  // e.g mul(li457, 456) would result in left_str = "li457"
  slice(left_str, open_paren, 0, left_len);

  char *endp = NULL;
  int left = strtol(left_str, &endp, 10);
  // attempt to convert the left_str to a long int
  // - check if strtol consumed the entire string (endp will point to non-numeric
  // characters so check with *endp != '\0')
  // - check that strtol converted something (endp will be the same as left_str if
  // not)
  //
  // e.g mul(li457, 456) will be fail for the left_str as there are non-numeric
  // characters, therefore *endp != '\0' and endp == left_str as it does not
  // start with a digit.
  if (*endp != '\0' || endp == left_str) {
    free(left_str);
    return -1;
  }
  free(left_str);

  ssize_t right_len = close_paren - comma - 1;
  char *right_str = malloc(right_len + 1);
  if (right_str == NULL) {
    perror("Malloc failed to alloc memory");
    exit(1);
  }
  // get the string from the opening parentheses to the comma.
  // e.g mul(li457, 456) would result in right_str = "456"
  slice(right_str, comma + 1, 0, right_len);

  endp = NULL;
  int right = strtol(right_str, &endp, 10);
  // attempt to convert the left_str to a long int
  // - check if strtol consumed the entire string (endp will point to non-numeric
  // characters so check with *endp != '\0')
  // - check that strtol converted something (endp will be the same as left_str if
  // not)
  //
  // e.g mul(li457, 456) will be fine for the right_str as there are no
  // non-numeric characters in the string so *endp == '\0' and endp != right_str
  if (*endp != '\0' || endp == right_str) {
    free(right_str);
    return -1;
  }

  free(right_str);
  return left * right;
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

int calculate_result_of_muls_part2(char **rows, int lines)
{
  int *valid_muls = NULL;
  int num_muls;
  int valid_muls_counter = 0;
  int mul_active = 1;
  for (int i = 0; i < lines; i++) {
    int row_len = strlen(rows[i]);
    printf("mul_active: %d\n", mul_active);
    num_muls = get_valid_muls_part2(rows[i], row_len, &valid_muls,
                                    &valid_muls_counter, &mul_active);

    if (num_muls < 0) {
      continue;
    }
    valid_muls_counter += num_muls;
  }
  int result = 0;
  if (valid_muls != NULL) {
    for (int i = 0; i < valid_muls_counter; i++) {
      result += valid_muls[i];
    }
    free(valid_muls);
    valid_muls = NULL;
  }
  return result;
}

int get_valid_muls_part2(const char *row, int length, int **valid_muls,
                         int *len_valid_muls, int *mul_active)
{
  int num_mul = 0;
  int pos = 0;
  while (pos < length) {
    /*     printf("row + pos: %s\n", row + pos); */
    // check for the start of a valid mul instruction
    char *substr = strstr(row + pos, "mul(");
    if (substr == NULL) // if no mul instructions were found, then we can break
    {
      fprintf(stderr, "No mul instructions\n");
      break;
    }

    // get the distance from the current pos to the start of the mul instruction
    // substring
    int dist_to_mul = substr - (row + pos);
    char *pos_to_mul_substr = malloc(dist_to_mul + 1);
    if (pos_to_mul_substr == NULL) {
      perror("Malloc failed to alloc memory");
      exit(1);
    }
    slice(pos_to_mul_substr, row + pos, 0, dist_to_mul);

    // search in reverse through pos_to_mul_substr for the do() instruction
    char *do_mul = rev_strstr(pos_to_mul_substr, "do()");
    // search in reverse through pos_to_mul_substr for the don't() instruction
    char *dont_mul = rev_strstr(pos_to_mul_substr, "don't()");

    // if only a do() instruction was found before mul instruction, set
    // mul_active to 1
    if (dont_mul && (!do_mul || dont_mul > do_mul)) {
      *mul_active = 0;
    } else if (do_mul) {
      *mul_active = 1;
    }

    // check for the end of a valid mul instruction
    char *substr_end = strstr(substr, ")");
    if (substr_end == NULL) // if no end to a mul struction was found, it is
    // invalid and we can move on
    {
      fprintf(stderr, "Unmatched parenthesis for mul instruction\n");
      valid_muls = NULL;
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
    next_mul = strstr(valid_mul + 1, ",");
    if (next_mul == NULL) {
      fprintf(stderr, "Comma not found within mul instruction: %s\n",
              valid_mul);
      // free valid_mul before going to next iteration
      free(valid_mul);
      pos = substr + 4 - row;
      continue;
    }
    int calc_result = get_digits_and_mul(substr, substr_end);
    if (calc_result == -1) {
      fprintf(stderr,
              "one side of mul instruction contains invalid digits: %s\n",
              valid_mul);
      // free valid_mul before going to next iteration
      free(valid_mul);
      pos = substr + 4 - row;
      continue;
    }

    if (*mul_active == 1) {
      num_mul++;
      *valid_muls =
        realloc(*valid_muls, (*len_valid_muls + num_mul) * sizeof(int));
      if (valid_muls == NULL) {
        perror("realloc failed to alloc memory");
        exit(1);
      }
      (*valid_muls)[(*len_valid_muls + num_mul) - 1] = calc_result;
      pos = substr_end - row + 1;
    } else {
      pos = substr + 4 - row;
    }
    free(valid_mul);
  }
  return num_mul;
}

char *rev_strstr(const char *haystack, const char *needle)
{
  const char *last = NULL;
  const char *current = haystack;

  if (*needle == '\0') {
    return (char *)haystack + strlen(haystack);
  }

  while ((current = strstr(current, needle)) != NULL) {
    last = current;
    current++;
  }

  return (char *)last;
}

void slice(char *result, const char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
  result[end] = '\0';
}
