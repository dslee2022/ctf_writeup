#include <stdio.h>
//#include <rand.h>

int main()
{
	srand(time(0));
	for (;;)
	{
		int v1 = time(0);
		printf("%d\n", v1);
	}
}
