#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utf.h>
#include <fmt.h>
#include "dat.h"
#include "fns.h"

int
getechoplus(UDPEchoPlus *ephdr, uchar *data)
{
	ephdr->gensn = get32(data);
	data += 4;
	ephdr->ressn = get32(data);
	data += 4;
	ephdr->rxtime = get32(data);
	data += 4;
	ephdr->txtime = get32(data);
	data += 4;
	ephdr->ntxfails = get32(data);
	data += 4;
	ephdr->iterno = get32(data);

	return 0;
}

void
putechoplus(UDPEchoPlus *ephdr, uchar *data)
{
	put32(data, ephdr->gensn);
	data += 4;
	put32(data, ephdr->ressn);
	data += 4;
	put32(data, ephdr->rxtime);
	data += 4;
	put32(data, ephdr->txtime);
	data += 4;
	put32(data, ephdr->ntxfails);
	data += 4;
	put32(data, ephdr->iterno);
}
