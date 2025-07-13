#include <cctype>
#include <cstring>
#include <string>
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

int hash(std::string word, int mul_value)
{
	int res = 0;
	for (int i = 0; i < word.size(); i++)
	{
		res += word[i];
	}
	res *= mul_value;
	return res;
}
