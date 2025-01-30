#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search(const char **rows, ssize_t num_rows);
void solve_part1(const char *pinput, ssize_t input_length);

int get_number_lines(const char *pinput, ssize_t input_length);
void split_rows(const char *pinput, ssize_t input_length, char **rows);
void slice(char *result, const char *source, ssize_t start, ssize_t end);

int main()
{
  char *test_input =
    "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX\n";
  ssize_t test_input_length = strlen(test_input);
  solve_part1(test_input, test_input_length);
}

void solve_part1(const char *pinput, ssize_t input_length)
{
  ssize_t number_lines = get_number_lines(pinput, input_length);
  printf("number lines: %zd\n", number_lines);

  char **rows = malloc(number_lines * sizeof(char *));
  if (rows == NULL) {
    perror("Malloc failed to allocate memory\n");
    exit(-1);
  }
  split_rows(pinput, input_length, rows);
  printf("num_xmas: %d\n", search(rows, number_lines));
  free(rows);
}

int search(const char **rows, ssize_t num_rows)
{
  int cur_row = 0;
  // all rows should be the same length
  int row_length = strlen(rows[0]);
  int xmas_count = 0;

  while (cur_row < num_rows) {
    for (int pos = 0; pos < row_length; pos++) {
      // check if there is an X on the current row
      char *x_pos = strstr(rows[cur_row] + pos, "X");
      if (x_pos == NULL && pos == 0) {
        fprintf(stderr, "No occurrence of X\n");
        break;
      }
      if (x_pos == NULL) {
        fprintf(stderr, "No further occurrences of X\n");
        break;
      }
      // knowing the index of x is helpful below
      int x_index = x_pos - rows[cur_row];

      // check if XMAS occurrs on the same row
      if (strncmp(x_pos - 3, "SAM", 3) == 0) {
        xmas_count++;
      }
      if (strncmp(x_pos + 1, "MAS", 3) == 0) {
        xmas_count++;
      }
      // check if XMAS occurrs on the vertical (same index on row but +- a row)

      // if cur_row is 0 and cur_row+3 is not num_rows we want to check below
      if (cur_row == 0 || cur_row + 3 < num_rows) {
        if (strncmp(&rows[cur_row + 1][x_index], "M", 1) == 0) {
          if (strncmp(&rows[cur_row + 2][x_index], "A", 1) == 0) {
            if (strncmp(&rows[cur_row + 3][x_index], "S", 1) == 0) {
              xmas_count++;
            }
          }
        } else {
          fprintf(stderr, "No xmas occurrence downwards from %d\n", x_index);
        }
        // check if XMAS occurrs on the diagonal (+- index and +- row)

        // if x_index is 0 (leftmost) and x_index+3 is less than or equal to
        // row_length (rightmost) then we want to check the diagonal down and to the right
        if (x_index == 0 || x_index + 3 <= row_length) {
          if (strncmp(&rows[cur_row + 1][x_index + 1], "M", 1) == 0) {
            if (strncmp(&rows[cur_row + 2][x_index + 2], "A", 1) == 0) {
              if (strncmp(&rows[cur_row + 3][x_index + 3], "S", 1) == 0) {
                xmas_count++;
              }
            }
          } else {
            fprintf(stderr,
                    "No xmas occurrence downwards right diagonal from %d\n",
                    x_index);
          }
        }
        if (x_index == row_length || x_index - 3 >= 0) {
          if (strncmp(&rows[cur_row + 1][x_index - 1], "M", 1) == 0) {
            if (strncmp(&rows[cur_row + 2][x_index - 2], "A", 1) == 0) {
              if (strncmp(&rows[cur_row + 3][x_index - 3], "S", 1) == 0) {
                xmas_count++;
              }
            }
          } else {
            fprintf(stderr,
                    "No xmas occurrence downward left diagonal from %d\n",
                    x_index);
          }
        }
      }
      // if cur_row is not num_rows (last row) we want to check above
      if (cur_row == num_rows || cur_row - 3 >= 0) {
        if (strncmp(&rows[cur_row - 1][x_index], "M", 1) == 0) {
          if (strncmp(&rows[cur_row - 2][x_index], "A", 1) == 0) {
            if (strncmp(&rows[cur_row - 3][x_index], "S", 1) == 0) {
              xmas_count++;
            }
          }
        } else {
          fprintf(stderr, "No xmas occurrence upwards from %d\n", x_index);
        }
        // check if XMAS occurrs on the diagonal (+- index and +- row)

        if (x_index == 0 || x_index + 3 <= row_length) {
          if (strncmp(&rows[cur_row - 1][x_index + 1], "M", 1) == 0) {
            if (strncmp(&rows[cur_row - 2][x_index + 2], "A", 1) == 0) {
              if (strncmp(&rows[cur_row - 3][x_index + 3], "S", 1) == 0) {
                xmas_count++;
              }
            }
          } else {
            fprintf(stderr,
                    "No xmas occurrence upwards right diagonal from %d\n",
                    x_index);
          }
        }
        if (x_index == row_length || x_index - 3 >= 0) {
          if (strncmp(&rows[cur_row - 1][x_index - 1], "M", 1) == 0) {
            if (strncmp(&rows[cur_row - 2][x_index - 2], "A", 1) == 0) {
              if (strncmp(&rows[cur_row - 3][x_index - 3], "S", 1) == 0) {
                xmas_count++;
              }
            }
          } else {
            fprintf(stderr,
                    "No xmas occurrence upwards left diagonal from %d\n",
                    x_index);
          }
        }
      }

      pos += x_index;
    }
    cur_row++;
  }
  return xmas_count;
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

void slice(char *result, const char *source, ssize_t start, ssize_t end)
{
  strncpy(result, source + start, end);
  result[end] = '\0';
}
