#include "nm.h"

static char bindingConversion(t_symbol64 *symb, char c)
{
	if (ELF64_ST_BIND(symb->info) == STB_WEAK) //can be an weak object (v) or weak symbol (w) 
	{
		if (ELF64_ST_TYPE(symb->info) == STT_OBJECT)
			c = (symb->shndx == SHN_UNDEF ? 'v' : 'V');
		else
			c = (symb->shndx == SHN_UNDEF ? 'w' : 'W');
	}

	if (c != '?' && ELF64_ST_BIND(symb->info) == STB_LOCAL)
		c += 32;
	return (c);
}

char	symbType64(t_symbol64 *symb, Elf64_Shdr *sections)
{
	char c = '?';

	if (symb->shndx == SHN_ABS)
		return (bindingConversion(symb, 'A'));
	else if (symb->shndx == SHN_COMMON)
		return (bindingConversion(symb, 'C'));

	Elf64_Shdr sec = sections[symb->shndx];

	if (sec.sh_type == SHT_NOBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		c = 'B';
	else if ((sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE))
		|| sec.sh_type == SHT_INIT_ARRAY || sec.sh_type == SHT_FINI_ARRAY || sec.sh_type == SHT_DYNAMIC) //.data ou .data1
		c = 'D';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == SHF_ALLOC) // .rodata .rodata1
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // .text
		c = 'T';
	else if (sec.sh_type == SHT_NOTE) //.note
		c = 'R';
	else
		c = 'U'; //unknown

	c = bindingConversion(symb, c);
	return (c);
}