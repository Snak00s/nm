#include "nm.h"

int nmElf64(Elf64_Ehdr *header, void *map_start, char *flagList, char *filename, int multiFile)
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

	t_symbol64 **symb = symbCreate64(symtab, strtab, sections, nbr_entry, checkFlag('a', flagList));
	if (!symb)
	{
		write(2, "symbCreate64 error.\n", 19);
		return (1);
	}

	unsigned long trueSize = trueSymbSize64(symtab, nbr_entry, checkFlag('a', flagList));

	if (!checkFlag('p', flagList))
		sortSymb64(symb, trueSize);
	if (multiFile > 1)
	{
		write(1, "\n", 1);
		write(1, filename, ft_strlen(filename));
		write(1, ":\n", 2);
	}
	displaySymb64(symb, trueSize, flagList);

	for (unsigned long j = 0; j < trueSize; j++)
	{
		if (symb[j]->name)
			free(symb[j]->name);
		if ((symb[j]->lowTrimName))
			free(symb[j]->lowTrimName);
		if (symb[j]->value)
			free(symb[j]->value);
		free(symb[j]);
	}
	free(symb);
	return (0);
}

int	nmElf32(Elf32_Ehdr *header, void *map_start, char *flagList, char *filename, int multiFile)
{
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

	t_symbol32 **symb = symbCreate32(symtab, strtab, sections, nbr_entry, checkFlag('a', flagList));
	if (!symb)
	{
		write(2, "symbCreate32 error.\n", 19);
		return (1);
	}

	unsigned long trueSize = trueSymbSize32(symtab, nbr_entry, checkFlag('a', flagList));
	if (!checkFlag('p', flagList))
		sortSymb32(symb, trueSize);
	if (multiFile > 1)
	{
		write(1, "\n", 1);
		write(1, filename, ft_strlen(filename));
		write(1, ":\n", 2);
	}
	displaySymb32(symb, trueSize, flagList);

	for (unsigned long j = 0; j < trueSize; j++)
	{
		if (symb[j]->name)
			free(symb[j]->name);
		if ((symb[j]->lowTrimName))
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

int initWithArgs(char **flagList, int *file_idx, int *nbrFile, char **argv, int argc)
{
	char allowedFlag[] = "agurp";
	int i = 1;
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
			file_idx[(*nbrFile)++] = i;
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

int	check_ehdr_error(void *map_start, unsigned long file_size)
{

	if (((Elf64_Ehdr *)map_start)->e_ident[EI_CLASS] == 2)
	{
		Elf64_Ehdr *header = (Elf64_Ehdr *)map_start;
		if ((unsigned long)header->e_phoff > file_size
			|| (unsigned long)header->e_shoff > file_size
			|| header->e_shnum == 0)
			return (0);
	}
	else
	{
		Elf32_Ehdr *header = (Elf32_Ehdr *)map_start;
		if ((unsigned long)header->e_phoff > file_size
			|| (unsigned long)header->e_shoff > file_size
			|| header->e_shnum == 0)
			return (0);
	}
	return(1);
}

int nmLoop(char *filename, char *flagList, int multiFile)
{
	struct stat fdstat;
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		char *error = ft_strjoin("ft_nm: \'", filename);
		error = ft_strjoin_free(error, "\'");
		perror(error);
		free(error);
		return(1);
	}

	if (fstat(fd, &fdstat) == -1)
	{
		perror("fstat");
		return(1);
	}

	void *map_start = mmap(NULL, fdstat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (map_start == MAP_FAILED)
	{
		perror("mmap");
		return(1);
	}
	if (close(fd) == -1)
	{
		perror("close");
		munmap(map_start, fdstat.st_size);
		return(1);
	}

	Elf64_Ehdr	*header = (Elf64_Ehdr *)map_start;
	if (ft_memcmp(header->e_ident, ELFMAG, 4) != 0
		|| !check_ehdr_error(map_start, fdstat.st_size))
	{
		char *error = ft_strjoin("ft_nm: ", filename);
		error = ft_strjoin_free(error, ": file format not recognized\n");
		write(2, error, ft_strlen(error));
		free(error);
		return (1);
	}

	if (header->e_ident[EI_CLASS] == 2)
		nmElf64(header, map_start, flagList, filename, multiFile);
	else if (header->e_ident[EI_CLASS] == 1)
		nmElf32((Elf32_Ehdr *)map_start, map_start, flagList, filename, multiFile);
	else
		write(1, "Undefined ELF format.\n", 23);
	if (munmap(map_start, fdstat.st_size) == -1)
	{
		perror("munmap");
		return(1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	int	nbrFile = 0;
	
	int *file_idx = NULL;
	char *flagList = NULL;

	char flagError[] = "ft_nm: invalid option\n \
Usage: nm [option(s)] [file(s)]\n \
 List symbols in [file(s)] (a.out by default).\n \
 The options are:\n \
  -a,	Display debugger-only symbols\n \
  -g,	Display only external symbols\n \
  -p,	Do not sort the symbols\n \
  -r,	Reverse the sense of the sort\n \
  -u,	Display only undefined symbols\n";

	file_idx = ft_calloc(argc, sizeof(int));
	if (!file_idx)
		exit(1);

	if (initWithArgs(&flagList, file_idx, &nbrFile, argv, argc) == 0)
	{
		free(flagList);
		free(file_idx);
		write(2, flagError, ft_strlen(flagError));
		exit(1);
	}

	int i = 0;
	int ret = 0;
	if (nbrFile == 0)
		ret = nmLoop("a.out", flagList, 0);
	while (i < nbrFile && ret == 0)
		ret = nmLoop(argv[file_idx[i++]], flagList, nbrFile);
	free(flagList);
	free(file_idx);
	if (ret != 0)
		exit(1);
	return (0);
}