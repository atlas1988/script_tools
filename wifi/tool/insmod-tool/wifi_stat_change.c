#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#define CONFIG_BUF_SIZE 64
struct wifi_mode {
	char con_mode;
	char wifi_driver[CONFIG_BUF_SIZE];
	char wifi_ifconfig[CONFIG_BUF_SIZE];
	char iwpriv_setting[CONFIG_BUF_SIZE];
	char iwpriv_wlan_up[CONFIG_BUF_SIZE];
};
static struct wifi_mode wifi_config;
static void load_driver() {
    system(wifi_config.wifi_driver);
    printf("exec '%s' to load wifi driver\n", wifi_config.wifi_driver);
}

static void unload_driver() {
	if(wifi_config.con_mode == 4){
		system("iwpriv wlan0 monitor 0 ");
    	printf("exec 'iwpriv wlan0 monitor 0\n");
	}
    system("rmmod wlan");
    printf("exec 'rmmod wlan' to unload wifi driver\n");
}
static void config_iwpriv(){
	//iwpriv wlan0 MonitorModeConf 4 20 0 111 0
	//iwpriv wlan0 monitor 1
    system(wifi_config.iwpriv_setting);	
    printf("exec '%s' to config wifi\n", wifi_config.iwpriv_setting);
	
    system( wifi_config.iwpriv_wlan_up);
    printf("exec '%s' to config wifi\n",  wifi_config.iwpriv_wlan_up);
}
static void config_wlan() {

    system(wifi_config.wifi_ifconfig);	
    printf("exec '%s' to config wifi\n", wifi_config.wifi_ifconfig);
}

int main()
{

    int i = 0, count = 0;
    char *device = NULL;
	//wifi config init
	wifi_config.con_mode = 4;// mode 4 monitor
	//snprintf(temp, sizeof(temp), "insmod %s", get_value("wifi_driver"));
	snprintf(wifi_config.wifi_driver, CONFIG_BUF_SIZE, "insmod vendor/lib/modules/wlan.ko con_mode=4");
	snprintf(wifi_config.wifi_ifconfig, CONFIG_BUF_SIZE, "ifconfig wlan0 up");
	snprintf(wifi_config.iwpriv_setting, CONFIG_BUF_SIZE, "iwpriv wlan0 MonitorModeConf 4 20 0 111 0");
	snprintf(wifi_config.iwpriv_wlan_up, CONFIG_BUF_SIZE, "iwpriv wlan0 monitor 1");
	//system("su");
	 uid_t uid = getuid();                //普通用户的uid
     if (setuid(0)) {     
	 //设置为普通用户和超级用户一样的权限
	 	printf("setuid error!\n");
     	return -1;
     }
	// execl("/system/bin/lsmod"," ");
	// execl("/system/bin/insmod","vendor/lib/modules/wlan.ko");
    load_driver();
	config_iwpriv();
    config_wlan();
	//unload_driver();
	 if (setuid(uid)) { //恢复uid            恢复到只有普通用户的权限
	 printf("restor uid\n");
   }
    return 0;
}