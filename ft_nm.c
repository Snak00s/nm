#include "nm.h"

unsigned long	trueSymbSize(Elf64_Sym *symtab, unsigned long nbr_entry)
{
	unsigned long	size = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF64_ST_TYPE(symtab[i].st_info);
		if (type != SHT_STRTAB && type != SHT_RELA)
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
		int type = ELF64_ST_TYPE(symtab[i].st_info);
		if (type != SHT_STRTAB && type != SHT_RELA)
		{
			ret[ret_idx] = ft_calloc(1, sizeof(t_symbol));
			if (!ret[ret_idx])
				return (NULL);
			ret[ret_idx]->name = ft_strdup((char *)strtab + symtab[i].st_name);
			if (!(ret[ret_idx]->name))
				return (NULL);
			ret[ret_idx]->value = symbValueFormat(symtab[i].st_value);
			if (symtab[i].st_value != 0 && !ret[ret_idx]->value)
				return (NULL);
			ret[ret_idx]->info = symtab[i].st_info;
			ret[ret_idx]->shndx = symtab[i].st_shndx;
			ret_idx++;
		}
	}
	return (ret);
}

char	symbType(t_symbol *symb, Elf64_Shdr *sections)
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

int main(int argc, char **argv)
{
	struct stat fdstat;

	if (argc > 2)
	{
		write(2, "Invalid amount of argument.\n", 29);
		return (1);
	}

	int fd = argc > 1 ? open(argv[1], O_RDONLY) : open("a.out", O_RDONLY);
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
		munmap(map_start, fdstat.st_size);
		write(2, "symbcreate error.\n", 19);
		return (1);
	}

	unsigned long i = 0;
	unsigned long trueSize = trueSymbSize(symtab, nbr_entry);
	sortSymb(symb, trueSize);
	while(i < trueSize)
	{
		char c;

		c = symbType(symb[i], sections);
		if (c != 'w' && c != 'W' && c != 'U')
			!symb[i]->value ? write(1, "0000000000000000", 16) : write(1, symb[i]->value, 16);
		else
			!symb[i]->value ? write(1, "                ", 16) : write(1, symb[i]->value, 16);
		write(1, " ", 1);
		write(1, &c, 1);
		write(1, " ", 1);
		write(1, symb[i]->name, ft_strlen(symb[i]->name));
		write(1, "\n", 1);
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