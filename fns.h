/* util */
void sysfatal(char*, ...);
void *emalloc(ulong);
sockaddr_in *mkinetsa(char*, int);
int listentcp(int);
int bindudp(int);
int acceptcall(int, char*, int);
u32int get32(uchar*);
void put32(uchar*, u32int);

/* echoplus */
int getechoplus(UDPEchoPlus*, uchar*);
void putechoplus(UDPEchoPlus*, uchar*);
