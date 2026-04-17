#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<elf.h>
#include<ctype.h>
#include"libft/libft.h"
#include"libft/get_next_line_bonus.h"

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

char *symbValueFormat(uint64_t symbValue)
{
	char set[] = "0123456789abcdef";
	char sample[100];
	char *ret;

	int i = 0;
	uint64_t val = symbValue;
	while (val)
	{
		uint64_t reste = val % 16;
		sample[i++] = set[reste];
		val /= 16;
	}
	if (i == 0)
		return("");
	hexaton(sample, i - 1);
	if (i < 16)
	{
		ret = calloc(17, sizeof(char));
		if (!ret)
			return (NULL);
		int r_idx = 0;
		int s_idx = 0;
		while (r_idx < 16)
		{
			if (r_idx < 16 - i)
				ret[r_idx++] = '0';
			else
				ret[r_idx++] = sample[s_idx++];
		}
		return(ret);
	}
	else
		ret = ft_strdup(sample);
	return (ret);
}

int main(void)
{
	// uint64_t nbr = 9957845147789;
	// printf("%s\n", symbValueFormat(nbr));
	// return (0);
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
		if (!strcmp((char *)shstrtab + sections[i].sh_name, ".strtab"))
			strtab = map_start + sections[i].sh_offset;
		if (!strcmp((char *)shstrtab + sections[i].sh_name, ".symtab"))
		{
			symtab = map_start + sections[i].sh_offset;
			nbr_entry = sections[i].sh_size / sections[i].sh_entsize;
		}
	}

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		if (symtab[i].st_info != 4)
			printf("%16s %d %s\n", symbValueFormat(symtab[i].st_value), ELF64_ST_TYPE(symtab[i].st_info), (char *)strtab + symtab[i].st_name);
	}

	munmap(map_start, fdstat.st_size);
	return (0);
}