/* What BStone's POSIX files call and libnix does not have.
 * The stove's <dlfcn.h> and <time.h> declare these without extern "C", so
 * they are defined here as C++ functions, as the callers see them.
 * Copyright (c) 2026 AmiCradle Team. GPL 2 or later, like BStone. */

#include <dlfcn.h>
#include <sys/file.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/* No shared libraries: BStone looks for OpenAL, OpenGL and Vulkan this
 * way, finds none and uses its software renderer and its SDL mixer. */
void* dlopen(const char*, int)
{
	return nullptr;
}

void* dlsym(void*, const char*)
{
	return nullptr;
}

int dlclose(void*)
{
	return 0;
}

/* One program owns its files here; the locks always succeed. */
extern "C" int flock(int, int)
{
	return 0;
}

/* "Open a file or a URL" (the menus' web links): nothing to start it with. */
extern "C" int execle(const char*, const char*, ...)
{
	errno = ENOSYS;
	return -1;
}

// bstone_time.cpp includes <time.h> from C++ and so calls a C++
// clock_gettime(unsigned long, timespec*), while this file's <time.h>
// gives the C one; the C++ name is given here by hand.
extern "C" int amiga_clock_gettime(unsigned long, struct timespec* tp)
	__asm__("__Z13clock_gettimemP8timespec");

extern "C" int amiga_clock_gettime(unsigned long, struct timespec* tp)
{
	struct timeval tv;

	if (gettimeofday(&tv, nullptr) != 0)
	{
		return -1;
	}

	tp->tv_sec = tv.tv_sec;
	tp->tv_nsec = tv.tv_usec * 1000L;
	return 0;
}
