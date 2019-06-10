#ifndef PACKET_HEADER_H
#define PACKET_HEADER_H
#define BIT(nr) (1 << (nr))
#include <stdbool.h>

#define MAC_LEN			6
#define MAX_CONF_VALUE_STRLEN	200
#define MAX_CONF_NAME_STRLEN	32

#define WLAN_MODE_AP		BIT(0)
#define WLAN_MODE_IBSS		BIT(1)
#define WLAN_MODE_STA		BIT(2)
#define WLAN_MODE_PROBE		BIT(3)
#define WLAN_MODE_4ADDR		BIT(4)
#define WLAN_MODE_UNKNOWN	BIT(5)

#define WLAN_MODE_ALL		(WLAN_MODE_AP | WLAN_MODE_IBSS | WLAN_MODE_STA | WLAN_MODE_PROBE | WLAN_MODE_4ADDR | WLAN_MODE_UNKNOWN)


#define WLAN_FRAME_FC_VERSION_MASK	0x0003
#define WLAN_FRAME_FC_TYPE_MASK		0x000C
#define WLAN_FRAME_FC_STYPE_MASK	0x00F0
#define WLAN_FRAME_FC_STYPE_QOS		0x0080
#define WLAN_FRAME_FC_TO_DS		0x0100
#define WLAN_FRAME_FC_FROM_DS		0x0200
#define WLAN_FRAME_FC_MORE_FRAG		0x0400
#define WLAN_FRAME_FC_RETRY		0x0800
#define WLAN_FRAME_FC_POWER_MGMT	0x1000
#define WLAN_FRAME_FC_MORE_DATA		0x2000
#define WLAN_FRAME_FC_PROTECTED		0x4000
#define WLAN_FRAME_FC_ORDER		0x8000

#define WLAN_FRAME_FC_MASK		(WLAN_FRAME_FC_TYPE_MASK | WLAN_FRAME_FC_STYPE_MASK)

#define WLAN_FRAME_TYPE(_fc)		((_fc & WLAN_FRAME_FC_TYPE_MASK) >> 2)
#define WLAN_FRAME_STYPE(_fc)		((_fc & WLAN_FRAME_FC_STYPE_MASK) >> 4)
#define WLAN_FRAME_FC(_type, _stype)	((((_type) << 2) | ((_stype) << 4)) & WLAN_FRAME_FC_MASK)
#define WLAN_FRAME_TYPE_MGMT		0x0
#define WLAN_FRAME_TYPE_CTRL		0x1
#define WLAN_FRAME_TYPE_DATA		0x2

#define WLAN_NUM_TYPES			3
#define WLAN_NUM_STYPES			16

#define WLAN_FRAME_IS_MGMT(_fc)		(WLAN_FRAME_TYPE(_fc) == WLAN_FRAME_TYPE_MGMT)
#define WLAN_FRAME_IS_CTRL(_fc)		(WLAN_FRAME_TYPE(_fc) == WLAN_FRAME_TYPE_CTRL)
#define WLAN_FRAME_IS_DATA(_fc)		(WLAN_FRAME_TYPE(_fc) == WLAN_FRAME_TYPE_DATA)
#define WLAN_FRAME_IS_QOS(_fc)		(((_fc) & WLAN_FRAME_FC_STYPE_MASK) == WLAN_FRAME_FC_STYPE_QOS)
#ifndef u_char
#define u_char unsigned char
#endif
/*** management ***/
#define WLAN_FRAME_ASSOC_REQ		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x0)
#define WLAN_FRAME_ASSOC_RESP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x1)
#define WLAN_FRAME_REASSOC_REQ		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x2)
#define WLAN_FRAME_REASSOC_RESP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x3)
#define WLAN_FRAME_PROBE_REQ		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x4)
#define WLAN_FRAME_PROBE_RESP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x5)
#define WLAN_FRAME_TIMING		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x6)
/* (reserved)								    0x7 */
#define WLAN_FRAME_BEACON		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x8)
#define WLAN_FRAME_ATIM			WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0x9)
#define WLAN_FRAME_DISASSOC		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0xa)
#define WLAN_FRAME_AUTH			WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0xb)
#define WLAN_FRAME_DEAUTH		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0xc)
#define WLAN_FRAME_ACTION		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0xd)
#define WLAN_FRAME_ACTION_NOACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_MGMT, 0xe)
/* (reserved)								    0xf */
/*** control ***/
/* (reserved)								    0-3 */
#define WLAN_FRAME_BEAM_REP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0x4)
#define WLAN_FRAME_VHT_NDP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0x5)
/* (reserved)								    0x6 */
#define WLAN_FRAME_CTRL_WRAP		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0x7)
#define WLAN_FRAME_BLKACK_REQ		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0x8)
#define WLAN_FRAME_BLKACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0x9)
#define WLAN_FRAME_PSPOLL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xa)
#define WLAN_FRAME_RTS			WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xb)
#define WLAN_FRAME_CTS			WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xc)
#define WLAN_FRAME_ACK			WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xd)
#define WLAN_FRAME_CF_END		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xe)
#define WLAN_FRAME_CF_END_ACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_CTRL, 0xf)

/*** data ***/
#define WLAN_FRAME_DATA			WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x0)
#define WLAN_FRAME_DATA_CF_ACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x1)
#define WLAN_FRAME_DATA_CF_POLL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x2)
#define WLAN_FRAME_DATA_CF_ACKPOLL	WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x3)
#define WLAN_FRAME_NULL			WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x4)
#define WLAN_FRAME_CF_ACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x5)
#define WLAN_FRAME_CF_POLL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x6)
#define WLAN_FRAME_CF_ACKPOLL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x7)
#define WLAN_FRAME_QDATA		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x8)
#define WLAN_FRAME_QDATA_CF_ACK		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0x9)
#define WLAN_FRAME_QDATA_CF_POLL	WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0xa)
#define WLAN_FRAME_QDATA_CF_ACKPOLL	WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0xb)
#define WLAN_FRAME_QOS_NULL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0xc)
/* (reserved)								    0xd */
#define WLAN_FRAME_QOS_CF_POLL		WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0xe)
#define WLAN_FRAME_QOS_CF_ACKPOLL	WLAN_FRAME_FC(WLAN_FRAME_TYPE_DATA, 0xf)

#define WLAN_FRAME_QOS_TID_MASK		0x7
#define WLAN_FRAME_QOS_AMSDU_PRESENT	0x80

#define WLAN_FRAME_SEQ_MASK		0xFFF0
#define WLAN_FRAME_SEQ_FRAG_MASK	0x000F

#define WLAN_FRAME_HT_VHT		0x1
#define WLAN_FRAME_VHT_BW_MASK		0x30000
#define WLAN_FRAME_VHT_BW_20MHZ		0x0
#define WLAN_FRAME_VHT_BW_40MHZ		0x10000
#define WLAN_FRAME_VHT_BW_80MHZ		0x20000
#define WLAN_FRAME_VHT_BW_160MHZ	0x30000

/*** individual frame formats ***/

/* beacon + probe response */
struct wlan_frame_beacon {
	uint64_t	tsf;
	uint16_t	bintval;
	uint16_t	capab;
	unsigned char	ie[0];
} __attribute__ ((packed));


/*** capabilities ***/
#define WLAN_CAPAB_ESS		0x0001
#define WLAN_CAPAB_IBSS		0x0002
#define WLAN_CAPAB_CF_POLL	0x0004
#define WLAN_CAPAB_CF_POLL_REQ	0x0008
#define WLAN_CAPAB_PRIVACY	0x0010
#define WLAN_CAPAB_SHORT_PRE	0x0020
#define WLAN_CAPAB_PBCC		0x0040
#define WLAN_CAPAB_CHAN_AGILIY	0x0080
#define WLAN_CAPAB_SPECT_MGMT	0x0100
#define WLAN_CAPAB_QOS		0x0200
#define WLAN_CAPAB_SHORT_SLOT	0x0400
#define WLAN_CAPAB_APSD		0x0800
#define WLAN_CAPAB_RADIO_MEAS	0x1000
#define WLAN_CAPAB_OFDM		0x2000
#define WLAN_CAPAB_DEL_BLKACK	0x4000
#define WLAN_CAPAB_IMM_BLKACK	0x8000

/*** information elements ***/
struct information_element {
	uint8_t		id;
	uint8_t		len;
	unsigned char	var[0];
};

/* only the information element IDs we are interested in */
#define WLAN_IE_ID_SSID		0
#define WLAN_IE_ID_DSSS_PARAM	3
#define WLAN_IE_ID_HT_CAPAB	45
#define WLAN_IE_ID_RSN		48
#define WLAN_IE_ID_HT_OPER	61
#define WLAN_IE_ID_VHT_CAPAB	191
#define WLAN_IE_ID_VHT_OPER	192
#define WLAN_IE_ID_VHT_OMN	199
#define WLAN_IE_ID_VENDOR	221

/* HT capability info */
// present in Beacon, Assoc Req/Resp, Reassoc Req/Resp, Probe Req/Resp, Mesh Peering Open/Close
#define WLAN_IE_HT_CAPAB_INFO_CHAN_WIDTH_40	0x0002

#define WLAN_IE_HT_OPER_INFO_CHAN_OFFSET	0x0003

/* VHT capability info */
#define WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH	0x0000000c
#define WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_80	0 /* 80MHz only */
#define WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_160	1 /* 160MHz */
#define WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_BOTH	2 /* 160MHz and 80+80 MHz */

#define WLAN_MAX_SSID_LEN	34


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
enum chan_width {
	CHAN_WIDTH_UNSPEC,
	CHAN_WIDTH_20_NOHT,
	CHAN_WIDTH_20,
	CHAN_WIDTH_40,
	CHAN_WIDTH_80,
	CHAN_WIDTH_160,
	CHAN_WIDTH_8080,
};

struct packet_info {
	/* general */
	unsigned int		pkt_types;	/* bitmask of packet types */

	/* wlan phy (from radiotap) */
	int			phy_signal;	/* signal strength (usually dBm) */
	unsigned int		phy_rate;	/* physical rate * 10 (=in 100kbps) */
	unsigned char		phy_rate_idx;	/* MCS index */
	unsigned char		phy_rate_flags;	/* MCS flags */
	unsigned int		phy_freq;	/* frequency from driver */
	unsigned int		phy_flags;	/* A, B, G, shortpre */

	/* wlan mac */
	unsigned int		wlan_len;	/* packet length */
	uint16_t		wlan_type;	/* frame control field */
	unsigned char		wlan_src[MAC_LEN]; /* transmitter (TA) */
	unsigned char		wlan_dst[MAC_LEN]; /* receiver (RA) */
	unsigned char		wlan_bssid[MAC_LEN];
	char			wlan_essid[WLAN_MAX_SSID_LEN];
	uint64_t		wlan_tsf;	/* timestamp from beacon */
	unsigned int		wlan_bintval;	/* beacon interval */
	unsigned int		wlan_mode;	/* AP, STA or IBSS */
	unsigned char		wlan_channel;	/* channel from beacon, probe */
	enum chan_width		wlan_chan_width;
	unsigned char		wlan_tx_streams;
	unsigned char		wlan_rx_streams;
	unsigned char		wlan_qos_class;	/* for QDATA frames */
	unsigned int		wlan_nav;	/* frame NAV duration */
	unsigned int		wlan_seqno;	/* sequence number */

	/* flags */
	unsigned int		wlan_wep:1,	/* WEP on/off */
				wlan_retry:1,
				wlan_wpa:1,
				wlan_rsn:1,
				wlan_ht40plus:1;

	/* batman-adv */
	unsigned char		bat_version;
	unsigned char		bat_packet_type;
	unsigned char		bat_gw:1;

	/* IP */
	unsigned int		ip_src;
	unsigned int		ip_dst;
	unsigned int		tcpudp_port;
	unsigned int		olsr_type;
	unsigned int		olsr_neigh;
	unsigned int		olsr_tc;

	/* calculated from other values */
	unsigned int		pkt_duration;	/* packet "airtime" */
	int			pkt_chan_idx;	/* received while on channel */
	int			wlan_retries;	/* retry count for this frame */
};

/* 以太帧头 */
typedef struct tagEthHeader_t
{
    //Pcap捕获的数据帧头
    u_int8 dest_hwaddr[6];   //目的MAC地址
    u_int8 source_hwaddr[6]; //源MAC地址
    u_short frame_type;      //帧类型
}EthHeader_t;

//IP数据报头
typedef struct tagIPHeader_t
{
    //IP数据报头
    u_int8  Ver_HLen; //版本+报头长度
    u_int8  TOS;      //服务类型
    u_int16 TotalLen;//总长度
    u_int16 ID;      //标识
    u_int16 Flag_Segment; //标志+片偏移
    u_int8  TTL;      //生存周期
    u_int8  Protocol; //协议类型
    u_int16 Checksum;//头部校验和
    u_int8 SrcIP[4];   //源IP地址
    u_int8 DstIP[4];   //目的IP地址
} IPHeader_t;

//IPv6基本首部
#if 0
typedef struct tagIPv6Header_t
{
    u_char    version:4;      // 4-bit版本号
    u_char  traffic_class:8;  // 8-bit流量等级
    u_int32 label:20;       // 20-bit流标签
    u_short    payload_len;    // 16-bit 载荷长度
    u_char    next_header;    // 8-bit 下一首部
    u_char    hop_limit;        // 8-bit 跳数限制
    struct
    {
        u_int64 prefix_subnetid;
        u_char interface_id[8];
    } src_ip;                // 128-bit 源地址
    struct
    {
        u_int64 prefix_subnetid;
        u_char interface_id[8];
    } dst_ip;                // 128-bit 目的地址

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
            u_int32_t ip6_unl_flow;/* 4位的版本，8位的传输与分类，20位的流标识符 */
            u_int16_t ip6_unl_plen;/* 报头长度 */
            u_int8_t ip6_unl_nxt;  /* 下一个报头 */
            u_int8_t ip6_unl_hlim; /* 跨度限制 */
        }ip6_unl ;

        u_int8_t ip6_un2_vfc;/* 4位的版本号，跨度为4位的传输分类 */
    }ip6_ctlun ;

#define ip6_vfc              ip6_ctlun.ip6_un2_vfc
#define ip6_flow             ip6_ctlun.ip6_unl.ip6_unl_flow
#define ip6_plen             ip6_ctlun.ip6_unl.ip6_unl_plen
#define ip6_nxt              ip6_ctlun.ip6_unl.ip6_unl_nxt
#define ip6_hlim             ip6_ctlun.ip6_unl.ip6_unl_hlim
#define ip6_hops             ip6_ctlun.ip6_unl.ip6_unl_hops

    struct in6_addr ip6_src;/* 发送端地址 */
    struct in6_addr ip6_dst;/* 接收端地址 */
}IPv6Header_t;

//TCP数据报头
typedef struct tagTCPHeader_t
{
    //TCP数据报头
    u_int16 SrcPort; //源端口
    u_int16 DstPort; //目的端口
    u_int32 SeqNO;   //序号
    u_int32 AckNO;   //确认号
} TCPHeader_t;

#endif
