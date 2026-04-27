#include "nm.h"

unsigned long	trueSymbSize64(Elf64_Sym *symtab, unsigned long nbr_entry)
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

t_symbol64	**symbCreate64(Elf64_Sym *symtab, Elf64_Sym *strtab, Elf64_Shdr *sections, unsigned long nbr_entry)
{
	t_symbol64 **ret = ft_calloc(trueSymbSize64(symtab, nbr_entry), sizeof(t_symbol64 *));
	if (!ret)
		return (NULL);

	unsigned long ret_idx = 0;
	
	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF64_ST_TYPE(symtab[i].st_info);
		if (type != SHT_STRTAB && type != SHT_RELA)
		{
			ret[ret_idx] = ft_calloc(1, sizeof(t_symbol64));
			if (!ret[ret_idx])
				return (NULL);
			ret[ret_idx]->name = ft_strdup((char *)strtab + symtab[i].st_name);
			if (!(ret[ret_idx]->name))
				return (NULL);
			ret[ret_idx]->lowTrimName = strDecapitalize(ft_strtrim(ret[ret_idx]->name, "_"));
			if (!(ret[ret_idx]->lowTrimName))
				return (NULL);
			ret[ret_idx]->value = symbValueFormat64(symtab[i].st_value);
			if (symtab[i].st_value != 0 && !ret[ret_idx]->value)
				return (NULL);
			ret[ret_idx]->rawValue = symtab[i].st_value;
			ret[ret_idx]->info = symtab[i].st_info;
			ret[ret_idx]->shndx = symtab[i].st_shndx;
			ret[ret_idx]->type = symbType64(ret[ret_idx], sections);
			ret_idx++;
		}
	}
	return (ret);
}