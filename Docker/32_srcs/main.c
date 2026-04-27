#include"omg.h"

int main(void)
{
	srand(time(0));
	int i = 0;

	int max = rand() % 100;
	while (i < max)
	{
		printRand();
	}
	return (0);
}