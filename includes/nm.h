#ifndef NM_H
# define NM_H

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
#include<locale.h>
#include"../libft/libft.h"
#include"../libft/get_next_line_bonus.h"

typedef struct symbol64 {
	char			*name;
	char			*value;
	Elf64_Addr		rawValue;
	unsigned char	info;
	unsigned char	type;
	Elf64_Section	shndx;
}	t_symbol64;

typedef struct symbol32 {
	char			*name;
	char			*value;
	Elf32_Addr		rawValue;
	unsigned char	info;
	unsigned char	type;
	Elf32_Section	shndx;
}	t_symbol32;

// 64-bits

char			*symbValueFormat64(uint64_t symbValue);
unsigned long	trueSymbSize64(Elf64_Sym *symtab, unsigned long nbr_entry, int aFlag);
t_symbol64		**symbCreate64(Elf64_Sym *symtab, Elf64_Sym *strtab, Elf64_Shdr *sections, unsigned long nbr_entry, Elf64_Shdr *shstrtab, unsigned long trueSize, int shmax, int aFlag);

void			sortSymb64(t_symbol64 **symb, unsigned long size);
char			symbType64(t_symbol64 *symb, Elf64_Shdr *sections);
void			displaySymb64(t_symbol64 **symb, unsigned long size, char *flagList);

// 32-bits

char			*symbValueFormat32(uint32_t symbValue);
unsigned long	trueSymbSize32(Elf32_Sym *symtab, unsigned long nbr_entry, int aFlag);
t_symbol32		**symbCreate32(Elf32_Sym *symtab, Elf32_Sym *strtab, Elf32_Shdr *sections, unsigned long nbr_entry, int aFlag);
void			sortSymb32(t_symbol32 **symb, unsigned long size);
char			symbType32(t_symbol32 *symb, Elf32_Shdr *sections);
void			displaySymb32(t_symbol32 **symb, unsigned long size, char *flagList);

//utils

char			*strDecapitalize(char *str);
void			hexaton(char *hexa, size_t len);
void			swapSymb(void **symb1, void **symb2);
int				ft_strncmpletter(const char *s1, const char *s2, size_t n);
int				cmpType(unsigned char c1, unsigned char c2);
int				checkFlag(char c, char *flagList);
void			checkFlag2(char *flagList, int *aFlag, int *uFlag, int *gFlag, int *rFlag, int *pFlag);
int				applyFlags(unsigned char c, int gFlag, int uFlag);

#endif