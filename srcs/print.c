#include "nm.h"

static void printSymb32(t_symbol32 *symb)
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
				printSymb32(symb[i++]);
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
				int j = i;
				while (j - 1 >= 0 && !strcoll(symb[j]->name, symb[j - 1]->name))
					j--;
				if (j != i)
				{
					int g = i - j;
					while (j != i)
					{
						if (!applyFlags(symb[j]->type, gFlag, uFlag))
							printSymb32(symb[j]);
						j++;
					}
					if (!applyFlags(symb[j]->type, gFlag, uFlag))
						printSymb32(symb[j]);
					i -= g + 1;
				}
				else
					printSymb32(symb[i--]);
			}
		}
	}
}

static void printSymb64(t_symbol64 *symb)
{
	if (symb->type != 'w' && symb->type != 'W' && symb->type != 'U')
		!symb->value ? write(1, "0000000000000000", 16) : write(1, symb->value, 16);
	else if (symb->type != 'U')
		!symb->value ? write(1, "                ", 16) : write(1, symb->value, 16);
	else
		write(1, "                ", 16);
	write(1, " ", 1);
	write(1, &symb->type, 1);
	write(1, " ", 1);
	write(1, symb->name, ft_strlen(symb->name));
	write(1, "\n", 1);
}

void	displaySymb64(t_symbol64 **symb, unsigned long size, char *flagList)
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
				printSymb64(symb[i++]);
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
				int j = i;
				while (j - 1 >= 0 && !strcoll(symb[j]->name, symb[j - 1]->name))
					j--;
				if (j != i)
				{
					int g = i - j;
					while (j != i)
					{
						if (!applyFlags(symb[j]->type, gFlag, uFlag))
							printSymb64(symb[j]);
						j++;
					}
					if (!applyFlags(symb[j]->type, gFlag, uFlag))
						printSymb64(symb[j]);
					i -= g + 1;
				}
				else
					printSymb64(symb[i--]);
			}
		}
	}
}