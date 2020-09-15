struct ipheader
{
    unsigned char iph_ihl : 4;
    unsigned char iph_ver : 4;
    unsigned char iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned short int iph_flag : 3;
    unsigned short int iph_offset : 13;
    unsigned char iph_ttl;
    unsigned char iph_protocol;
    unsigned short intiph_chksum;
    struct in_addr iph_sourceip;
    struct in_addr iph_destip;


};

struct udpheader
{
    unsigned short udp_sport;
    unsigned short udp_dport;
    unsigned short udp_ulength;
    unsigned short udp_check;

};

struct tcpheader
{
    unsigned short tcp_sport;
    unsigned short tcp_dport;
    unsigned short tcp_seq;
    unsigned short tc_offx2;
    unsigned short tcp_flags;
    unsigned short tcp_win;
    unsigned short tcp_check;

};

struct icmpheader
{
    unsigned char icmp_type;
    unsigned char icmp_code;
    unsigned short int icmp_check;
    unsigned short int icmp_id;
    unsigned short int icmp_seq;
};