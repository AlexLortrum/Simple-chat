#include <cctype>
#include <cstring>
#include "../include/other.h"

int char_to_int(char* number)
{
	int len = strlen(number);
	int res = 0;
	int digit_unit = 1;
	for (int i = len-1; i >= 0; i--)
	{
		if (!std::isdigit(number[i])) return 1;
		res = (((int) number[i] - '0') * digit_unit) + res;
		digit_unit *= 10;
	}
	return res;
}
