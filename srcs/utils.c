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

int	ft_strncmpletter(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	s1_offset;
	size_t	s2_offset;

	i = 0;
	s1_offset = 0;
	s2_offset = 0;
	if (n == 0)
		return (0);
	while ((s1[i + s1_offset] || s2[i + s2_offset]) && s1[i + s1_offset] == s2[i + s2_offset] && i < n - 1)
	{
		i++;
		if (s1[i + s1_offset] && !ft_isalnum(s1[i + s1_offset]))
			s1_offset++;
		if (s2[i + s2_offset] && !ft_isalnum(s2[i + s2_offset]))
			s2_offset++;
	}
	return ((unsigned char)s1[i + s1_offset] - (unsigned char)s2[i + s2_offset]);
}

int		cmpType(unsigned char c1, unsigned char c2)
{
	unsigned char c1Modif = c1;
	unsigned char c2Modif = c2;

	if (c1Modif >= 65 && c1Modif <= 90)
		c1Modif += 32;

	if (c2Modif >= 65 && c2Modif <= 90)
		c2Modif += 32;

	if (c1Modif > c2Modif || (c2Modif == c1Modif && c2 > c1))
		return (1);
	return (0);
}