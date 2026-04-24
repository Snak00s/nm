#include "nm.h"

unsigned long	trueSymbSize32(Elf32_Sym *symtab, unsigned long nbr_entry)
{
	unsigned long	size = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF32_ST_TYPE(symtab[i].st_info);
		if (type != SHT_STRTAB && type != SHT_RELA)
			size++;
	}
	return (size);
}

t_symbol32	**symbCreate32(Elf32_Sym *symtab, Elf32_Sym *strtab, unsigned long nbr_entry)
{
	t_symbol32 **ret = ft_calloc(trueSymbSize32(symtab, nbr_entry), sizeof(t_symbol32 *));
	if (!ret)
		return (NULL);

	unsigned long ret_idx = 0;
	
	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF32_ST_TYPE(symtab[i].st_info);
		if (type != SHT_STRTAB && type != SHT_RELA)
		{
			ret[ret_idx] = ft_calloc(1, sizeof(t_symbol32));
			if (!ret[ret_idx])
				return (NULL);
			ret[ret_idx]->name = ft_strdup((char *)strtab + symtab[i].st_name);
			if (!(ret[ret_idx]->name))
				return (NULL);
			ret[ret_idx]->value = symbValueFormat32(symtab[i].st_value);
			if (symtab[i].st_value != 0 && !ret[ret_idx]->value)
				return (NULL);
			ret[ret_idx]->info = symtab[i].st_info;
			ret[ret_idx]->shndx = symtab[i].st_shndx;
			ret_idx++;
		}
	}
	return (ret);
}