#include "CraftPackets.h"

void craftIcpHeader(struct ipheader* ip)
{
    ip->iph_protocol = IPPROTO_UDP;
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_ttl = 20;
    ip->iph_sourceip.s_addr = rand()
}

ipheader* craftICMP(const char* destination)
{
    char buffer[1500];
    memset(buffer, 0, 1500);
    struct icmpheader* icmp = (struct icmpheader*)(buffer + sizeof(struct ipheader));

    icmp->icmp_type = 8;

    struct ipheader* ip = (struct ipheader*)buffer;

    craftIcpHeader(ip);

    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct tcpheader));

    return ip;
}

ipheader* craftTCP(const char* destination)
{
    char buffer[1500];
    struct ipheader* ip = (struct ipheader*)buffer;
    struct tcpheader* tcp = (struct tcpheader*)(buffer + sizeof(struct ipheader));

    memset(buffer, 0, 1500);
    tcp->tcp_sport = rand();
    tcp->tcp_dport = htons(80);
    tcp->tcp_seq = rand();
    tcp->tc_offx2 = 0x50;
    tcp->tcp_win = htons(20000);
    tcp->tcp_seq = 0;

    craftIcpHeader(ip);
    ip->iph_destip.s_addr = inet_addr(destination);
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct tcpheader));
    return ip;
}


ipheader* craftUdp(const char* destination)
{
    char buffer[1500];
    memset(buffer, 0, 1500);
    struct ipheader* ip = (struct ipheader*)buffer;
    struct udpheader* udp = (struct udpheader*)(buffer + sizeof(struct ipheader));

    char* data = buffer + sizeof(struct ipheader) + sizeof(struct udpheader);
    const char* msg = "wassgood\n";
    int data_len = strlen(msg);
    strncpy(data, msg, data_len);

    udp->udp_sport = htons(12345);
    udp->udp_dport = htons(9090);
    udp->udp_ulength = htons(sizeof(struct udpheader) + data_len);
    udp->udp_check = 0;

    craftIcpHeader(ip);

    ip->iph_destip.s_addr = inet_addr(destination);
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader));

    return ip;
}

ipheader* snoofTCP(struct ipheader* ip)
{
    char buffer[1500];
    int ip_header_len = ip->iph_ihl * 4;
    struct tcpheader* tcp = (struct tcpheader*)(unsigned char*)ip + ip_header_len;
    memset((char*)buffer, 0, 1500);
    memcpy((char*)buffer, ip, ntohs(ip->iph_len));
    struct ipheader* newip = (struct ipheader*)buffer;
    struct tcpheader* newtcp = (struct tcpheader*)(buffer + ip_header_len);
    char* data = (char*)newtcp + sizeof(struct tcpheader);

    newtcp->tcp_sport = tcp->tcp_dport;
    newtcp->tcp_dport = tcp->tcp_sport;

    newip->iph_sourceip = ip->iph_destip;
    newip->iph_destip = ip->iph_sourceip;
    newip->iph_ttl = 50;
    newip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader));

    return newip;
}

ipheader* snoofUDP(struct ipheader* ip)
{
    char buffer[1500];
    int ip_header_len = ip->iph_ihl * 4;
    struct udpheader* udp = (struct udpheader*)(unsigned char*)ip + ip_header_len;
    memset((char*)buffer, 0, 1500);
    memcpy((char*)buffer, ip, ntohs(ip->iph_len));
    struct ipheader* newip = (struct ipheader*)buffer;
    struct udpheader* newudp = (struct udpheader*)(buffer + ip_header_len);
    char* data = (char*)newudp + sizeof(struct udpheader);

    const char* msg = "wasupwasupwasup";
    int data_len = strlen(msg);
    strncpy(data, msg, data_len);

    newudp->udp_sport = udp->udp_dport;
    newudp->udp_dport = udp->udp_sport;
    newudp->udp_ulength = htons(sizeof(struct udpheader) + data_len);
    newudp->udp_check = 0;

    newip->iph_sourceip = ip->iph_destip;
    newip->iph_destip = ip->iph_sourceip;
    newip->iph_ttl = 50;
    newip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader) + data_len);

    return newip;
}