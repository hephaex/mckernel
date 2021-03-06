/* bitops-set_bit.h COPYRIGHT FUJITSU LIMITED 2015-2016 */
#ifndef INCLUDE_BITOPS_SET_BIT_H
#define INCLUDE_BITOPS_SET_BIT_H

static inline void set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	unsigned long *p = ((unsigned long *)addr) + (nr / BITS_PER_LONG);

	*p  |= mask;
}

#endif

