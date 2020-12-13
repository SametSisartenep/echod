#define nil NULL

typedef long long vlong;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long uvlong;
typedef uint32_t u32int;
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

typedef struct UDPEchoPlus UDPEchoPlus;


/*
 * TR-143 - Enabling Network Throughput Performance Tests and Statistical Monitoring
 * Issue 1 Amendment 1 Corrigendum 1, § A.1.4
 */
struct UDPEchoPlus
{
	u32int gensn;		/* TestGenSN */
	u32int ressn;		/* TestRespSN */
	u32int rxtime;		/* TestRespRecvTimeStamp */
	u32int txtime;		/* TestRespReplyTimeStamp */
	u32int ntxfails;	/* TestRespReplyFailureCount */
	u32int iterno;		/* TestIterationNumber */
};
