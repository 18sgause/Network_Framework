#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable: 4996)


#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "PacketStructures.h"

using namespace std;


void sendRawPacket(struct ipheader* ip)
{

    struct sockaddr_in client;
    char enable = 1;
    const char* buffer = "Wassup";

    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

    client.sin_family = AF_INET;
    client.sin_addr = ip->iph_destip;


    sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, sizeof(client));

    closesocket(sock);
}

void sendUdp(const char* destination)
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

    ip->iph_protocol = IPPROTO_UDP;
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_ttl = 20;
    ip->iph_sourceip.s_addr = rand();
    ip->iph_destip.s_addr = inet_addr(destination);
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader));

    sendRawPacket(ip);
}


void sendTCP(const char* destination)
{
    char buffer[1500];
    struct ipheader* ip = (struct ipheader*)buffer;
    struct tcpheader* tcp = (struct tcpheader*)(buffer + sizeof(struct ipheader));

    srand(time(0));

    memset(buffer, 0, 1500);
    tcp->tcp_sport = rand();
    tcp->tcp_dport = htons(80);
    tcp->tcp_seq = rand();
    tcp->tc_offx2 = 0x50;
    tcp->tcp_win = htons(20000);
    tcp->tcp_seq = 0;
    do
    {
        ip->iph_protocol = IPPROTO_UDP;
        ip->iph_ver = 4;
        ip->iph_ihl = 5;
        ip->iph_ttl = 20;
        ip->iph_sourceip.s_addr = rand();
        ip->iph_destip.s_addr = inet_addr(destination);
        ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct tcpheader));
    } while (false);

;    sendRawPacket(ip);

}

void sendICMP(const char* destination)
{
    char buffer[1500];
    memset(buffer, 0, 1500);
    struct icmpheader* icmp = (struct icmpheader*)(buffer + sizeof(struct ipheader));

    icmp->icmp_type = 8;

    struct ipheader* ip = (struct ipheader*)buffer;
    ip->iph_protocol = IPPROTO_UDP;
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_ttl = 20;
    ip->iph_sourceip.s_addr = rand();
    ip->iph_destip.s_addr = inet_addr(destination);
    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct tcpheader));

    sendRawPacket(ip);
}

void snoof(struct ipheader* ip)
{
    char buffer[1500];
    int ip_header_len = ip->iph_ihl * 4;
    struct udpheader * udp = (struct udpheader*) (unsigned char*)ip + ip_header_len;
    memset((char*)buffer, 0, 1500);
    memcpy((char*)buffer, ip , ntohs(ip->iph_len));
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

    sendRawPacket(ip);
}

int main(int argc, char** argv)
{

    if ('t' == (char)argv[0])
    {
        sendTCP(argv[1]);
    }
}