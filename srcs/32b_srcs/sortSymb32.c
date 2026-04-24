#include"nm.h"

void	sortSymb32(t_symbol32 **symb, unsigned long size)
{
	unsigned long i = 0;
	while (i < size)
	{
		unsigned long j = i + 1;
		while (j < size)
		{
			char *si = ft_strtrim(symb[i]->name, "_");
			char *sj = ft_strtrim(symb[j]->name, "_");
			if (ft_strncmp(strDecapitalize(si), strDecapitalize(sj), ft_strlen(si)) > 0)
				swapSymb((void *)&symb[i], (void *)&symb[j]);
			free(si);
			free(sj);
			j++;
		}
		i++;
	}
	return ;
}