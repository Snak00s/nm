#include"nm.h"

void	sortSymb64(t_symbol64 **symb, unsigned long size)
{
	unsigned long i = 0;
	while (i < size)
	{
		unsigned long j = i + 1;
		while (j < size)
		{
			char *si = symb[i]->lowTrimName;
			char *sj = symb[j]->lowTrimName;
			int cmp = ft_strncmpletter(si, sj, ft_strlen(si));
			if (cmp > 0)
				swapSymb((void *)&symb[i], (void *)&symb[j]);
			j++;
		}
		i++;
	}
	return ;
}