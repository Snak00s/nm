#include "nm.h"

unsigned long	trueSymbSize(Elf64_Sym *symtab, unsigned long nbr_entry)
{
	unsigned long	size = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		if (symtab[i].st_info != 4)
			size++;
	}
	return (size);
}

t_symbol	**symbCreate(Elf64_Sym *symtab, Elf64_Sym *strtab, unsigned long nbr_entry)
{
	t_symbol **ret = ft_calloc(trueSymbSize(symtab, nbr_entry), sizeof(t_symbol *));
	if (!ret)
		return (NULL);

	unsigned long ret_idx = 0;
	
	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		if (symtab[i].st_info != 4)
		{
			ret[ret_idx] = ft_calloc(1, sizeof(t_symbol));
			if (!ret[ret_idx])
				return (NULL);
			ret[ret_idx]->name = ft_strdup((char *)strtab + symtab[i].st_name);
			if (!(ret[ret_idx]->name))
				return (NULL);
			ret[ret_idx]->value = symbValueFormat(symtab[i].st_value);
			ret[ret_idx]->info = symtab[i].st_info;
			ret[ret_idx]->shndx = symtab[i].st_shndx;
			ret_idx++;
		}
	}
	return (ret);
}

// char            print_type(t_symbol *sym, Elf64_Shdr *shdr)
// {
// 	char  c;

// 	if (ELF64_ST_BIND(sym->info) == STB_GNU_UNIQUE)
// 	c = 'u';
// 	else if (ELF64_ST_BIND(sym->info) == STB_WEAK)
// 	{
// 		c = 'W';
// 		if (sym->shndx == SHN_UNDEF)
// 		c = 'w';
// 	}
// 	else if (ELF64_ST_BIND(sym->info) == STB_WEAK && ELF64_ST_TYPE(sym->info) == STT_OBJECT)
// 	{
// 		c = 'V';
// 		if (sym->shndx == SHN_UNDEF)
// 		c = 'v';
// 	}
// 	else if (sym->shndx == SHN_UNDEF)
// 	c = 'U';
// 	else if (sym->shndx == SHN_ABS)
// 	c = 'A';
// 	else if (sym->shndx == SHN_COMMON)
// 	c = 'C';
// 	else if (shdr[sym->shndx].sh_type == SHT_NOBITS
// 		&& shdr[sym->shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
// 	c = 'B';
// 	else if (shdr[sym->shndx].sh_type == SHT_PROGBITS
// 		&& shdr[sym->shndx].sh_flags == SHF_ALLOC)
// 	c = 'R';
// 	else if (shdr[sym->shndx].sh_type == SHT_PROGBITS
// 		&& shdr[sym->shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
// 	c = 'D';
// 	else if (shdr[sym->shndx].sh_type == SHT_PROGBITS
// 		&& shdr[sym->shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
// 	c = 'T';
// 	else if (shdr[sym->shndx].sh_type == SHT_DYNAMIC)
// 	c = 'D';
// 	else
// 	c = '?';
// 	if (ELF64_ST_BIND(sym->info) == STB_LOCAL && c != '?')
// 	c += 32;
// 	return c;
// }

char	symbType(t_symbol *symb, Elf64_Shdr *sections)
{
	char c = ' ';

	Elf64_Shdr sec = sections[symb->shndx];
	if (sec.sh_type == SHT_NOBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		c = 'B';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_WRITE)) //.data ou .data1
		c = 'D';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == SHF_ALLOC) // .rodata .rodata1
		c = 'R';
	else if (sec.sh_type == SHT_PROGBITS && sec.sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // .text
		c = 'T';
	else if (sec.sh_type == SHT_DYNAMIC)
		c = 'D';
	else if (sec.sh_type == SHT_INIT_ARRAY)
		c = 'D';
	else if (sec.sh_type == SHT_NOTE)
		c = 'R';
	else
		c = 'U';

	if (ELF64_ST_BIND(symb->info) == STB_WEAK)
		c = 'W';

	if (c != ' ' && ELF64_ST_BIND(symb->info) == STB_LOCAL)
		c += 32;
	// printf ("\nsh_type : %d, sh_flags : %ld, weak local :%d, weak global: %d\n", sec.sh_type, sec.sh_flags, (ELF64_ST_BIND(symb->info) == (STB_WEAK | STB_LOCAL)), (ELF64_ST_BIND(symb->info) == (STB_WEAK | STB_GLOBAL)));
	return (c);
}

int main(void)
{
	struct stat fdstat;

	int fd = open("a.out", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}

	if (fstat(fd, &fdstat) == -1)
	{
		perror("fstat");
		return (1);
	}

	void *map_start = mmap(NULL, fdstat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (map_start == MAP_FAILED)
	{
		perror("mmap");
		return (1);
	}

	Elf64_Ehdr	*header = (Elf64_Ehdr *)map_start;
	Elf64_Shdr	*sections = (Elf64_Shdr *)(map_start + header->e_shoff);

	int shstrtabIdx = header->e_shstrndx;
	if (header->e_shstrndx >= SHN_LORESERVE)
		shstrtabIdx = sections[0].sh_link;

	Elf64_Sym *shstrtab = (map_start + sections[shstrtabIdx].sh_offset);

	Elf64_Sym *symtab = NULL;
	Elf64_Sym *strtab = NULL;

	unsigned long nbr_entry = 0;

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (!ft_strncmp((char *)shstrtab + sections[i].sh_name, ".strtab", ft_strlen(".strtab")))
			strtab = map_start + sections[i].sh_offset;
		if (!ft_strncmp((char *)shstrtab + sections[i].sh_name, ".symtab", ft_strlen(".symtab")))
		{
			symtab = map_start + sections[i].sh_offset;
			nbr_entry = sections[i].sh_size / sections[i].sh_entsize;
		}
	}

	t_symbol **symb = symbCreate(symtab, strtab, nbr_entry);
	if (!symb)
	{
		printf("symbcreate error");
		return (0);
	}

	unsigned long i = 0;
	unsigned long trueSize = trueSymbSize(symtab, nbr_entry);
	sortSymb(symb, trueSize);
	while(i < trueSize)
	{
		char c = '0';
		
		c = symbType(symb[i], sections);
		
		printf("%16s %c %s\n", symb[i]->value, c, symb[i]->name);
		i++;
	}

	for (unsigned long j = 0; j < trueSize; j++)
	{
		free(symb[j]->name);
		if (symb[j]->value)
			free(symb[j]->value);
		free(symb[j]);
	}
	free(symb);

	munmap(map_start, fdstat.st_size);
	return (0);
}