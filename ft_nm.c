#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<elf.h>

int main(void)
{
	struct stat fdstat;
	int fd = open("a.out", O_RDONLY);
	if (fd == -1)
	{
		printf("feur\n");
		return (0);
	}
	fstat(fd, &fdstat);

	void *map_start = mmap(NULL, fdstat.st_size, PROT_READ, MAP_SHARED, fd, 0);

	Elf64_Ehdr	*header = (Elf64_Ehdr *)map_start;

	char *string = (char *)map_start;

	printf("%1$p : %1$s\n", string + 0x3541);

	// if (memcmp(header->e_ident, ELFMAG, 4) == 0) {
	// 	printf("ELF valide, %d sections\n", header->e_shnum);
	// }

	Elf64_Shdr	*sections = (Elf64_Shdr *)(map_start + header->e_shoff);

	printf("%d\n", header->e_shnum);

	int strtabIdx = header->e_shstrndx;

	if (header->e_shstrndx >= SHN_LORESERVE)
		strtabIdx = sections[0].sh_link;

	// Elf64_Sym *symtab = NULL;

	// for (int i = 0; i < header->e_shnum; i++)
	// {
	// 	// printf("section %d name : %lx | %s\n", i, test->st_value, (char *)(map_start + sections[i].sh_offset) + sections[i].sh_name);
	// 	if (sections[i].sh_type == SHT_SYMTAB)
	// 	{
	// 		symtab = (Elf64_Sym *)(map_start + sections[i].sh_offset);
	// 		printf("index symtab : %d\n", i);
	// 		break;
	// 	}
	// }

	Elf64_Sym *strtab = (map_start + sections[strtabIdx].sh_offset);

	Elf64_Sym *data = NULL;
	for (int i = 0; i < header->e_shnum; i++)
	{
		printf("section[%d] : %s\n", i,  (char *)strtab + sections[i].sh_name);
		if (!strcmp((char *)strtab + sections[i].sh_name, ".data"))
		{
			data = (map_start + sections[i].sh_offset);
		}
	}

	printf("%s\n", (char *)strtab + data->st_name);
	return (0);
}