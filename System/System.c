
#include "System.h"


void Delay_ms(uint32_t ms)
{
	uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 7200; j++)
			;
	}
}