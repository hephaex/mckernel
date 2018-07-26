#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

char *
gettime(char *buf, struct timeval *tv)
{
	struct tm *tm;

	gettimeofday(tv, NULL);
	tm = localtime(&tv->tv_sec);
	sprintf(buf, "%02d:%02d:%02d.%06d", tm->tm_hour, tm->tm_min, tm->tm_sec, tv->tv_usec);
	return buf;
}

void
tv_sub(struct timeval *tv1, const struct timeval *tv2)
{
	tv1->tv_sec -= tv2->tv_sec;
	tv1->tv_usec -= tv2->tv_usec;
	if (tv1->tv_usec < 0) {
		tv1->tv_sec--;
		tv1->tv_usec += 1000000;
	}
}

struct timeval tv1;
struct timeval tv2;

void
sig(int s)
{
	char buf[16];

	fprintf(stderr, "%s signal hanlder is called\n", gettime(buf, &tv2));
}

int
main(int argc, char **argv)
{
	struct sigaction act;
	int fds[2];
	char c;
	int rc;
	char buf[16];

	memset(&act, '\0', sizeof act);
	act.sa_handler = sig;
	sigaction(SIGALRM, &act, NULL);
	fprintf(stderr, "%s test start, kill after 3 seconds\n", gettime(buf, &tv1));
	alarm(3);
	pipe(fds);
	rc = read(fds[0], &c, 1);
	if (rc != -1) {
		fprintf(stderr, "CT3001 NG BAD read rc=%d\n", rc);
		exit(1);
	}
	if (errno != EINTR) {
		fprintf(stderr, "CT3001 NG BAD error errno=%d\n", errno);
		exit(1);
	}
	tv_sub(&tv2, &tv1);
	if (tv2.tv_sec != 3)
		fprintf(stderr, "CT3001 NG signal delayed (%d.%06d)\n", tv2.tv_sec, tv2.tv_usec);
	else
		fprintf(stderr, "CT3001 OK\n");
	exit(0);
}
