#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <utf.h>
#include <fmt.h>
#include "dat.h"
#include "fns.h"
#include "args.h"

enum {
	LPORT = 7
};

int lfd, ncpu;
pthread_t *threads;
pthread_mutex_t attendlock;

int debug;
char *argv0;

/*
 * echo service as of rfc862
 */
//void
//srvecho(int fd)
//{
//	char buf[8192];
//	int n;
//
//	while((n = read(fd, buf, sizeof buf)) > 0)
//		if(write(fd, buf, n) != n)
//			return;
//}
//
//void*
//tmain(void *a)
//{
//	char caddr[128];
//	int cfd;
//
//	for(;;){
//		pthread_mutex_lock(&attendlock);
//		if((cfd = acceptcall(lfd, caddr, sizeof caddr)) < 0)
//			sysfatal("acceptcall: %r");
//		pthread_mutex_unlock(&attendlock);
//
//		if(debug)
//			fprint(2, "thr#%lu accepted call from %s\n", pthread_self(), caddr);
//
//		srvecho(cfd);
//		close(cfd);
//
//		if(debug)
//			fprint(2, "thr#%lu ended call with %s\n", pthread_self(), caddr);
//	}
//}

/*
 * (udp)echo service as of rfc862
 */
void*
udpechosrv(void *a)
{
	sockaddr_in csa;
	uchar buf[4096];
	char caddr[128], *cs;
	int n, port;
	uint csalen;
	UDPEchoPlus ephdr;

	struct timespec ts;
	uvlong us;

	memset(&csa, 0, sizeof(sockaddr_in));
	csalen = sizeof(sockaddr_in);

	for(;;){
		pthread_mutex_lock(&attendlock);
		if((n = recvfrom(lfd, buf, sizeof buf, 0, (sockaddr*)&csa, &csalen)) < 0){
			pthread_mutex_unlock(&attendlock);
			continue;
		}
		pthread_mutex_unlock(&attendlock);

		clock_gettime(CLOCK_REALTIME, &ts);
		us = ts.tv_sec*1e6;
		us += ts.tv_nsec/1e3;

		cs = inet_ntoa(csa.sin_addr);
		port = ntohs(csa.sin_port);
		snprint(caddr, sizeof caddr, "udp!%s!%d", cs, port);

		if(debug)
			fprint(2, "thr#%lu received %d byte datagram from %s\n", pthread_self(), n, caddr);

		/* check for echo+ data */
		if(n >= 24){
			if(getechoplus(&ephdr, buf) < 0)
				goto EchoBack;

			ephdr.ressn = ephdr.gensn;
			ephdr.rxtime = us;
			ephdr.ntxfails = 0; /* being over-optimistic */

			clock_gettime(CLOCK_REALTIME, &ts);
			us = ts.tv_sec*1e6;
			us += ts.tv_nsec/1e3;

			ephdr.txtime = us;

			putechoplus(&ephdr, buf);
		}
EchoBack:
		if(sendto(lfd, buf, n, 0, (sockaddr*)&csa, csalen) < 0)
			continue;

		if(debug)
			fprint(2, "thr#%lu sent %d byte datagram to %s\n", pthread_self(), n, caddr);
	}
}

static void
usage(void)
{
	fprint(2, "usage: %s [-d]\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	int i;

	ARGBEGIN{
	case 'd': debug++; break;
	default: usage();
	}ARGEND;
	if(argc != 0)
		usage();

	if((lfd = bindudp(LPORT)) < 0)
		sysfatal("bindudp: %r");

	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
	if(ncpu < 1)
		ncpu = 1;

	threads = emalloc(sizeof(pthread_t)*ncpu);
	pthread_mutex_init(&attendlock, nil);

	for(i = 0; i < ncpu; i++){
		pthread_create(threads+i, nil, udpechosrv, nil);
		if(debug)
			fprint(2, "created thr#%lu\n", *(threads+i));
	}

	pause();

	free(threads);
	pthread_mutex_destroy(&attendlock);
	exit(0);
}
