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
#include"libft/libft.h"
#include"libft/get_next_line_bonus.h"

typedef struct symbol {
	char		*name;
	char		*value;
	unsigned char	info;
	Elf64_Section	shndx;
}	t_symbol;

char	*symbValueFormat(uint64_t symbValue);
void	sortSymb(t_symbol **symb, unsigned long size);

#endif