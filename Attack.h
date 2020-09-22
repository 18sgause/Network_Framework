#pragma once
#include "CraftPackets.h"
class Attack
{
public:
	void sendRawPacket(struct ipheader*);
	void SYNAttack(ipheader*);
};