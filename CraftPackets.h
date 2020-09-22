#include "PacketStructures.h"

class CraftPackets
{
public:
	void craftIcpHeader(struct ipheader*);
	ipheader* craftUdp(const char*);
	ipheader* craftTCP(const char*);
	ipheader* craftICMP(const char*);

	ipheader* snoofTCP(struct ipheader*);
	ipheader* snoofUDP(struct ipheader*);

};
