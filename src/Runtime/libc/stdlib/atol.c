#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

long atol(const char* str)
{
	long val = 0;
	bool neg = false;

	while(isspace(*str))
	{
		str++;
	}

	switch(*str)
	{
		case '-':
			neg = true;
			/* fall through */
		case '+':
			str++;
		default:
			break;
	}

	while(isdigit(*str))
	{
		val = (10 * val) + (*str++ - '0');
	}

	return neg ? -val : val;
}
