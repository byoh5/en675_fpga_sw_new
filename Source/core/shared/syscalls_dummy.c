#include "dev.h"

#include <sys/time.h>
//#include <unistd.h>

#if 0
int _gettimeofday(struct timeval *tp, void *tzp)
{
	struct timezone *tz = tzp;
    if (tz) {
    	tz->tz_minuteswest = tz->tz_dsttime = 0;
    }
	uintptr_t t = rdcycle();
	tp->tv_sec = t / MCK_FREQ;
	tp->tv_usec = (t % MCK_FREQ) / (MCK_FREQ / 1000000);
	return 0;
}
#endif

unsigned long offset = 0;

int settimeofday(const struct timeval *a, const struct timezone *b)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	offset = a->tv_sec - tv.tv_sec;
	_printf("%s offset: %d\n", __func__, offset);
	return 0;
}

long gettime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}
