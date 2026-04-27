#include "nm.h"

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

void	print64(t_symbol64 **symb, unsigned long size, char *flagList)
{
	int aFlag = 0;
	int gFlag = 0;
	int uFlag = 0;

	if (checkFlag('a', flagList))
		aFlag = 1;
	if (checkFlag('g', flagList))
		gFlag = 1;
	if (checkFlag('u', flagList))
		uFlag = 1;

	if (!checkFlag('r', flagList))
	{
		unsigned long i = 0;
		while(i < size)
		{
			if (uFlag == 1)
			{
				if (symb[i]->type != 'U' && symb[i]->type != 'w')
				{
					i++;
					continue;
				}
			}
			else if (gFlag == 1)
			{
				if (symb[i]->type >= 97 && symb[i]->type <= 122 && symb[i]->type != 'w')
				{
					i++;
					continue;
				}
			}
			else if (aFlag)
				;
			if (symb[i]->type != 'w' && symb[i]->type != 'W' && symb[i]->type != 'U')
				!symb[i]->value ? write(1, "0000000000000000", 16) : write(1, symb[i]->value, 16);
			else
				!symb[i]->value ? write(1, "                ", 16) : write(1, symb[i]->value, 16);
			write(1, " ", 1);
			write(1, &symb[i]->type, 1);
			write(1, " ", 1);
			write(1, symb[i]->name, ft_strlen(symb[i]->name));
			write(1, "\n", 1);
			i++;
		}
	}
	else
	{
		long i = size - 1;
		while(i >= 0)
		{
			if (uFlag == 1)
			{
				if (symb[i]->type != 'U' && symb[i]->type != 'w')
				{
					i++;
					continue;
				}
			}
			else if (gFlag == 1)
			{
				if (symb[i]->type >= 97 && symb[i]->type <= 122 && symb[i]->type != 'w')
				{
					i++;
					continue;
				}
			}
			else if (aFlag)
				;
			if (symb[i]->type != 'w' && symb[i]->type != 'W' && symb[i]->type != 'U')
				!symb[i]->value ? write(1, "0000000000000000", 16) : write(1, symb[i]->value, 16);
			else
				!symb[i]->value ? write(1, "                ", 16) : write(1, symb[i]->value, 16);
			write(1, " ", 1);
			write(1, &symb[i]->type, 1);
			write(1, " ", 1);
			write(1, symb[i]->name, ft_strlen(symb[i]->name));
			write(1, "\n", 1);
			i--;
		}
	}
}

int nmElf64(Elf64_Ehdr *header, void *map_start, char *flagList)
{
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

	t_symbol64 **symb = symbCreate64(symtab, strtab, sections, nbr_entry);
	if (!symb)
	{
		write(2, "symbCreate64 error.\n", 19);
		return (1);
	}

	unsigned long trueSize = trueSymbSize64(symtab, nbr_entry);

	if (!checkFlag('p', flagList))
		sortSymb64(symb, trueSize);
	print64(symb, trueSize, flagList);

	for (unsigned long j = 0; j < trueSize; j++)
	{
		free(symb[j]->name);
		free(symb[j]->lowTrimName);
		if (symb[j]->value)
			free(symb[j]->value);
		free(symb[j]);
	}
	free(symb);
	return (0);
}

int	nmElf32(Elf32_Ehdr *header, void *map_start, char *flagList)
{
	(void)flagList;
	Elf32_Shdr *sections = (Elf32_Shdr *)(map_start + header->e_shoff);

	int shstrtabIdx = header->e_shstrndx;
	if (header->e_shstrndx >= SHN_LORESERVE)
		shstrtabIdx = sections[0].sh_link;

	Elf32_Sym *shstrtab = (map_start + sections[shstrtabIdx].sh_offset);
	Elf32_Sym *symtab = NULL;
	Elf32_Sym *strtab = NULL;
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

	t_symbol32 **symb = symbCreate32(symtab, strtab, sections, nbr_entry);
	if (!symb)
	{
		write(2, "symbCreate32 error.\n", 19);
		return (1);
	}

	unsigned long i = 0;
	unsigned long trueSize = trueSymbSize32(symtab, nbr_entry);
	sortSymb32(symb, trueSize);
	while(i < trueSize)
	{
		if (symb[i]->type != 'w' && symb[i]->type != 'W' && symb[i]->type != 'U')
			!symb[i]->value ? write(1, "00000000", 8) : write(1, symb[i]->value, 8);
		else
			!symb[i]->value ? write(1, "        ", 8) : write(1, symb[i]->value, 8);
		write(1, " ", 1);
		write(1, &symb[i]->type, 1);
		write(1, " ", 1);
		write(1, symb[i]->name, ft_strlen(symb[i]->name));
		write(1, "\n", 1);
		i++;
	}

	for (unsigned long j = 0; j < trueSize; j++)
	{
		free(symb[j]->name);
		free(symb[j]->lowTrimName);
		if (symb[j]->value)
			free(symb[j]->value);
		free(symb[j]);
	}
	free(symb);
	return (0);
}

int isFLag(char c, char *allowedFlag)
{
	int i = 0;
	while (allowedFlag[i])
	{
		if (c == allowedFlag[i++])
			return (1);
	}
	return (0);
}

int initWithArgs(char **flagList, int *file_idx, char **argv, int argc)
{
	char allowedFlag[] = "agurp";
	int i = 1;
	(void)argc;
	char *str = NULL;
	while (i < argc)
	{
		if (*argv[i] == '-')
		{
			if (*(argv[i] + 1) != '\0')
				str = ft_strjoin_free(str, argv[i] + 1);
			else
				return (free(str), 0);
		}
		else
			*file_idx = i;
		i++;
	}

	i = 0;
	while (str && str[i])
	{
		if (isFLag(str[i], allowedFlag) == 0)
			return (free(str), 0);
		i++;
	}
	*flagList = str;
	return (1);
}

int main(int argc, char **argv)
{
	struct stat fdstat;
	int file_idx = -1;

	char *flagList = NULL;

	if (initWithArgs(&flagList, &file_idx, argv, argc) == 0)
	{
		free(flagList);
		write(2, "Invalid flag\n", 14);
		return (0);
	}

	int fd = file_idx != -1 ? open(argv[file_idx], O_RDONLY) : open("a.out", O_RDONLY);
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
	if (close(fd) == -1)
	{
		perror("close");
		munmap(map_start, fdstat.st_size);
		return (1);
	}

	Elf64_Ehdr	*header = (Elf64_Ehdr *)map_start;
	if (ft_memcmp(header->e_ident, ELFMAG, 4) != 0) {
		write(1, "File is not in ELF format.\n", 28);
	}
	if (header->e_ident[EI_CLASS] == 2)
		nmElf64(header, map_start, flagList);
	else if (header->e_ident[EI_CLASS] == 1)
		nmElf32((Elf32_Ehdr *)map_start, map_start, flagList);
	else
		write(1, "Undefined ELF format.\n", 23);

	free(flagList);
	if (munmap(map_start, fdstat.st_size) == -1)
	{
		perror("munmap");
		return(1);
	}
	return (0);
}