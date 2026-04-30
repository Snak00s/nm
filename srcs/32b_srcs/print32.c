#include "nm.h"

static void printSymb(t_symbol32 *symb)
{
	if (symb->type != 'w' && symb->type != 'W' && symb->type != 'U')
		!symb->value ? write(1, "00000000", 8) : write(1, symb->value, 8);
	else if (symb->type != 'U')
		!symb->value ? write(1, "        ", 8) : write(1, symb->value, 8);
	else
		write(1, "        ", 8);
	write(1, " ", 1);
	write(1, &symb->type, 1);
	write(1, " ", 1);
	write(1, symb->name, ft_strlen(symb->name));
	write(1, "\n", 1);
}

void	displaySymb32(t_symbol32 **symb, unsigned long size, char *flagList)
{
	int aFlag = 0;
	int gFlag = 0;
	int uFlag = 0;
	int rFlag = 0;
	int pFlag = 0;

	checkFlag2(flagList, &aFlag, &uFlag, &gFlag, &rFlag, &pFlag);
	if (pFlag || !rFlag)
	{
		unsigned long i = 0;
		while(i < size)
		{
			if (applyFlags(symb[i]->type, gFlag, uFlag))
				i++;
			else
				printSymb(symb[i++]);
		}
	}
	else
	{
		long i = size - 1;
		while(i >= 0)
		{
			if (applyFlags(symb[i]->type, gFlag, uFlag))
				i--;
			else
			{
				if (i - 1 >= 0 && !ft_strncmp(symb[i]->name, symb[i - 1]->name, ft_strlen(symb[i]->name)) && symb[i]->type == symb[i - 1]->type && symb[i]->rawValue > symb[i - 1]->rawValue)
				{
					printSymb(symb[i - 1]);
					printSymb(symb[i--]);
					i--;
				}
				else
					printSymb(symb[i--]);
			}
		}
	}
}