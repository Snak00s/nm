#include "nm.h"

char *symbValueFormat32(uint32_t symbValue)
{
	char set[] = "0123456789abcdef";
	char sample[100];
	char *ret;

	int i = 0;
	uint32_t val = symbValue;
	while (val)
	{
		uint32_t reste = val % 16;
		sample[i++] = set[reste];
		val /= 16;
	}
	if (i == 0)
		return(NULL);
	hexaton(sample, i - 1);
	if (i < 8)
	{
		ret = ft_calloc(17, sizeof(char));
		if (!ret)
			return (NULL);
		int r_idx = 0;
		int s_idx = 0;
		while (r_idx < 8)
		{
			if (r_idx < 8 - i)
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