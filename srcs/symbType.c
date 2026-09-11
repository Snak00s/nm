#include "nm.h"

static int isTypeGNU(uint32_t type)
{
	if (type == SHT_GNU_ATTRIBUTES || type == SHT_GNU_HASH
		|| type == SHT_GNU_LIBLIST || type == SHT_GNU_verdef
		|| type == SHT_GNU_verneed || type == SHT_GNU_versym)
		return (1);
	return (0);
}

static char bindingConversion32(t_symbol32 *symb, char c)
{
	if (ELF32_ST_BIND(symb->info) == STB_WEAK) //can be an weak object (v) or weak symbol (w) 
	{
		if (ELF32_ST_TYPE(symb->info) == STT_OBJECT)
			c = (symb->shndx == SHN_UNDEF ? 'v' : 'V');
		else
			c = (symb->shndx == SHN_UNDEF ? 'w' : 'W');
	}

	if (ELF32_ST_TYPE(symb->info) == STT_GNU_IFUNC)
		c = 'i';

	if (c != '?' && ELF32_ST_BIND(symb->info) == STB_LOCAL)
		c += 32;
	return (c);
}

char	symbType32(t_symbol32 *symb, Elf32_Shdr *sections)
{
	char c = '?';

	if (symb->shndx == SHN_ABS)
		return (bindingConversion32(symb, 'A'));
	else if (symb->shndx == SHN_COMMON)
		return (bindingConversion32(symb, 'C'));

	Elf32_Shdr sec = sections[symb->shndx];

	if (sec.sh_type == SHT_NOBITS && (sec.sh_flags == (SHF_ALLOC | SHF_WRITE) //.bss
		|| sec.sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS)))
		c = 'B';
	else if ((sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE))
		|| sec.sh_type == SHT_INIT_ARRAY || sec.sh_type == SHT_FINI_ARRAY || sec.sh_type == SHT_DYNAMIC
		|| (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))) //.data ou .data1
		c = 'D';
	else if (isTypeGNU(sec.sh_type) || sec.sh_type == SHT_RELA
		|| sec.sh_type == SHT_DYNSYM || sec.sh_type == SHT_STRTAB
		|| (sec.sh_type == SHT_PROGBITS && sec.sh_flags == SHF_ALLOC)
		|| (sec.sh_type == SHT_REL && sec.sh_flags == SHF_ALLOC)) // .rodata .rodata1
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // .text
		c = 'T';
	else if (sec.sh_type == SHT_NOTE) //.note
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS)
		c = 'N';
	else
		c = 'U'; //unknown

	if (ft_strncmp(symb->name, ".debug", ft_strlen(".debug")))
		c = bindingConversion32(symb, c);
	return (c);
}

static char bindingConversion64(t_symbol64 *symb, char c)
{
	if (ELF64_ST_BIND(symb->info) == STB_WEAK) //can be an weak object (v) or weak symbol (w) 
	{
		if (ELF64_ST_TYPE(symb->info) == STT_OBJECT)
			c = (symb->shndx == SHN_UNDEF ? 'v' : 'V');
		else
			c = (symb->shndx == SHN_UNDEF ? 'w' : 'W');
	}

	if (ELF64_ST_TYPE(symb->info) == STT_GNU_IFUNC)
		c = 'i';

	if (c != '?' && ELF64_ST_BIND(symb->info) == STB_LOCAL)
		c += 32;
	return (c);
}

char	symbType64(t_symbol64 *symb, Elf64_Shdr *sections)
{
	char c = '?';

	if (symb->shndx == SHN_ABS)
		return (bindingConversion64(symb, 'A'));
	else if (symb->shndx == SHN_COMMON)
		return (bindingConversion64(symb, 'C'));

	Elf64_Shdr sec = sections[symb->shndx];

	if (sec.sh_type == SHT_NOBITS && (sec.sh_flags == (SHF_ALLOC | SHF_WRITE) //.bss
		|| sec.sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS)))
		c = 'B';
	else if ((sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE))
		|| sec.sh_type == SHT_INIT_ARRAY || sec.sh_type == SHT_FINI_ARRAY || sec.sh_type == SHT_DYNAMIC
		|| (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))) //.data ou .data1
		c = 'D';
	else if (isTypeGNU(sec.sh_type) || sec.sh_type == SHT_RELA
		|| sec.sh_type == SHT_DYNSYM || sec.sh_type == SHT_STRTAB
		|| (sec.sh_type == SHT_PROGBITS && sec.sh_flags == SHF_ALLOC)
		|| (sec.sh_type == SHT_REL && sec.sh_flags == SHF_ALLOC)) // .rodata .rodata1
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // .text
		c = 'T';
	else if (sec.sh_type == SHT_NOTE) //.note
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS)
		c = 'N';
	else
		c = 'U'; //unknown

	if (ft_strncmp(symb->name, ".debug", ft_strlen(".debug")))
		c = bindingConversion64(symb, c);
	return (c);
}