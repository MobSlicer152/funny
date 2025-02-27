#include "libc.h"
#include "base/math.h"
#include "base/timer.h"
#include "macros.h"

// i've written a better memcpy and memset before, but this is easy and fast enough (they also use these for some cases)

void* memcpy(void* dest, const void* src, usize size)
{
	if ((uptr)src == (uptr)dest)
	{
		return dest;
	}

	asm("pushf\n"
		"cld\n"
		"rep movsb\n"
		"popf\n"
		:
		: "D"(dest), "S"(src), "c"(size)
		: "memory");
	return dest;
}

void* memset(void* dest, s32 value, usize size)
{
	asm("pushf\n"
		"cld\n"
		"rep stosb\n"
		"popf\n"
		:
		: "D"(dest), "a"(value), "c"(size));
	return dest;
}

void* memmove(void* dest, const void* src, usize size)
{
	if ((uptr)dest > (uptr)src)
	{
		// copying in reverse prevents overwrite
		asm("pushf\n"
			"std\n" // reverse
			"rep movsb\n"
			"popf\n"
			:
			: "D"(dest + size - 1), "S"(src + size - 1), "c"(size)
			: "memory");
		return dest;
	}
	else
	{
		return memcpy(dest, src, size);
	}
}

int memcmp(const void* a, const void* b, usize size)
{
	if ((uptr)a == (uptr)b)
	{
		return 0;
	}

	usize i = 0;
	// compare, storing final ECX value in i (if it's 0, it'll be equal still)
	asm("pushf\n"
		"cld\n"
		"repz cmpsb\n"
		"popf\n"
		: "=c"(i)
		: "S"(a), "D"(b), "c"(size));

	return ((s8*)a)[i] - ((s8*)b)[i];
}

usize strlen(cstr str)
{
	if (!str)
	{
		return 0;
	}

	cstr nul = str;
	asm("pushf\n"
		"cld\n"
		"repnz scasb\n"
		"popf\n"
		: "=D"(nul)
		: "a"(0), "D"(str), "c"(SIZE_MAX));

	return nul - str;
}

int strncmp(cstr a, cstr b, usize count)
{
	usize aLength = strlen(a);
	usize bLength = strlen(b);
	return memcmp(a, b, MIN(count, MIN(aLength, bLength)));
}

int strcmp(cstr a, cstr b)
{
	usize aLength = strlen(a);
	usize bLength = strlen(b);
	return memcmp(a, b, MIN(aLength, bLength));
}

cstr strchr(cstr str, char c)
{
	if (!str)
	{
		return NULL;
	}

	cstr pos = str;
	usize length = strlen(str);
	asm("pushf\n"
		"cld\n"
		"repnz scasb\n"
		"popf\n"
		: "=D"(pos)
		: "a"(c), "D"(str), "c"(length));

	return *pos == c ? pos : NULL;
}

cstr strrchr(cstr str, char c)
{
	if (!str)
	{
		return NULL;
	}

	cstr pos = str;
	usize length = strlen(str);
	asm("pushf\n"
		"std\n"
		"repnz scasb\n"
		"popf\n"
		: "=D"(pos)
		: "a"(c), "D"(str + length - 1), "c"(length));

	return *pos == c ? pos : NULL;
}

cstr strstr(cstr str, cstr seq)
{
	if (!str || !seq || *seq == '\0')
	{
		return NULL;
	}

	usize seqLength = strlen(seq);
	for (usize i = 0; i <= strlen(str) - seqLength; i++)
	{
		if (strncmp(&str[i], seq, seqLength) == 0)
		{
			return &str[i];
		}
	}

	return NULL;
}

extern dstr strncpy(dstr dest, cstr src, usize size)
{
	return memcpy(dest, src, size);
}

#define DEC 0
#define HEX 1
#define BIN 2

#define MAKE_ATOX(name, T)                                                                                                       \
	T name(cstr str)                                                                                                             \
	{                                                                                                                            \
		if (!str)                                                                                                                \
		{                                                                                                                        \
			return 0;                                                                                                            \
		}                                                                                                                        \
                                                                                                                                 \
		bool negative = str[0] == '-';                                                                                           \
		T sign = negative ? -1 : 1;                                                                                              \
		usize start = (str[0] == '-' || str[0] == '+') ? 1 : 0;                                                                  \
                                                                                                                                 \
		u32 mode = DEC;                                                                                                          \
		usize len = strlen(str);                                                                                                 \
		if (len > start + 2)                                                                                                     \
		{                                                                                                                        \
			switch (str[start + 1])                                                                                              \
			{                                                                                                                    \
			case 'b':                                                                                                            \
			case 'B':                                                                                                            \
				mode = HEX;                                                                                                      \
				start++;                                                                                                         \
				break;                                                                                                           \
			case 'x':                                                                                                            \
			case 'X':                                                                                                            \
				mode = BIN;                                                                                                      \
				start++;                                                                                                         \
				break;                                                                                                           \
			}                                                                                                                    \
		}                                                                                                                        \
                                                                                                                                 \
		T value = 0;                                                                                                             \
		switch (mode)                                                                                                            \
		{                                                                                                                        \
		default:                                                                                                                 \
		case DEC: {                                                                                                              \
			for (usize i = start; i < len; i++)                                                                                  \
			{                                                                                                                    \
				char c = str[i];                                                                                                 \
				if (!isdigit(c))                                                                                                 \
				{                                                                                                                \
					return 0;                                                                                                    \
				}                                                                                                                \
                                                                                                                                 \
				T digit = c - '0';                                                                                               \
				value = value * 10 + digit;                                                                                      \
			}                                                                                                                    \
			break;                                                                                                               \
		}                                                                                                                        \
		case HEX: {                                                                                                              \
			for (usize i = start; i < len; i++)                                                                                  \
			{                                                                                                                    \
				char c = toupper(str[i]);                                                                                        \
				if (!isxdigit(c))                                                                                                \
				{                                                                                                                \
					return 0;                                                                                                    \
				}                                                                                                                \
                                                                                                                                 \
				T digit = isdigit(c) ? c - '0' : isxdigit(c) ? c - 'A' + 10 : 0;                                                 \
				value |= digit << ((len - i - 1) << 3);                                                                          \
			}                                                                                                                    \
			break;                                                                                                               \
		}                                                                                                                        \
		case BIN: {                                                                                                              \
			for (usize i = start; i < len; i++)                                                                                  \
			{                                                                                                                    \
				if (str[i] != '0' && str[i] != '1')                                                                              \
				{                                                                                                                \
					return 0;                                                                                                    \
				}                                                                                                                \
                                                                                                                                 \
				value |= (str[i] - '0') << (len - i - 1);                                                                        \
			}                                                                                                                    \
			break;                                                                                                               \
		}                                                                                                                        \
		}                                                                                                                        \
                                                                                                                                 \
		return value * sign;                                                                                                     \
	}

MAKE_ATOX(atoll, s64)
MAKE_ATOX(atoi, s32)

f32 atof(cstr str)
{
	if (!str)
	{
		return 0.0f;
	}

	bool negative = str[0] == '-';
	f32 sign = negative ? -1.0f : 1.0f;
	usize start = (str[0] == '-' || str[0] == '+') ? 1 : 0;

	f32 fractional = 0;
	f32 integral = 0;

	// dubious infinity/nan check
	if (tolower(str[start]) == 'i')
	{
		return INFINITY * sign;
	}

	if (tolower(str[start]) == 'n')
	{
		return NAN; // NaN's sign isn't specified
	}

	cstr decimal = strchr(str, '.');
	usize len = strlen(str);
	usize integralLen = decimal ? decimal - str - start : len;

	if (integralLen > 0)
	{
		for (usize i = start; i < integralLen; i++)
		{
			char c = str[i];
			if (!isdigit(c))
			{
				return 0.0f;
			}

			f32 digit = c - '0';
			integral = integral * 10 + digit;
		}
	}

	if (decimal)
	{
		f32 scale = 0.1f;
		for (usize i = start + integralLen + 1; i < len; i++)
		{
			char c = str[i];
			if (!isdigit(c))
			{
				return 0.0f;
			}

			f32 digit = c - '0';
			fractional += digit * scale;
			scale *= 0.1f;
		}
	}

	return (integral + fractional) * sign;
}

struct
{
	bool initialized;
	u32 x[5];
	u32 counter;
} s_randState;

void srand(s32 seed)
{
	if (seed == 0)
	{
		((u64*)s_randState.x)[0] = GetTimer();
	}
	else
	{
		s_randState.x[0] = seed;
	}

	s_randState.initialized = true;
}

// https://en.wikipedia.org/wiki/Xorshift
s32 rand()
{
	if (!s_randState.initialized)
	{
		srand(0);
	}

	u32 t = s_randState.x[4];

	u32 s = s_randState.x[0];
	s_randState.x[4] = s_randState.x[3];
	s_randState.x[3] = s_randState.x[2];
	s_randState.x[2] = s_randState.x[1];
	s_randState.x[1] = s;

	t ^= t >> 2;
	t ^= t << 1;
	t ^= s ^ (s << 4);
	s_randState.x[0] = t;
	s_randState.counter += 362437;
	return t + s_randState.counter;
}
