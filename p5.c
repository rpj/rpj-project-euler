// real	0m12.501s

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int c = 1;
	int a = 0;
	int top = 21;;

	for (; !a; c++) {
		int i = 1;

		for (; i < top; i++)
			if (c % i) break;

		if (i == top) a = c;
	}

	printf("%d\n", a);
	return 0;
}
