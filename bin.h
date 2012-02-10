#include <cstdio>
#include <cstring>
#include <cstdlib>

template <typename T>
T str_to_bin (char const *n_str)
{
	T b = 0;
	int n_len = strlen(n_str);
	for (int i = 0; i < n_len; i++)
	{
		if (n_str[i] == '1')
		{
			b |= 1 << n_len - 1 - i;
		}
	}
	return b;
}

template <typename T>
void bin_to_str (char *dst, T n)
{
	int j;
	for (int i = sizeof(T) * 8 - 1; i >= 0; i--)
	{
		j = sizeof(T) * 8 - 1 - i;
		if ((n >> i) & 1)
		{
			dst[j] = '1';
		}
		else
		{
			dst[j] = '0';
		}
	}
	dst[j + 1] = '\0';
}

template <typename T>
void puts_bin (T n)
{
	char *str = (char *)malloc(sizeof(T) * 8 + 1);
	bin_to_str(str, n);
	puts(str);
	free(str);
}

