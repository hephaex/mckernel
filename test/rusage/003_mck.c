#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "util.h"

#define DEBUG

int sz_mem[] = {
	4 * (1ULL<<10),
	2 * (1ULL<<20),
	1 * (1ULL<<30),
	134217728};

#define SZ_INDEX 0

int main(int argc, char **argv)
{
	void *mem;
	int ret = 0;
	int fd;
	char fn[256] = "/dev/shm/Intel_MPI";

	fd = open(fn, O_CREAT | O_TRUNC | O_RDWR, 0600);
	CHKANDJUMP(fd == -1, 255, "shm_open failed,str=%s\n", strerror(errno));

	ret = ftruncate(fd, sz_mem[SZ_INDEX]);
	CHKANDJUMP(ret != 0, 255, "ftruncate failed\n");

	mem = mmap(0, sz_mem[SZ_INDEX], PROT_READ | PROT_WRITE, MAP_SHARED,
		   fd, 0);
	CHKANDJUMP(mem == MAP_FAILED, 255, "mmap failed\n");

	//unsigned long val = *((unsigned long *)mem);
	//memset(mem, 0, sz_mem[SZ_INDEX]);

		*((unsigned long *)mem) = 0x123456789abcdef0;
	//  ret = msync(mem, sz_mem[SZ_INDEX], MS_SYNC);
	//	*((unsigned long *)mem) = 0x123456789abcdef0;

	munmap(mem, sz_mem[SZ_INDEX]);
	ret = close(fd);
	CHKANDJUMP(ret != 0, 255, "close failed\n");
	ret = unlink(fn);
	CHKANDJUMP(ret != 0, 255, "shm_unlink failed\n");

 fn_exit:
	return ret;
 fn_fail:
	goto fn_exit;
}
