#include "../utils/utils.h"
#include <stdio.h>
#include <string.h>

int solve_part1(char *input, ssize_t length);

int main()
{
  char test_input[] =
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
  int test_input_length = strlen(test_input);
  printf("part1 result!!! %d\n", solve_part1(test_input, test_input_length));
}

int solve_part1(char *input, ssize_t input_length)
{
  int result = 0;

  return result;
}
