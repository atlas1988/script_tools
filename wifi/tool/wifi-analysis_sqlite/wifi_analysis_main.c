#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include "wifi_analysis_main.h"
#include "radiotap_iter.h"
#if defined(__APPLE__)
#include <machine/endian.h>
#else
#include <endian.h>
#endif

#include <stdio.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SQL_BUFF_SIZE 256
sqlite3 *db;
char *zErrMsg = 0;
char sql[SQL_BUFF_SIZE];
//sqlite3_get_table
struct packet_info pcap_data;


static const struct radiotap_align_size align_size_000000_00[] = {
	[0] = { .align = 1, .size = 4, },
	[52] = { .align = 1, .size = 4, },
};

static const struct ieee80211_radiotap_namespace vns_array[] = {
	{
		.oui = 0x000000,
		.subns = 0,
		.n_bits = sizeof(align_size_000000_00),
		.align_size = align_size_000000_00,
	},
};
static const struct ieee80211_radiotap_vendor_namespaces vns = {
	.ns = vns_array,
	.n_ns = sizeof(vns_array)/sizeof(vns_array[0]),
};

static const struct radiotap_override overrides[] = {
	{ .field = 14, .align = 4, .size = 4, }
};

#define MAXBYTE2CAPTURE 2048
#define IEEE80211_CHAN_A (IEEE80211_CHAN_5GHZ | IEEE80211_CHAN_OFDM)
#define IEEE80211_CHAN_G (IEEE80211_CHAN_2GHZ | IEEE80211_CHAN_OFDM)
//#define MAC2ADDR(nane)	"%02x:%02x:%02x:%02x:%02x:%02x \n",#name[0],#name[1],#name[2],#name[3],#name[4],#name[5]

int isprint(char c)
{
    return 0;
}

void print_buf(u_char* pBuf, u_int32 len)
{
    if (!pBuf)
    {
        return;
    }

    for(int i=0; i<len; i++)
    {
        printf("%02x ",  pBuf[i]);

        if ((i%16 == 0 && i!=0) || i == len-1)
        {
            printf("\r\n");
        }
    }
}
static void print_radiotap_namespace(struct ieee80211_radiotap_iterator *iter)
{

	switch (iter->this_arg_index) {
	case IEEE80211_RADIOTAP_TSFT:
		//printf("\tTSFT: %llu\n", le64toh(*(unsigned long long *)iter->this_arg));
		break;
	case IEEE80211_RADIOTAP_FLAGS:
		//printf("\tflags: %02x\n", *iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_RATE:
		//printf("\trate: %lf\n", (double)*iter->this_arg/2);
		break;
	case IEEE80211_RADIOTAP_CHANNEL:
	    iter->this_arg = iter->this_arg + 2; // 通道信息如2G、5G，等
        int x = le16toh(*(uint16_t*)iter->this_arg);
        //printf("\tfreq: %d type: ", le16toh(*(uint16_t*)iter->this_arg)); // 信道
        if ((x & IEEE80211_CHAN_A) == IEEE80211_CHAN_A)
        {
         //   printf("A\n");
        }
        else if ((x & IEEE80211_CHAN_G) == IEEE80211_CHAN_G)
        {
         //   printf("G\n");
        }
        else if ((x & IEEE80211_CHAN_2GHZ) == IEEE80211_CHAN_2GHZ)
        {
         //   printf("B\n");
        }
		break;
	case IEEE80211_RADIOTAP_FHSS:
		break;
	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
        //printf("\tsignal: %d dBm\n", *(signed char*)iter->this_arg);
		break;
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
	case IEEE80211_RADIOTAP_ANTENNA:
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
	case IEEE80211_RADIOTAP_TX_FLAGS:
		break;
	case IEEE80211_RADIOTAP_RX_FLAGS:
		if (1) {
		//	printf("\tFCS in header: %.8x\n",
		//		le32toh(*(uint32_t *)iter->this_arg));
			break;
		}
		//printf("\tRX flags: %#.4x\n",
		//	le16toh(*(uint16_t *)iter->this_arg));
		break;
	case IEEE80211_RADIOTAP_RTS_RETRIES:
	case IEEE80211_RADIOTAP_DATA_RETRIES:
		break;
	default:
		printf("\tBOGUS DATA\n");
		break;
	}
}

static void print_test_namespace(struct ieee80211_radiotap_iterator *iter)
{
	switch (iter->this_arg_index) {
	case 0:
	case 52:
		printf("\t00:00:00-00|%d: %.2x/%.2x/%.2x/%.2x\n",
			iter->this_arg_index,
			*iter->this_arg, *(iter->this_arg + 1),
			*(iter->this_arg + 2), *(iter->this_arg + 3));
		break;
	default:
		printf("\tBOGUS DATA - vendor ns %d\n", iter->this_arg_index);
		break;
	}
}

int parse_radiotap(u_char* pData, u_int32 len, u_char* length)
{
	struct ieee80211_radiotap_iterator iter;
	int err = 0, i = 0;
    if (!pData || len <14)
    {
    	printf("data is wrong!\n");
        return 2;
    }
	err = ieee80211_radiotap_iterator_init(&iter, pData, len, &vns);
	if (err) {
		printf("malformed radiotap header (init returns %d)\n", err);
		return 3;
	}
	
	iter.overrides = overrides;
	iter.n_overrides = sizeof(overrides)/sizeof(overrides[0]);
	//printf("radiotap frame: \r\n");
   // print_buf(pData, 14);
   	//printf("\tLength:%d \n",iter._max_length);
   	*length = iter._max_length;
	while (!(err = ieee80211_radiotap_iterator_next(&iter))) {
		if (iter.this_arg_index == IEEE80211_RADIOTAP_VENDOR_NAMESPACE) {
			printf("\tvendor NS (%.2x-%.2x-%.2x:%d, %d bytes)\n",
				iter.this_arg[0], iter.this_arg[1],
				iter.this_arg[2], iter.this_arg[3],
				iter.this_arg_size - 6);
			for (i = 6; i < iter.this_arg_size; i++) {
				if (i % 8 == 6)
					printf("\t\t");
				else
					printf(" ");
				printf("%.2x", iter.this_arg[i]);
			}
			printf("\n");
		} else if (iter.is_radiotap_ns)
			print_radiotap_namespace(&iter);
		else if (iter.current_namespace == &vns_array[0])
			print_test_namespace(&iter);
	}


	if (err != -ENOENT) {
		printf("malformed radiotap data\n");
		return 3;
	}
 
	return 0;

}
// 802.11帧头
struct wlan_frame {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		addr1[6];
	uint8_t		addr2[6];
	uint8_t		addr3[6];
	uint16_t	seq;
	union {
		uint16_t		qos;
		uint8_t			addr4[6];
		struct {
			uint16_t	qos;
			uint32_t	ht;
		} __attribute__ ((packed)) ht;
		struct {
			uint8_t		addr4[6];
			uint16_t	qos;
			uint32_t	ht;
		} __attribute__ ((packed)) addr4_qos_ht;
	} u;
} __attribute__ ((packed));
const char* ether_sprintf(const unsigned char *mac)
{
	static char etherbuf[18];
	snprintf(etherbuf, sizeof(etherbuf), "%02x:%02x:%02x:%02x:%02x:%02x",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return etherbuf;
}
/* Note: mcs must be at least 13 bytes long! In theory its 16 byte */
void ht_streams_from_mcs_set(unsigned char* mcs, unsigned char* rx, unsigned char* tx)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (!mcs[i])
			break;
	}
	*rx = i;

	bool tx_mcs_defined = mcs[12] & 0x01;
	bool tx_rx_mcs_not_equal = !!(mcs[12] & 0x02);
	char tx_max_streams = ((mcs[12] & 0x0c) >> 2) + 1;

	if (tx_mcs_defined && !tx_rx_mcs_not_equal)
		*tx = *rx;
	else if (tx_mcs_defined && tx_rx_mcs_not_equal)
		*tx = tx_max_streams;
}
enum chan_width chan_width_from_vht_capab(uint32_t vht)
{
	switch (((vht & WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH) >> 2)) {
		case WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_80: return CHAN_WIDTH_80;
		case WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_160: return CHAN_WIDTH_160;
		case WLAN_IE_VHT_CAPAB_INFO_CHAN_WIDTH_BOTH: return CHAN_WIDTH_8080;
		default: printf("(reserved)\n"); return CHAN_WIDTH_UNSPEC;
	}
}
/* Note: mcs must be at least 6 bytes long! In theory its 8 byte */
void vht_streams_from_mcs_set(unsigned char* mcs, unsigned char* rx, unsigned char* tx)
{
	int i;
	/* RX */
	uint16_t tmp = mcs[0] | (mcs[1] << 8);
	for (i = 0; i < 8; i++) {
		if (((tmp >> (i*2)) & 3) == 3)
			break;
	}
	*rx = i;

	/* TX */
	tmp = mcs[4] | (mcs[5] << 8);
	for (i = 0; i < 8; i++) {
		if (((tmp >> (i*2)) & 3) == 3)
			break;
	}
	*tx = i;
}
static int sqlit_callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
	printf("-----------------------------%s = %s\n", azColName[2], argv[2] ? argv[2] : "NULL");
//   for(i=0; i<argc; i++){
//     printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//   }
   printf("\n");
   return 0;
}

static void wlan_parse_information_elements(unsigned char* buf, size_t bufLen, struct packet_info *p_pcap_data)
{
	int len = bufLen ;
	while (len > 2) {
		struct information_element* ie = (struct information_element*)buf;
		//printf("------ IE %d len %d t len %d\n", ie->id, ie->len, len);

		switch (ie->id) {
		case WLAN_IE_ID_SSID:
			if (ie->len < WLAN_MAX_SSID_LEN-1) {
				memcpy(p_pcap_data->wlan_essid, ie->var, ie->len);
				p_pcap_data->wlan_essid[ie->len] = '\0';
			} else {
				memcpy(p_pcap_data->wlan_essid, ie->var, WLAN_MAX_SSID_LEN-1);
				p_pcap_data->wlan_essid[WLAN_MAX_SSID_LEN-1] = '\0';
			}
			break;

		case WLAN_IE_ID_DSSS_PARAM:
			p_pcap_data->wlan_channel = *ie->var;
			break;

		case WLAN_IE_ID_RSN:
			p_pcap_data->wlan_rsn = 1;
			break;

		case WLAN_IE_ID_HT_CAPAB:
			//printf("HT %d %x\n", ie->len, ie->var[0]);
			if (ie->var[0] & WLAN_IE_HT_CAPAB_INFO_CHAN_WIDTH_40)
				p_pcap_data->wlan_chan_width = CHAN_WIDTH_40;
			else
				p_pcap_data->wlan_chan_width = CHAN_WIDTH_20;

			if (ie->len >= 26) {
				ht_streams_from_mcs_set(&ie->var[3], &p_pcap_data->wlan_rx_streams, &p_pcap_data->wlan_tx_streams);
				//printf("STREAMS %dx%d\n", p_pcap_data->wlan_tx_streams, p_pcap_data->wlan_rx_streams);
			}
			break;

		case WLAN_IE_ID_HT_OPER:
			printf("HT OPER %d %x\n", ie->len, ie->var[0]);
			if (ie->len > 1) {
				switch (ie->var[1] & WLAN_IE_HT_OPER_INFO_CHAN_OFFSET) {
					case 0: p_pcap_data->wlan_chan_width = CHAN_WIDTH_20; break;
					case 1: p_pcap_data->wlan_ht40plus = true; break;
					case 3: p_pcap_data->wlan_ht40plus = false; break;
					default: printf("HT OPER wrong?"); break;
				}
			}
			break;

		case WLAN_IE_ID_VHT_OPER:
		case WLAN_IE_ID_VHT_OMN:
			printf("VHT OPER %d %x\n", ie->len, ie->var[0]);
			p_pcap_data->wlan_chan_width = CHAN_WIDTH_80; /* minimum, otherwise not AC */
			break;

		case WLAN_IE_ID_VHT_CAPAB:
			printf("VHT %d %x\n", ie->len, ie->var[0]);
			if (ie->len >= 12) {
				p_pcap_data->wlan_chan_width = chan_width_from_vht_capab(ie->var[0]);
				vht_streams_from_mcs_set(&ie->var[4], &p_pcap_data->wlan_rx_streams, &p_pcap_data->wlan_tx_streams);
				printf("VHT STREAMS %dx%d\n", p_pcap_data->wlan_tx_streams, p_pcap_data->wlan_rx_streams);
			}
			break;

		case WLAN_IE_ID_VENDOR:
			if (ie->len >= 4 &&
			    ie->var[0] == 0x00 && ie->var[1] == 0x50 && ie->var[2] == 0xf2 && /* Microsoft OUI (00:50:F2) */
			    ie->var[3] == 1) {	/* OUI Type 1 - WPA IE */
				p_pcap_data->wlan_wpa=1;
			}
			break;
		}

		buf += ie->len + 2;
		len -= ie->len + 2;
	}
}
int is_in_sqlite(struct packet_info *p_pcap_data){
	int nResult = 0;
    char *strOut;
	char** pResult;
	int nRow;
	int nCol;
    int nIndex;
	int is_ap = 0;
	char sta_mac[18] ={0},ap_mac[18] ={0},bssid_mac[18] ={0};
	memset(sql, 0, SQL_BUFF_SIZE);
	sprintf(sta_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_src[0],p_pcap_data->wlan_src[1],\
			p_pcap_data->wlan_src[2],p_pcap_data->wlan_src[3],p_pcap_data->wlan_src[4],p_pcap_data->wlan_src[5]);
	//printf("ssssssss %s sss\n",sta_mac);
	sprintf(ap_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_dst[0],p_pcap_data->wlan_dst[1],\
			p_pcap_data->wlan_dst[2],p_pcap_data->wlan_dst[3],p_pcap_data->wlan_dst[4],p_pcap_data->wlan_dst[5]);
	//printf("ssssssss %s sss\n",ap_mac);
	sprintf(bssid_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_bssid[0],p_pcap_data->wlan_bssid[1],\
			p_pcap_data->wlan_bssid[2],p_pcap_data->wlan_bssid[3],p_pcap_data->wlan_bssid[4],p_pcap_data->wlan_bssid[5]);
	//printf("ssssssss %s sss\n",bssid_mac);
		// is data in db 
	sprintf(sql, "SELECT * from MAC_ADDR WHERE STA_MAC='%s' and AP_MAC='%s' and BSSID_MAC='%s'"\
			, is_ap?ap_mac:sta_mac, is_ap?sta_mac:ap_mac, bssid_mac);
	//printf("ssssssss %s sss\n",sql);
	nResult = sqlite3_get_table(db,sql,&pResult,&nRow,&nCol,&zErrMsg);
	  if (nResult != SQLITE_OK)
	{
		sqlite3_close(db);
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}
	nIndex = nCol;
	//printf("----start step 1--%d %d--\n",nRow,nCol);
	if(nRow == 0){
		printf("\t----mac is not in OUI.\n");
		return 0;
	}
	for(int i=0;i<nRow;i++)
	{
		//printf("----start step 2-------------------------------------------------\n");
		printf("\t----mac is  in %d %s = %s\n",nResult,pResult[2],pResult[nIndex + 2]);
	}
	sqlite3_free_table(pResult);
	return 1;
}

int insert_sqlite(struct packet_info *p_pcap_data){

	int is_ap = 0, rc = 0;
	char sta_mac[18] ={0},ap_mac[18] ={0},bssid_mac[18] ={0};
	memset(sql, 0, SQL_BUFF_SIZE);
	//create insert sql
	sprintf(sta_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_src[0],p_pcap_data->wlan_src[1],\
			p_pcap_data->wlan_src[2],p_pcap_data->wlan_src[3],p_pcap_data->wlan_src[4],p_pcap_data->wlan_src[5]);
	sprintf(ap_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_dst[0],p_pcap_data->wlan_dst[1],\
			p_pcap_data->wlan_dst[2],p_pcap_data->wlan_dst[3],p_pcap_data->wlan_dst[4],p_pcap_data->wlan_dst[5]);
	sprintf(bssid_mac,"%02X:%02X:%02X:%02X:%02X:%02X",p_pcap_data->wlan_bssid[0],p_pcap_data->wlan_bssid[1],\
			p_pcap_data->wlan_bssid[2],p_pcap_data->wlan_bssid[3],p_pcap_data->wlan_bssid[4],p_pcap_data->wlan_bssid[5]);
	sprintf(sql,"INSERT INTO MAC_ADDR (ID,TYPE,SUBTYPE,STA_MAC,AP_MAC, BSSID_MAC,FLAGS ) VALUES ( NULL, %d, %d, '%s', '%s', '%s', %X );",\
		  p_pcap_data->wlan_mode & WLAN_FRAME_FC_TYPE_MASK, p_pcap_data->wlan_mode & WLAN_FRAME_FC_STYPE_MASK, is_ap?ap_mac:sta_mac, is_ap?sta_mac:ap_mac, bssid_mac, p_pcap_data->wlan_mode); 
	//printf(" ----insert %s------\n",sql);
		/* Execute SQL statement */
   rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
   }else{
	  fprintf(stdout, "Table created successfully\n");
   }
   return 0;
}
//int parse_packet_wlan(const char* buffer, int len)
#if 0 // lx
int sql_mac_name(uint8_t *addr,char *sql){
	int nResult = 0;
    char *strOut;
	char** pResult;
	int nRow;
	int nCol;
    int nIndex;
	//printf("----start----\n");
	sprintf(sql,"SELECT * from OUI WHERE MAC='%02X-%02X-%02X'",addr[0],addr[1],addr[2]);
 	nResult = sqlite3_get_table(db,sql,&pResult,&nRow,&nCol,&zErrMsg);
      if (nResult != SQLITE_OK)
    {
        sqlite3_close(db);
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
    }
	nIndex = nCol;
	//printf("----start step 1--%d %d--\n",nRow,nCol);
	if(nRow == 0){
		printf("\t----mac is not in OUI.\n");
	}
    for(int i=0;i<nRow;i++)
    {
    	//printf("----start step 2-------------------------------------------------\n");
		printf("\t---- %s = %s\n",pResult[2],pResult[nIndex + 2]);
    }
    sqlite3_free_table(pResult);
	return nResult;
    //sqlite3_close(db);	
}
#endif //lx
int parse_packet_80211(u_char* buf, u_int32 len){
	int hdrlen = 0 , rc = 0;
	uint16_t fc = 0, cap_i;
	uint8_t* ra = NULL;
	uint8_t* ta = NULL;
	uint8_t* bssid = NULL;
	//struct packet_info p;
	struct wlan_frame* wh = NULL;
	
	if (10>= len)
	{
		printf("minimum frame size (CTS/ACK)!");
		return -1;
	}
 
	wh = (struct wlan_frame*)buf;
	
	fc = le16toh(wh->fc); // 传输格式为little endian，要转换成host格式
	pcap_data.wlan_type = (fc & WLAN_FRAME_FC_MASK);
	//lxprintf("fc:0x%x wlan_type 0x%x - type 0x%x - stype 0x%x \n", fc, fc & WLAN_FRAME_FC_TYPE_MASK, fc & WLAN_FRAME_FC_STYPE_MASK);
	
	if (WLAN_FRAME_IS_DATA(fc)) {
	
		hdrlen = 24;
		if (WLAN_FRAME_IS_QOS(fc)) {
			hdrlen += 2;
			if (fc & WLAN_FRAME_FC_ORDER)
				hdrlen += 4;
		}

		/* AP, STA or IBSS */
		if ((fc & WLAN_FRAME_FC_FROM_DS) == 0 &&
			(fc & WLAN_FRAME_FC_TO_DS) == 0) {
			pcap_data.wlan_mode = WLAN_MODE_IBSS;
			bssid = wh->addr3;
		} else if ((fc & WLAN_FRAME_FC_FROM_DS) &&
			   (fc & WLAN_FRAME_FC_TO_DS)) {
			pcap_data.wlan_mode = WLAN_MODE_4ADDR;
			hdrlen += 6;
			if (WLAN_FRAME_IS_QOS(fc)) {
				uint16_t qos = le16toh(wh->u.addr4_qos_ht.qos);
				printf("4ADDR A-MSDU %x\n", qos & WLAN_FRAME_QOS_AMSDU_PRESENT);
				if (qos & WLAN_FRAME_QOS_AMSDU_PRESENT)
					bssid = wh->addr3;
				// in the MSDU case BSSID is unknown
			}
		} else if (fc & WLAN_FRAME_FC_FROM_DS) {
			pcap_data.wlan_mode = WLAN_MODE_AP;
			bssid = wh->addr2;
		} else if (fc & WLAN_FRAME_FC_TO_DS) {
			pcap_data.wlan_mode = WLAN_MODE_STA;
			bssid = wh->addr1;
		}

		if (len < hdrlen)
			return -1;

		pcap_data.wlan_nav = le16toh(wh->duration);
		printf("DATA NAV %d\n", pcap_data.wlan_nav);
		pcap_data.wlan_seqno = (le16toh(wh->seq) & WLAN_FRAME_SEQ_MASK) >> 4;
		printf("DATA SEQ %d\n", pcap_data.wlan_seqno);
		printf("A1 %s\n", ether_sprintf(wh->addr1));
		printf("A2 %s\n", ether_sprintf(wh->addr2));
		printf("A3 %s\n", ether_sprintf(wh->addr3));
		if (pcap_data.wlan_mode == WLAN_MODE_4ADDR) {
			printf("A4 %s\n", ether_sprintf(wh->u.addr4));
		}
		printf("ToDS %d FromDS %d\n", (fc & WLAN_FRAME_FC_FROM_DS) != 0, (fc & WLAN_FRAME_FC_TO_DS) != 0);

		ra = wh->addr1;
		ta = wh->addr2;

		/* WEP */
		if (fc & WLAN_FRAME_FC_PROTECTED)
			pcap_data.wlan_wep = 1;

		if (fc & WLAN_FRAME_FC_RETRY)
			pcap_data.wlan_retry = 1;

	} else if (WLAN_FRAME_IS_CTRL(fc)) {
		if (pcap_data.wlan_type == WLAN_FRAME_CTS ||
			pcap_data.wlan_type == WLAN_FRAME_ACK)
			hdrlen = 10;
		else
			hdrlen = 16;

		if (len < hdrlen)
			return -1;

	} else if (WLAN_FRAME_IS_MGMT(fc)) {
		hdrlen = 24;
		if (fc & WLAN_FRAME_FC_ORDER)
			hdrlen += 4;

		if (len < hdrlen)
			return -1;

		ra = wh->addr1;
		ta = wh->addr2;
		bssid = wh->addr3;
		pcap_data.wlan_seqno = (le16toh(wh->seq) & WLAN_FRAME_SEQ_MASK) >> 4;
		//printf("MGMT SEQ %d\n", pcap_data.wlan_seqno);

		if (fc & WLAN_FRAME_FC_RETRY)
			pcap_data.wlan_retry = 1;
	} else {
		printf("!!!UNKNOWN FRAME!!!");
		return -1;
	}

	pcap_data.wlan_len = len;

	switch (pcap_data.wlan_type) {
		case WLAN_FRAME_NULL:
			break;

		case WLAN_FRAME_QDATA:
			pcap_data.wlan_qos_class = le16toh(wh->u.qos) & WLAN_FRAME_QOS_TID_MASK;
			printf("***QDATA %x\n", pcap_data.wlan_qos_class);
			break;

		case WLAN_FRAME_RTS:
			pcap_data.wlan_nav = le16toh(wh->duration);
			printf("RTS NAV %d\n", pcap_data.wlan_nav);
			ra = wh->addr1;
			ta = wh->addr2;
			break;

		case WLAN_FRAME_CTS:
			pcap_data.wlan_nav = le16toh(wh->duration);
			printf("CTS NAV %d\n", pcap_data.wlan_nav);
			ra = wh->addr1;
			break;

		case WLAN_FRAME_ACK:
			pcap_data.wlan_nav = le16toh(wh->duration);
			printf("ACK NAV %d\n", pcap_data.wlan_nav);
			ra = wh->addr1;
			break;

		case WLAN_FRAME_PSPOLL:
			ra = wh->addr1;
			bssid = wh->addr1;
			ta = wh->addr2;
			break;

		case WLAN_FRAME_CF_END:
		case WLAN_FRAME_CF_END_ACK:
			ra = wh->addr1;
			ta = wh->addr2;
			bssid = wh->addr2;
			break;

		case WLAN_FRAME_BLKACK:
		case WLAN_FRAME_BLKACK_REQ:
			pcap_data.wlan_nav = le16toh(wh->duration);
			ra = wh->addr1;
			ta = wh->addr2;
			break;

		case WLAN_FRAME_BEACON:
		case WLAN_FRAME_PROBE_RESP:
			;
			struct wlan_frame_beacon* bc = (struct wlan_frame_beacon*)(buf + hdrlen);
			pcap_data.wlan_tsf = le64toh(bc->tsf);
			pcap_data.wlan_bintval = le16toh(bc->bintval);
			//printf("TSF %u\n BINTVAL %u", pcap_data.wlan_tsf, pcap_data.wlan_bintval);

			wlan_parse_information_elements(bc->ie,
				len - hdrlen - sizeof(struct wlan_frame_beacon) - 4 /* FCS */, &pcap_data);
			printf("ESSID %s \n", pcap_data.wlan_essid );
			printf("CHAN %d \n", pcap_data.wlan_channel );
			cap_i = le16toh(bc->capab);
			if (cap_i & WLAN_CAPAB_IBSS)
				pcap_data.wlan_mode = WLAN_MODE_IBSS;
			else if (cap_i & WLAN_CAPAB_ESS)
				pcap_data.wlan_mode = WLAN_MODE_AP;
			if (cap_i & WLAN_CAPAB_PRIVACY)
				pcap_data.wlan_wep = 1;
			break;

		case WLAN_FRAME_PROBE_REQ:
			wlan_parse_information_elements(buf + hdrlen,
				len - hdrlen - 4 /* FCS */, &pcap_data);
			pcap_data.wlan_mode = WLAN_MODE_PROBE;
			break;

		case WLAN_FRAME_ASSOC_REQ:
		case WLAN_FRAME_ASSOC_RESP:
		case WLAN_FRAME_REASSOC_REQ:
		case WLAN_FRAME_REASSOC_RESP:
		case WLAN_FRAME_DISASSOC:
			break;

		case WLAN_FRAME_AUTH:
			if (fc & WLAN_FRAME_FC_PROTECTED)
				pcap_data.wlan_wep = 1;
				/* no break */
		case WLAN_FRAME_DEAUTH:
			break;

		case WLAN_FRAME_ACTION:
			break;
	}

	if (ta != NULL) {
		memcpy(pcap_data.wlan_src, ta, MAC_LEN);
		if((ta[0] == 0xff)&&(ta[1] == 0xff)&&(ta[2] == 0xff)){
			// data is unuseful
		}else{
			if(ta[5] == 0x40)
				printf("======================================\n");
				 /* Create SQL statement */
			//sprintf(sql,"SELECT * from OUI WHERE MAC='%02X-%02X-%02X'",ta[0],ta[1],ta[2]);
			//printf("sql = %s \n",sql);
			//sql_mac_name(ta,sql);
			printf("TA	 %s\n", ether_sprintf(ta));
		}
	}
	if (ra != NULL) {
		memcpy(pcap_data.wlan_dst, ra, MAC_LEN);
		if((ra[0] == 0xff)&&(ra[1] == 0xff)&&(ra[2] == 0xff)){
			// data is unuse
		}else{
			printf("RA	 %s\n", ether_sprintf(ra));
		   	//sql_mac_name(ra,sql);
		}
	}
	if (bssid != NULL) {
		memcpy(pcap_data.wlan_bssid, bssid, MAC_LEN);
		if((bssid[0] == 0xff)&&(bssid[1] == 0xff)&&(bssid[2] == 0xff)){
			// data is unuse
		}else{
			printf("BSSID %s\n", ether_sprintf(bssid));
			//sql_mac_name(bssid,sql);
		}
	}
	if(is_in_sqlite(&pcap_data) == 0 ){//is a new data
		insert_sqlite(&pcap_data);
	}
	/* only data frames contain more info, otherwise stop parsing */
	if (WLAN_FRAME_IS_DATA(pcap_data.wlan_type) && pcap_data.wlan_wep != 1) {
		return hdrlen;
	}

 
	return 0;
}

void parse_packet(const u_char* packet, u_int32 len)
{
    u_short ftype = 0;
	int err = 0;
	int offset = 0;
    if (!packet)
    {
    	 printf("NO data be catch\n");
        return ;
    }

    u_char* pMbuf = (u_char*)packet;
    err = parse_radiotap(pMbuf, len,&offset);
	if(err != 0){
		printf("parse_radiotap failed\n");
		return;
	}
	parse_packet_80211(pMbuf+offset, len-offset);

    printf("\r\n");
}
/*
 a const struct pcap_pkthdr pointer pointing to the packet time stamp and lengths,
 and a const u_char pointer to the first caplen (as given in the struct pcap_pkthdr
 a pointer to which is passed to the callback routine) bytes of data from the packet
*/
void processPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int i = 0, *counter = (int *)arg;

    printf("--------------------------------------------\r\n");
   // printf("Packet Count: %d\n", ++(*counter));
   // printf("Received Packet Size: %d\n", pkthdr->len);
    //printf("Payload:\n");

#if 0 //打印数据
    for (i = 0; i < pkthdr->len; i++)
    {
        if (isprint(packet[i]))
        {
            printf("%02d ", packet[i]);
        }
        else
        {
            printf("%02x ", packet[i]);
        }

        if ((i % 16 == 0 && i != 0) || i == pkthdr->len-1)
        {
            printf("\n");
        }

    }
#endif
// 解析数据
    parse_packet(packet, pkthdr->len);
	//printf("parse data 1 fram ok:\n");
    return;
}
int creat_sqlite(){
	int i = 0, count = 0,rc =0, fd_tmp = 0;
	char *sql_tem;
    pcap_t *descr = NULL;
    char errbuf[PCAP_ERRBUF_SIZE], *device = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
	memset(sql, 0, SQL_BUFF_SIZE);
	    /* Create SQL statement  AUTOINCREMENT--auto incress */
   sql_tem = "CREATE TABLE MAC_ADDR("  \
         "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
         "TYPE INT  NOT NULL," \
         "SUBTYPE INT NOT NULL," \
         "STA_MAC TEXT," \
		 "AP_MAC TEXT," \
		 "BSSID_MAC TEXT," \
         "FLAGS INT);";
		// create file
	fd_tmp = open("/data/mac_addr.db3",O_RDWR|O_CREAT,0666);
	close(fd_tmp);
   rc = sqlite3_open("/data/mac_addr.db3", &db);
   if( rc ){
      printf("Can't open database: %s\n", sqlite3_errmsg(db));
      return -1;
   }else{
      printf("Opened database successfully\n");
   }
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql_tem, NULL, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   	printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      printf("Table created successfully\n");
   }
   return 0;
}
int main()
{

    int i = 0, count = 0,rc =0;
    pcap_t *descr = NULL;
    char errbuf[PCAP_ERRBUF_SIZE], *device = NULL;
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
	pcap_if_t *alldevs;
 	pcap_if_t *d;
	
    /* Retrieve the device list */
    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        printf("Error in pcap_findalldevs: %s\n", errbuf);
        return -1;
    }

    /* Print the list */
    for(d=alldevs; d; d=d->next)
    {
       // printf("%d. %s", ++i, d->name);
		if(strncmp(d->name,"wlanxx",4)==0){
			device = d->name;
			break;
		}
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }
	//release memery
	//pcap_freealldevs(alldevs);
    printf("Opening device %s\n", device);
	
	memset(errbuf, 0, PCAP_ERRBUF_SIZE);
    /* Open device in promiscuous mode 1-->specifies if the interface is to be put into promiscuous mode.
    	512-->specifies the packet buffer timeout,
    	errbuf -->return errbuf or warning text
    	 returns a pcap_t * on success and NULL on failure*/
    descr = pcap_open_live(device, MAXBYTE2CAPTURE, 1, 512, errbuf);
	if(NULL == descr){
		//release memery
		pcap_freealldevs(alldevs);
		return -1;
	}
	/*rc = sqlite3_open("/data/mac.db3", &db);
   if( rc ){
      printf("Can't open database: %s\n", sqlite3_errmsg(db));
      return -1;
   }else{
      printf("Opened database successfully\n");
   }
   */
	creat_sqlite();
    /* Loop forever & call processPacket() for every received packet 
	cnt is the maximum number of packets to process before returning  
	A value of -1 or 0 for cnt causes all the packets received*/
    pcap_loop(descr, -1, processPacket, (u_char *)&count);
   
	sqlite3_close(db);
    return 0;
}
