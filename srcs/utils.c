#include "nm.h"

void	hexaton(char *hexa, size_t len)
{
	size_t	i = 0;
	while (i <= len / 2)
	{
		char c = hexa[i];
		hexa[i] = hexa[len - i];
		hexa[len - i] = c;
		i++;
	}
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

void	swapSymb(void **symb1, void **symb2)
{
	void *temp = *symb1;
	*symb1 = *symb2;
	*symb2 = temp;
}