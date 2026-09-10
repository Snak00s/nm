#include "nm.h"

unsigned long	trueSymbSize32(Elf32_Sym *symtab, unsigned long nbr_entry, int aFlag)
{
	unsigned long	size = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF32_ST_TYPE(symtab[i].st_info);
		if (aFlag || (type != SHT_STRTAB && type != SHT_RELA))
			size++;
	}
	return (size);
}

t_symbol32	**symbCreate32(Elf32_Sym *symtab, Elf32_Sym *strtab, Elf32_Shdr *sections, unsigned long nbr_entry, int aFlag)
{
	t_symbol32 **ret = ft_calloc(trueSymbSize32(symtab, nbr_entry, aFlag), sizeof(t_symbol32 *));
	if (!ret)
		return (NULL);

	unsigned long ret_idx = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF32_ST_TYPE(symtab[i].st_info);
		if (aFlag || (type != SHT_STRTAB && type != SHT_RELA))
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
			ret[ret_idx]->rawValue = symtab[i].st_value;
			ret[ret_idx]->info = symtab[i].st_info;
			ret[ret_idx]->shndx = symtab[i].st_shndx;
			ret[ret_idx]->type = symbType32(ret[ret_idx], sections);
			ret_idx++;
		}
	}
	return (ret);
}

unsigned long	trueSymbSize64(Elf64_Sym *symtab, unsigned long nbr_entry, int aFlag)
{
	unsigned long	size = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF64_ST_TYPE(symtab[i].st_info);
		if (aFlag || (type != SHT_STRTAB && type != SHT_RELA))
			size++;
	}
	return (size);
}

t_symbol64	**symbCreate64(Elf64_Sym *symtab, Elf64_Sym *strtab, Elf64_Shdr *sections,
	unsigned long nbr_entry, Elf64_Shdr *shstrtab, unsigned long trueSize, int shmax, int aFlag)
{
	t_symbol64 **ret = ft_calloc(trueSize, sizeof(t_symbol64 *));
	if (!ret)
		return (NULL);

	unsigned long ret_idx = 0;

	for (unsigned long i = 1; i < nbr_entry; i++)
	{
		int type = ELF64_ST_TYPE(symtab[i].st_info);
		if (aFlag || (type != SHT_STRTAB && type != SHT_RELA))
		{
			ret[ret_idx] = ft_calloc(1, sizeof(t_symbol64));
			if (!ret[ret_idx])
				return (NULL);

			ret[ret_idx]->name = (char *)strtab + symtab[i].st_name;
			if (!ft_strlen(ret[ret_idx]->name) && symtab[i].st_shndx < shmax)
				ret[ret_idx]->name = (char *)shstrtab + (&sections[symtab[i].st_shndx])->sh_name;

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