#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_number_lines(char *pinput);

void split_input(char *pinput, int *left, int *right);

int main()
{
  char test_input[] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
  printf("%s", test_input);
  int lines = get_number_lines(test_input);
  int *left = calloc(lines, sizeof(int));
  int *right = calloc(lines, sizeof(int));
}

int get_number_lines(char *pinput)
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

void split_input(char *pinput, int *left, int *right)
{
}

