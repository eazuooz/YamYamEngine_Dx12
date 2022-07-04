#pragma once

#define arraysize(a) (sizeof(a) / sizeof(a[0]))

template <typename T>
static inline void			ZeroInitialize(T& value)
{
	memset(&value, 0, sizeof(T));
}