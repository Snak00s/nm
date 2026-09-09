#include"nm.h"

void	sortSymb32(t_symbol32 **symb, unsigned long size)
{
	unsigned long i = 0;
	while (i < size)
	{
		unsigned long j = 0;
		while (j < size - 1)
		{
			char *si = symb[j]->name;
			char *sj = symb[j + 1]->name;
			int cmp = strcoll(si, sj);
			if (cmp > 0)
				swapSymb((void *)&symb[j], (void *)&symb[j + 1]);
			j++;
		}
		i++;
	}
	return ;
}

void	sortSymb64(t_symbol64 **symb, unsigned long size)
{
	unsigned long i = 0;
	while (i < size)
	{
		unsigned long j = 0;
		while (j < size - 1)
		{
			char *si = symb[j]->name;
			char *sj = symb[j + 1]->name;
			int cmp = strcoll(si, sj);
			if (cmp > 0)
				swapSymb((void *)&symb[j], (void *)&symb[j + 1]);
			j++;
		}
		i++;
	}
	return ;
}