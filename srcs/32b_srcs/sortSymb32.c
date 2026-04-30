#include"nm.h"

void	sortSymb32(t_symbol32 **symb, unsigned long size, int rFlag)
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
			else if (cmp == 0)
			{
				if (cmpType(symb[i]->type, symb[j]->type))
					swapSymb((void *)&symb[i], (void *)&symb[j]);
				else if ((symb[i]->type == symb[j]->type) && (symb[i]->rawValue > symb[j]->rawValue) && !rFlag)
					swapSymb((void *)&symb[i], (void *)&symb[j]);
			}
			j++;
		}
		i++;
	}
	return ;
}