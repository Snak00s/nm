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

void	swapSymb(void **symb1, void **symb2)
{
	void *temp = *symb1;
	*symb1 = *symb2;
	*symb2 = temp;
}

int		checkFlag(char c, char *flagList)
{
	int i = 0;

	while (flagList && flagList[i])
	{
		if (flagList[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	checkFlag2(char *flagList, int *aFlag, int *uFlag, int *gFlag, int *rFlag, int *pFlag)
{
	int i = 0;

	while (flagList && flagList[i])
	{
		char c = flagList[i];
		switch (c)
		{
		case 'a': *aFlag = 1;
			break;
		case 'u': *uFlag = 1;
			break;
		case 'g': *gFlag = 1;
			break;
		case 'r': *rFlag = 1;
			break;
		case 'p': *pFlag = 1;
			break;
		default:
			break;
		}
		i++;
	}
}

int		applyFlags(unsigned char c, int gFlag, int uFlag)
{
	if (uFlag == 1)
	{
		if (c != 'U' && c != 'w')
			return (1);
	}
	else if (gFlag == 1)
	{
		if ((c >= 97 && c <= 122 && c != 'w' && c != 'i') || c == 'N')
			return (1);
	}
	return (0);
}