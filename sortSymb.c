#include"nm.h"

void	swapSymb(t_symbol **symb1, t_symbol **symb2)
{
	t_symbol *temp = *symb1;
	*symb1 = *symb2;
	*symb2 = temp;
}

char	*strDecapitalize(char *str)
{
	int i = 0;
	while (str[i])
	{
		char c = str[i];
		if (c >= 65 && c <= 90)
			str[i] = c + 32;
		i++;
	}
	return (str);
}

void	sortSymb(t_symbol **symb, unsigned long size)
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
				swapSymb(&symb[i], &symb[j]);
			free(si);
			free(sj);
			j++;
		}
		i++;
	}
	return ;
}