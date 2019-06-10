#ifndef PACKET_HEADER_H
#define PACKET_HEADER_H

#ifndef u_char
#define u_char unsigned char
#endif

#ifndef u_int8
#define u_int8 unsigned char
#endif

#ifndef u_int16
#define u_int16 unsigned short
#endif

#ifndef u_int32
#define u_int32 unsigned int
#endif

#ifndef u_int64
#define u_int64 unsigned long long
#endif

#ifndef u_short
#define u_short unsigned short
#endif

/* ��̫֡ͷ */
typedef struct tagEthHeader_t
{
    //Pcap���������֡ͷ
    u_int8 dest_hwaddr[6];   //Ŀ��MAC��ַ
    u_int8 source_hwaddr[6]; //ԴMAC��ַ
    u_short frame_type;      //֡����
}EthHeader_t;

//IP���ݱ�ͷ
typedef struct tagIPHeader_t
{
    //IP���ݱ�ͷ
    u_int8  Ver_HLen; //�汾+��ͷ����
    u_int8  TOS;      //��������
    u_int16 TotalLen;//�ܳ���
    u_int16 ID;      //��ʶ
    u_int16 Flag_Segment; //��־+Ƭƫ��
    u_int8  TTL;      //��������
    u_int8  Protocol; //Э������
    u_int16 Checksum;//ͷ��У���
    u_int8 SrcIP[4];   //ԴIP��ַ
    u_int8 DstIP[4];   //Ŀ��IP��ַ
} IPHeader_t;

//IPv6�����ײ�
#if 0
typedef struct tagIPv6Header_t
{
    u_char    version:4;      // 4-bit�汾��
    u_char  traffic_class:8;  // 8-bit�����ȼ�
    u_int32 label:20;       // 20-bit����ǩ
    u_short    payload_len;    // 16-bit �غɳ���
    u_char    next_header;    // 8-bit ��һ�ײ�
    u_char    hop_limit;        // 8-bit ��������
    struct
    {
        u_int64 prefix_subnetid;
        u_char interface_id[8];
    } src_ip;                // 128-bit Դ��ַ
    struct
    {
        u_int64 prefix_subnetid;
        u_char interface_id[8];
    } dst_ip;                // 128-bit Ŀ�ĵ�ַ

} IPv6Header_t;

typedef struct in6_addr {
  union {
    u_char  Byte[16];
    u_short Word[8];
  } u;
} IN6_ADDR, *PIN6_ADDR, FAR *LPIN6_ADDR;

#endif


typedef struct tagIPv6Header_t
{
    union
    {
        struct ip6_hdrctl
        {
            u_int32_t ip6_unl_flow;/* 4λ�İ汾��8λ�Ĵ�������࣬20λ������ʶ�� */
            u_int16_t ip6_unl_plen;/* ��ͷ���� */
            u_int8_t ip6_unl_nxt;  /* ��һ����ͷ */
            u_int8_t ip6_unl_hlim; /* ������� */
        }ip6_unl ;

        u_int8_t ip6_un2_vfc;/* 4λ�İ汾�ţ����Ϊ4λ�Ĵ������ */
    }ip6_ctlun ;

#define ip6_vfc              ip6_ctlun.ip6_un2_vfc
#define ip6_flow             ip6_ctlun.ip6_unl.ip6_unl_flow
#define ip6_plen             ip6_ctlun.ip6_unl.ip6_unl_plen
#define ip6_nxt              ip6_ctlun.ip6_unl.ip6_unl_nxt
#define ip6_hlim             ip6_ctlun.ip6_unl.ip6_unl_hlim
#define ip6_hops             ip6_ctlun.ip6_unl.ip6_unl_hops

    struct in6_addr ip6_src;/* ���Ͷ˵�ַ */
    struct in6_addr ip6_dst;/* ���ն˵�ַ */
}IPv6Header_t;

//TCP���ݱ�ͷ
typedef struct tagTCPHeader_t
{
    //TCP���ݱ�ͷ
    u_int16 SrcPort; //Դ�˿�
    u_int16 DstPort; //Ŀ�Ķ˿�
    u_int32 SeqNO;   //���
    u_int32 AckNO;   //ȷ�Ϻ�
} TCPHeader_t;

#endif