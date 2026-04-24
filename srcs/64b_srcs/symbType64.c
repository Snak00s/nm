#include "nm.h"

char	symbType64(t_symbol64 *symb, Elf64_Shdr *sections)
{
	char c = '?';

	Elf64_Shdr sec = sections[symb->shndx];
	// printf("\ntype : %d, flags : %ld, bind : %d %s\n", sec.sh_type, sec.sh_flags, ELF64_ST_BIND(symb->info), symb->name);

	if (symb->shndx == SHN_ABS)
		c = 'A';
	else if (sec.sh_type == SHT_NOBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
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