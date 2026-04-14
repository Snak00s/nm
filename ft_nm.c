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
	// printf("fstat value : %d\n", st);
	// printf("st_atime : %ld\n", fdstat.st_atime);
	// printf("st_blksize : %ld\n", fdstat.st_blksize);
	// printf("st_blocks : %ld\n", fdstat.st_blocks);
	// printf("st_ctime : %ld\n", fdstat.st_ctime);
	// printf("st_dev : %lu\n", fdstat.st_dev);
	// printf("st_gid : %u\n", fdstat.st_gid);
	// printf("st_ino : %lu\n", fdstat.st_ino);
	// printf("st_mode : %u\n", fdstat.st_mode);
	// printf("st_mtime : %ld\n", fdstat.st_mtime);
	// printf("st_nlink : %lu\n", fdstat.st_nlink);
	// printf("st_rdev : %lu\n", fdstat.st_rdev);
	// printf("st_size : %ld\n", fdstat.st_size);
	// printf("st_uid : %u\n", fdstat.st_uid);
	// printf("__glibc_reserved[0] : %ld\n", fdstat.__glibc_reserved[0]);
	// printf("__glibc_reserved[1] : %ld\n", fdstat.__glibc_reserved[1]);
	// printf("__glibc_reserved[2] : %ld\n", fdstat.__glibc_reserved[2]);
	// printf("__pad0 : %d\n", fdstat.__pad0);

	void *map_start = mmap(NULL, fdstat.st_size, PROT_READ, MAP_SHARED, fd, 0);

	printf("%s\n", (char *)map_start);

	Elf64_Ehdr	*header = (Elf64_Ehdr *)map_start;

	// if (memcmp(header->e_ident, ELFMAG, 4) == 0) {
	// 	printf("ELF valide, %d sections\n", header->e_shnum);
	// }

	Elf64_Shdr	*sections;

	sections = (Elf64_Shdr *)map_start;

	Elf64_Sym *symtab;

	printf("%d\n", header->e_shnum);

	for (int i = 0; i < header->e_shnum; i++)
	{
		printf("section %d : %u\n", i, sections[i].sh_type);
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			symtab = (Elf64_Sym *)((char *)map_start + sections[i].sh_offset);
			printf("founded\n");
			break;
		}
	}

	return (0);
}