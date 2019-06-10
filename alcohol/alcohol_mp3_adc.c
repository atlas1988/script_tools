#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/wakelock.h>
#include <linux/kthread.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/spi/spidev.h>
#include <linux/semaphore.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/input.h>
#include <linux/signal.h>
#include <linux/gpio.h>
#include <linux/mm.h>
#include <linux/of_gpio.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
//#include <linux/of_irq.h>
#include <linux/regulator/consumer.h>
#include<linux/input-polldev.h>
//#include <linux/fb.h>
//#include <linux/notifier.h>

#include <linux/qpnp/qpnp-adc.h>

#include <linux/printk.h>

#include <linux/fs.h>
#include <linux/uaccess.h>
#ifdef CONFIG_HY_DRV_ASSIST
#include <linux/hy-assist.h>
#endif
#include <linux/time.h>
#include <linux/timer.h>

//#define DEBUG_ENABLE
#ifndef DEBUG_ENABLE
#define PR_DEBUG(msg, args...)   
#else
#define PR_DEBUG(msg, args...) printk(KERN_ERR msg, ##args);
#endif //DEBUG_ON


static struct class* alcohol_class = NULL;

typedef struct key_report {
	int key;
	int value;
}key_report_t;


#define CONFIG_ALCOHOL_USE_TIMER

#define CONFIG_ALCOHOL_USE_CALIBRATION
//#define CONFIG_INPUT_EVENT_POLL
#define ALCOHOL_MIN_MEASURE_VAL 0
#define ALCOHOL_MAX_MEASURE_VAL 0x7fffffff
// input poll interval in milliseconds
#define ALCOHOL_POLL_INTERVAL 900
#define ALCOHOL_POLL_MIN 200
#define ALCOHOL_POLL_MAX 2000

#define DEVICE_NAME "mp3"
#define INPUT_DEVICE_NAME "alcohol"
#define EOF 0xff
//#define SUPPORT_ID_NUM
//#define POWER_REGULATOR
#if defined(CONFIG_ALCOHOL_USE_TIMER)
#define ALCOHOL_INIT_TIMER_S 180
static struct timer_list tm;
//struct timeval oldtv;
unsigned long old_jaffs;
#endif

static unchar alcohoc_stat = 0;
//static int isInKeyMode = 0; // key mode
//static int screen_status = 1; // screen on
static u8 alcohol_mp3_debug = 0x01;
//static int isInit = 0;


#define AlCOHOL_DBG(fmt, args...) \
	do{ \
		if(alcohol_mp3_debug & 0x01) \
		printk( KERN_ERR"[DBG][alcohol_mp3]:%5d: <%s>" fmt, __LINE__,__func__,##args ); \
	}while(0)

#define AlCOHOL_ERR(fmt, args...) \
	do{ \
		printk( KERN_ERR"[DBG][alcohol_mp3]:%5d: <%s>" fmt, __LINE__,__func__,##args ); \
	}while(0)

struct alcohol_mp3_data {
	struct platform_device *alcohol_mp3_dev;
	struct miscdevice *miscdev;
#ifdef SUPPORT_ID_NUM
	u32 id_num;
	u8 chip_id;
	struct pinctrl *s_pinctrl;
	struct pinctrl_state *s_id_high;
#endif
#ifndef POWER_REGULATOR
	u32 pwr_num;
#else
	struct regulator *vdd;
#endif
	int alcohoc_value;
	struct fasync_struct *async_queue;
	struct wake_lock alcohol_mp3_lock;
//	struct notifier_block notifier;
	struct mutex buf_lock;
#ifdef CONFIG_INPUT_EVENT_POLL
	struct input_polled_dev *input_polled;
#endif
	struct input_dev* alcohol_mp3_input;
}*g_alcohol_mp3_data;

static int alcohol_mp3_power_on(struct alcohol_mp3_data *pdata);
#if defined(CONFIG_ALCOHOL_USE_TIMER)
void timer_callback(unsigned long arg)
{
//    char *strp = (char*)arg;
    alcohoc_stat = 2;
	PR_DEBUG("atlas alcohoc_stat =%d \n",alcohoc_stat);

//    tm.expires = jiffies+1*HZ;    
   // add_timer(&tm);        
 #if defined(CONFIG_ALCOHOL_USE_TIMER)
	 del_timer(&tm);
#endif
}

#endif

struct alcohol_mp3_value_map {
	int adc_val;
	int alcohol_val;
};
static struct alcohol_mp3_value_map adc_alcohol[] = {
	{ 325000, 0 },
	{ 1400000, 130},
	{ 1890000, 300},
	{ 0x7fffffff, 0x7fffffff},
	{ 0x7fffffff, 0x7fffffff}
};
#ifdef CONFIG_ALCOHOL_USE_CALIBRATION
int is_calibration = 0;
int read_calibration_data(void){
	int ret = 0,i = 0, j = 0;
	struct file *fp;
    mm_segment_t fs;
    loff_t pos;
	char buf[256] = {0};
	char *p_buf = buf;
	int z = 0;
	struct alcohol_mp3_value_map adc_tmp;
	if(is_calibration){
		printk("read_calibration_data already been done/n");
		return 0;
	}
    //printk("read_calibration_data enter\n");
    fp = filp_open("/data/data/AdcToAlcohol.txt",O_RDONLY,0444);
    if (IS_ERR(fp)){
        printk("atlas --open file error\n");
        return -1;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
	pos =0;
    vfs_read(fp, buf, sizeof(buf), &pos);
	printk("read_calibration_data enter %s  2\n",buf);
	for(i = 0, j = 0; buf[i] !=EOF; i++){
		if(buf[i] =='\x0a'){  // 换行
			j++;
			if(j >= 9){
				PR_DEBUG("atlas --read_calibration_data error--\n");
				return -2;
			}
			ret = sscanf(p_buf, "%d,%d", &adc_tmp.adc_val, &adc_tmp.alcohol_val);
			printk("atlas lx  -- read p_buf: %s ret%d adc%d \n",p_buf,ret,adc_tmp.adc_val);
			if(adc_tmp.alcohol_val == 130){
					if((adc_tmp.adc_val >= 1200000) && (adc_tmp.adc_val < 1400000)){
						for(z = 0;adc_alcohol[z].alcohol_val != 130;z++);
						PR_DEBUG("atlas lx  ++++++-- %d adc%d \n",z,adc_alcohol[z].adc_val);
						if(adc_tmp.alcohol_val == 130){
							adc_alcohol[z].adc_val = adc_tmp.adc_val;
							//adc_alcohol[z].alcohol_val = adc_tmp.alcohol_val;
						}
				}
				p_buf = buf + i + 1;
				//msleep(10);
			}
		}
	}
	j++;
	if( z == 1)
		is_calibration = 1;
	//j++;
#if 0
	for(i = 0, j = 0; buf[i] !=EOF; i++){
		if(buf[i] =='\x0a'){  // 换行
			j++;
			if(j >= 9){
				PR_DEBUG("atlas --read_calibration_data error/n");
				return -2;
			}
			ret = sscanf(p_buf, "%d,%d", &adc_alcohol[j].adc_val, &adc_alcohol[j].alcohol_val);
			PR_DEBUG("atlas lx  -- read p_buf: %s ret%d adc%d \n",p_buf,ret,adc_alcohol[j].adc_val);
			p_buf = buf + i + 1;
			msleep(10);
		}
	}
	j++;
	if(2 == sscanf(p_buf, "%d,%d", &adc_alcohol[j].adc_val, &adc_alcohol[j].alcohol_val)){
		j++;
	}
#endif
	//adc_alcohol[j].adc_val = 0x7fffffff;
	//adc_alcohol[j].alcohol_val = 0x7fffffff;
	ret = filp_close(fp,NULL);
    set_fs(fs);
    return 0;
}
#endif // if read adc from /mnt/vendor/persist/sensors/AdcToAlcohol.txt
static void print_adc_alcohol(void){
	int i = 0;
	PR_DEBUG("atlas ---- print_adc_alcohol begain\n");
	for(i=0 ; adc_alcohol[i].adc_val != 0x7fffffff; i++ ){	
			PR_DEBUG("%d - %d : %d  \n",i,adc_alcohol[i].adc_val,adc_alcohol[i].alcohol_val);
	}
	PR_DEBUG("atlas ---- print_adc_alcohol end\n");
}
static int adc_temp[8] = {0};
static int read_adc_origin_data(void){
	int adc = 0, sum = 0;
	int i = 0,err = 0; 
	struct qpnp_vadc_chip *vadc_dev;
	struct qpnp_vadc_result result;
	
	vadc_dev = qpnp_get_vadc(&(g_alcohol_mp3_data->alcohol_mp3_dev->dev), "test");

	err = qpnp_vadc_read(vadc_dev, VADC_AMUX_THM4, &result); //VADC_AMUX_THM4_PU2 P_MUX4_1_1 Read the GPIO9 VADC channel with 1:1 scaling
	adc = (int) result.physical;
	PR_DEBUG("atlas ---- adc -- = %d \n",adc);
	for(i = 0; i < sizeof(adc_temp)/sizeof(int); i++){
		err = qpnp_vadc_read(vadc_dev,VADC_AMUX_THM4 , &result); //VADC_AMUX_THM4_PU2 P_MUX4_1_1 Read the GPIO9 VADC channel with 1:1 scaling
		adc = (int) result.physical;
		adc_temp[i] =  adc;
		sum += adc_temp[i];
		msleep(10);
		printk("atlas -- adc_temp[%d] = %d \n",i,adc_temp[i]);
	}

	adc = sum >> 3;

	return adc;
}
static int adc_convert_alcohol(unsigned int adc){
	int  i = 0; //err = 0,
	if(adc <= adc_alcohol[0].adc_val)
		return 0;
	for(i = 0; adc < 0x7fffffff; i++ ){
		if(adc < adc_alcohol[i].adc_val)
			break;
	}
	//printk("--atlas -- adc_alcohol[%d].adc_val = %d \n",i,adc_alcohol[i].adc_val);
	print_adc_alcohol(); // print by lx
	adc = adc_alcohol[i-1].alcohol_val + ((adc - adc_alcohol[i-1].adc_val)*(adc_alcohol[i].alcohol_val - adc_alcohol[i-1].alcohol_val))
				/ (adc_alcohol[i].adc_val - adc_alcohol[i-1].adc_val);
	//g_alcohol_mp3_data->alcohoc_value = adc;
	if(adc < 0)
		adc = 0;
	return adc;
}
/* Report alcohol input event */
static void report_alcohol_event(struct alcohol_mp3_data *alcohol_mp3)
{
	//uint16_t alcohol_val = alcohol;
	int adc = 0, alcohol_val = 0;
	adc = read_adc_origin_data();
	alcohol_val = adc_convert_alcohol(adc);
	alcohol_mp3->alcohoc_value = alcohol_val;
	input_report_abs(g_alcohol_mp3_data->alcohol_mp3_input, ABS_MISC, alcohol_val);
	input_sync(g_alcohol_mp3_data->alcohol_mp3_input);

}

static int alcohol_mp3_init_gpio(struct alcohol_mp3_data *alcohol_mp3)
{
	int err = 0;

	AlCOHOL_DBG("enter\n");
	if (gpio_is_valid(alcohol_mp3->pwr_num)) {
		err = gpio_request(alcohol_mp3->pwr_num, "alcohol_mp3-pwr");
		if (err) {
			gpio_free(alcohol_mp3->pwr_num);
			err = gpio_request(alcohol_mp3->pwr_num, "alcohol_mp3-pwr");
			if (err) {
				AlCOHOL_DBG("Could not request pwr gpio.\n");
				return err;
			}
		}
	}
	else {
		AlCOHOL_DBG("not valid pwr gpio\n");
		return -EIO;
	}
	alcohol_mp3_power_on(g_alcohol_mp3_data);

	return err;
}

#if 0
static int alcohol_mp3_free_gpio(struct alcohol_mp3_data *alcohol_mp3)
{
	int err = 0;

	AlCOHOL_DBG("enter\n");

	if (gpio_is_valid(alcohol_mp3->pwr_num)) {
		gpio_free(alcohol_mp3->pwr_num);
	}
	return err;
}

static void alcohol_mp3_reset(struct alcohol_mp3_data *pdata, int ms)
{
	gpio_set_value(pdata->pwr_num, 1);
	mdelay(ms);
	gpio_set_value(pdata->pwr_num, 0);
	mdelay(ms);
	gpio_set_value(pdata->pwr_num, 1);
	mdelay(ms);
}
#endif
static int alcohol_mp3_parse_dts(struct device *dev, struct alcohol_mp3_data *alcohol_mp3)
{
	int err = 0;

	AlCOHOL_DBG("enter\n");
#ifndef POWER_REGULATOR
	//gpio 26
	alcohol_mp3->pwr_num = of_get_named_gpio(dev->of_node, "alcohol_mp3,pwr_gpio", 0);
	//gpio_direction_output(alcohol_mp3->pwr_num, 1);
	//gpio_set_value_cansleep(g_alcohol_mp3_data->pwr_num,1);
#else
	alcohol_mp3->vdd = regulator_get(dev, "vdd");
#endif

#ifdef SUPPORT_ID_NUM
	alcohol_mp3->id_num = of_get_named_gpio(dev->of_node, "alcohol_mp3,id_gpio", 0);
	alcohol_mp3->s_pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR(alcohol_mp3->s_pinctrl))
	{
		AlCOHOL_ERR("pinctrl get failed!\n");
		err = -1;
	}
#endif
	return err;
}

static int alcohol_mp3_power_on(struct alcohol_mp3_data *pdata)
{
	int ret = 0;
#ifndef POWER_REGULATOR
	gpio_direction_output(pdata->pwr_num, 1);
#else
	regulator_set_voltage(pdata->vdd, 0, 2800000);
	ret = regulator_enable(pdata->vdd);
	if(ret)
	{
		AlCOHOL_ERR("enable regulato fail\n");
		return ret;
	}
#endif
	alcohoc_stat = 1;
#if defined(CONFIG_ALCOHOL_USE_TIMER)
   init_timer(&tm);
//   do_gettimeofday(&oldtv);
   tm.function= timer_callback;	 
   tm.data	  = (unsigned long)"timer";
   old_jaffs = jiffies;
   tm.expires = jiffies+ALCOHOL_INIT_TIMER_S*HZ; 	   //定时时间 180s
   add_timer(&tm);
#endif

   // power on delay 	
	msleep(20);
	return ret;
}

static int alcohol_mp3_power_off(struct alcohol_mp3_data *pdata)
{
#ifndef POWER_REGULATOR
	gpio_direction_output(pdata->pwr_num, 0);
#else
	regulator_disable(pdata->vdd);
#endif
	alcohoc_stat = 0;

	mdelay(1);
	return 0;
}

static int alcohol_mp3_open(struct inode *inode, struct file *file)
{
	int err = 0;

	AlCOHOL_DBG("enter\n");

	file->private_data = g_alcohol_mp3_data;
	err = alcohol_mp3_power_on(g_alcohol_mp3_data);
#ifdef CONFIG_ALCOHOL_USE_CALIBRATION
	err = read_calibration_data();
	if(err != 0){
		PR_DEBUG("atlas -- read_calibration_data faild err = %d \n",err);
		return err;
	}
#endif
	return 0;
}

static int alcohol_mp3_async_fasync(int fd, struct file *file,int mode)
{
	struct alcohol_mp3_data *alcohol_mp3 = g_alcohol_mp3_data;
	return fasync_helper(fd,file,mode,&alcohol_mp3->async_queue);
}

static int alcohol_mp3_release(struct inode *inode,struct file *file)
{
	struct alcohol_mp3_data *alcohol_mp3 = file->private_data;

	AlCOHOL_DBG("enter\n");
	PR_DEBUG("atlas -- alcohol_mp3_release \n");
	if(NULL == alcohol_mp3)
	{
		return -EIO;
	}
	alcohol_mp3_power_off(alcohol_mp3);
	file->private_data = NULL;
	
	return 0;
}
#if 0 //lx
static void alcohol_mp3_async_report(void)
{
	struct alcohol_mp3_data *alcohol_mp3 = g_alcohol_mp3_data;
	wake_lock_timeout(&alcohol_mp3->alcohol_mp3_lock, msecs_to_jiffies(1000));
	kill_fasync(&alcohol_mp3->async_queue,SIGIO,POLL_IN);
}

static void alcohol_mp3_wake_lock(struct alcohol_mp3_data *pdata,int arg)
{
	if(arg)
	{
		wake_lock(&pdata->alcohol_mp3_lock);
	}else{
		wake_unlock(&pdata->alcohol_mp3_lock);
		wake_lock_timeout(&pdata->alcohol_mp3_lock, msecs_to_jiffies(3000));
	}
}
#endif //lx


#ifdef CONFIG_HY_DRV_ASSIST
static ssize_t alcohol_mp3_ic_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n","mp3");
}
#endif

static ssize_t alcohol_read(struct file* filp,char __user *buf,size_t count,loff_t* f_pos){
    ssize_t err = 0;
    int adc = 0, alcohol_val = 0;
	struct alcohol_mp3_data *alcohol_mp3 = g_alcohol_mp3_data;
	adc = read_adc_origin_data();
	alcohol_val = adc_convert_alcohol(adc);
	alcohol_mp3->alcohoc_value = alcohol_val;

    if(count < sizeof(alcohol_mp3->alcohoc_value)){
        goto out;
    }
    if(copy_to_user(buf,&(alcohol_mp3->alcohoc_value),sizeof(alcohol_mp3->alcohoc_value))){
        err = -EFAULT;
        goto out;
    }
    err = sizeof(alcohol_mp3->alcohoc_value);

out:
    return err;
}

static ssize_t alcohol_write(struct file* filp,const char __user *buf,size_t count,loff_t* f_pos){
    ssize_t err = 0;
	int adc = 0;// alcohol_val = 0;
	struct alcohol_mp3_data *alcohol_mp3 = g_alcohol_mp3_data;
	adc = read_adc_origin_data();
	//alcohol_val = adc_convert_alcohol(adc);
	//alcohol_mp3->alcohoc_value = alcohol_val;

    if(count != sizeof(alcohol_mp3->alcohoc_value)){
        goto out;
    }

    if(copy_from_user(&(alcohol_mp3->alcohoc_value),buf,count)){
        err = -EFAULT;
        goto out;

    }

    err = sizeof(alcohol_mp3->alcohoc_value);
out:

    return err;
}

static const struct file_operations alcohol_mp3_fops = {
	.owner	 = THIS_MODULE,
	.open	 = alcohol_mp3_open,
	.read = alcohol_read,
	.write = alcohol_write,
//	.unlocked_ioctl = alcohol_mp3_ioctl,
	.release = alcohol_mp3_release,
	.fasync  = alcohol_mp3_async_fasync,
#ifdef CONFIG_COMPAT
//	.compat_ioctl = alcohol_mp3_ioctl,
#endif
};

static struct miscdevice st_alcohol_mp3_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &alcohol_mp3_fops,
};

#if 0 //lx
static int alcohol_mp3_fb_notifier_callback(struct notifier_block* self,
										unsigned long event, void* data)
{
	struct fb_event* evdata = data;
	unsigned int blank;
	int retval = 0;

	if (event != FB_EVENT_BLANK /* FB_EARLY_EVENT_BLANK */) {
		return 0;
	}

	blank = *(int*)evdata->data;

	switch (blank) {
		case FB_BLANK_UNBLANK:
			mutex_lock(&g_alcohol_mp3_data->buf_lock);
			screen_status = 1;
			if (isInKeyMode == 0)
				alcohol_mp3_async_report();
			mutex_unlock(&g_alcohol_mp3_data->buf_lock);
			break;

		case FB_BLANK_POWERDOWN:
			mutex_lock(&g_alcohol_mp3_data->buf_lock);
			screen_status = 0;
			if (isInKeyMode == 0)
				alcohol_mp3_async_report();
			mutex_unlock(&g_alcohol_mp3_data->buf_lock);
			break;

		default:
			break;
	}

	return retval;
}
#endif //lx

#ifdef SUPPORT_ID_NUM
static int alcohol_mp3_support_id(struct alcohol_mp3_data *alcohol_mp3)
{
	int err = 0;
	alcohol_mp3->s_id_high = pinctrl_lookup_state(alcohol_mp3->s_pinctrl, "alcohol_mp3_id_pin");
	if (IS_ERR(alcohol_mp3->s_id_high)){
		AlCOHOL_ERR("look up state err\n");
		return -1;
	}
	pinctrl_select_state(alcohol_mp3->s_pinctrl, alcohol_mp3->s_id_high);
	if (gpio_is_valid(alcohol_mp3->id_num)) {
		err = gpio_request(alcohol_mp3->id_num, "alcohol_mp3-id");
		if (err) {
			gpio_free(alcohol_mp3->id_num);
			err = gpio_request(alcohol_mp3->id_num, "alcohol_mp3-id");
			if (err) {
				AlCOHOL_ERR("Could not request id gpio.\n");
				return err;
			}
		}
		gpio_direction_input(alcohol_mp3->id_num);
	}
	else {
		AlCOHOL_ERR(KERN_ERR "not valid irq gpio\n");
		return -EIO;
	}
	err = gpio_get_value(alcohol_mp3->id_num);
	gpio_free(alcohol_mp3->id_num);
	return err;
}
#endif

static ssize_t alcohol_val_show(struct device* dev,struct device_attribute* attr,char* buf){
    ssize_t ret = 0;
#ifdef CONFIG_ALCOHOL_USE_CALIBRATION
	int err = 0;
	err = read_calibration_data();
	print_adc_alcohol();
	if(err != 0){
		PR_DEBUG("atlas -- read_calibration_data faild err = %d \n",err);
		//return -100000;
	}
#endif
#if defined(CONFIG_ALCOHOL_USE_TIMER)
	if(alcohoc_stat !=2 ){
		printk("atlas tm %ld \n",(jiffies -old_jaffs )/HZ);
		g_alcohol_mp3_data->alcohoc_value = ((jiffies -old_jaffs )/HZ - ALCOHOL_INIT_TIMER_S);
	}else
#endif
		report_alcohol_event(g_alcohol_mp3_data);
	PR_DEBUG("atlas -- alcohol_val = %d \n",g_alcohol_mp3_data->alcohoc_value);
    ret = sprintf(buf, "%d", g_alcohol_mp3_data->alcohoc_value);

    //ret = strlen(buf) + 1;

    return ret;
}

static ssize_t alcohol_val_store(struct device* dev,
				struct device_attribute* attr,const char* buf,size_t count){
    int value = 0, ret = 0;

	if (!g_alcohol_mp3_data)
		return -EPERM;

	ret = kstrtoint(buf, 10, &value);
	if (!ret)
		return ret;

	//if (value < MIN_BCL_POLL_INTERVAL)
	//	return -EINVAL;

	g_alcohol_mp3_data->alcohoc_value = value;

	return count;   
}
static ssize_t adc_val_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;
	int adc_val = 0;
#ifdef CONFIG_ALCOHOL_USE_CALIBRATION
	int err = 0;
	err = read_calibration_data();
	print_adc_alcohol();
	PR_DEBUG("atlas -- err = %d \n",err);
#endif
 #if defined(CONFIG_ALCOHOL_USE_TIMER)
	if(alcohoc_stat !=2 ){
		printk("atlas tm %ld \n",(jiffies -old_jaffs )/HZ);
		adc_val = (jiffies -old_jaffs )/HZ -ALCOHOL_INIT_TIMER_S;
	}else
#endif
		adc_val = read_adc_origin_data();

	PR_DEBUG("atlas -- adc_val = %d \n",adc_val);
    ret = sprintf(buf, "%d", adc_val);

    //ret = strlen(buf) + 1;

    return ret;
}

static ssize_t adc_val_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{

	unsigned int databuf[2];

	if(2 == sscanf(buf, "%x %x", &databuf[0], &databuf[1]))
	{
		if(databuf[0] >= 8)
			return -EPERM;
		adc_temp[databuf[0]] = databuf[1];
	}

	return len;
}

static DEVICE_ATTR(adc_val,0664,adc_val_show,adc_val_store);
static DEVICE_ATTR(alcohol_val,0664,alcohol_val_show,alcohol_val_store);
static ssize_t alcohol_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;
	int stat = gpio_get_value_cansleep(g_alcohol_mp3_data->pwr_num);
	int i = 0;
	for(i=0 ; adc_alcohol[i].adc_val != 0x7fffffff; i++ ){	
			printk("--%d - %d : %d  \n",i,adc_alcohol[i].adc_val,adc_alcohol[i].alcohol_val);
	}

    ret = sprintf(buf, "%d", stat);
    //ret = strlen(buf) + 1;

    return ret;
}

static ssize_t alcohol_enable_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{

    int  ret = 0;
	int  value = 0;
	if (!g_alcohol_mp3_data)
		return -EPERM;
	ret = kstrtoint(buf, 10, &value);
	if (ret < 0)
		return ret;
	//PR_DEBUG("atlas ---- buf = %s value = %d ret=%d pwr_num=%d\n",buf,value,ret,g_alcohol_mp3_data->pwr_num);
	//if (value < MIN_BCL_POLL_INTERVAL)
	//	return -EINVAL;
	if(value == 0){
		alcohol_mp3_power_off(g_alcohol_mp3_data);
	}else
		alcohol_mp3_power_on(g_alcohol_mp3_data);

	return len;
}

static DEVICE_ATTR(enable,0664,alcohol_enable_show,alcohol_enable_store);

#ifdef CONFIG_INPUT_EVENT_POLL
static ssize_t poll_interval_show(struct device* dev,struct device_attribute* attr,char* buf){
    ssize_t ret = 0;
    unsigned int poll_interval;

    poll_interval = g_alcohol_mp3_data->input_polled->poll_interval;

    sprintf(buf, "poll_interval: %d ms\n", poll_interval);

    ret = strlen(buf) + 1;

    return ret;

}

static ssize_t poll_interval_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
{
	int value = 0, ret = 0;

	if (!g_alcohol_mp3_data->input_polled)
		return -EPERM;

	ret = kstrtoint(buf, 10, &value);
	if (ret < 0)
		return ret;

	if (value < ALCOHOL_POLL_MIN)
		return -EINVAL;

	g_alcohol_mp3_data->input_polled->poll_interval = value;

	return count;
}

static DEVICE_ATTR(poll_interval,0664,poll_interval_show,poll_interval_store);
#endif
static int mp3_add_sysfile(void){
	struct device* device_temp = NULL;
	int err = 0;
	   alcohol_class = class_create(THIS_MODULE,"alcohol_cls");
	   if(IS_ERR(alcohol_class)){
		   err = PTR_ERR(alcohol_class);
		   AlCOHOL_ERR("Failed to create welcome class.\n");
		   return err;
	   }

	   device_temp = device_create(alcohol_class,NULL,0,NULL,"alcohol");
	   if(IS_ERR(device_temp)){
		   err = PTR_ERR(device_temp);
		   AlCOHOL_ERR("Failed to create welcome device.\n");
		   goto destroy_class;
	   }

	   err =device_create_file(device_temp,&dev_attr_adc_val);
	   if(err < 0){
		   AlCOHOL_ERR("Failed to create attribute adc_val. \n");
		   goto destroy_device;
	   }
	   err =device_create_file(device_temp,&dev_attr_alcohol_val);
	   if(err < 0){
		   AlCOHOL_ERR("Failed to create attribute alcohol_val. \n");
		   goto destroy_device;
	   }
	   err =device_create_file(device_temp,&dev_attr_enable);
	   if(err < 0){
		   AlCOHOL_ERR("Failed to create attribute enable. \n");
		   goto destroy_device;
	   }
	 #ifdef CONFIG_INPUT_EVENT_POLL
	   err =device_create_file(device_temp,&dev_attr_poll_interval);
	   if(err < 0){
		   AlCOHOL_ERR("Failed to create attribute poll_interval. \n");
		   goto destroy_device;
	   }
	#endif
	   return 0;

destroy_device:
    device_destroy(alcohol_class,0);
destroy_class:
    class_destroy(alcohol_class);

	return err;

}
#ifdef CONFIG_INPUT_EVENT_POLL
static void mp3_poll_open(struct input_polled_dev *ipoll_dev)
{
	struct alcohol_mp3_data *alcohol_mp3 = ipoll_dev->private;

	alcohol_mp3_power_on(alcohol_mp3);
}

static void mp3_poll_close(struct input_polled_dev *ipoll_dev)
{
	struct alcohol_mp3_data *alcohol_mp3 = ipoll_dev->private;

	alcohol_mp3_power_on(alcohol_mp3);
}

static void mp3_poll(struct input_polled_dev *dev)
{
	PR_DEBUG("atlas ----mp3_poll \n");
	report_alcohol_event(dev->private);
}

static int mp3_register_polled_device(struct alcohol_mp3_data *alcohol_mp3)
{
	struct input_polled_dev *ipoll_dev;
	int error;

	ipoll_dev = input_allocate_polled_device();
	if (!ipoll_dev)
		return -ENOMEM;

	ipoll_dev->private = alcohol_mp3;
	ipoll_dev->open = mp3_poll_open;
	ipoll_dev->close = mp3_poll_close;
	ipoll_dev->poll = mp3_poll;
	ipoll_dev->poll_interval = ALCOHOL_POLL_INTERVAL;
	ipoll_dev->poll_interval_min = ALCOHOL_POLL_MIN;
	ipoll_dev->poll_interval_max = ALCOHOL_POLL_MAX;

	ipoll_dev->input->name=INPUT_DEVICE_NAME;
	//ipoll_dev->input->phys = INPUT_DEVICE_NAME "/input0";
	set_bit(EV_ABS, ipoll_dev->input->evbit);
	input_set_abs_params(ipoll_dev->input, ABS_MISC,
			ALCOHOL_MIN_MEASURE_VAL, ALCOHOL_MAX_MEASURE_VAL, 0, 0);
	
	error = input_register_polled_device(ipoll_dev);
	if (error) {
		input_free_polled_device(ipoll_dev);
		return error;
	}

	alcohol_mp3->input_polled = ipoll_dev;
	alcohol_mp3->alcohol_mp3_input = ipoll_dev->input;

	return 0;
}
#endif
static int alcohol_mp3_probe(struct platform_device *pdev)
{
	struct alcohol_mp3_data *alcohol_mp3dev= NULL;
	int status = -ENODEV;
	alcohol_mp3dev = kzalloc(sizeof(struct alcohol_mp3_data),GFP_KERNEL);
	alcohol_mp3dev->alcohol_mp3_dev = pdev;

	AlCOHOL_DBG("enter\n");
	status=alcohol_mp3_parse_dts(&alcohol_mp3dev->alcohol_mp3_dev->dev, alcohol_mp3dev);
	if(status){
		AlCOHOL_ERR("alcohol_mp3 parse err %d\n",status);
		return -1;
	}

#ifdef SUPPORT_ID_NUM
	status = alcohol_mp3_support_id(alcohol_mp3dev);
	if (status != 1) {  // id pin is high , alcohol_mp3
		AlCOHOL_ERR("alcohol_mp3 support id error %d\n",status);
		return -1;
	}
#endif

	status = misc_register(&st_alcohol_mp3_dev);
	if (status) {
		AlCOHOL_ERR("alcohol_mp3 misc register err%d\n",status);
		return -1;
	}
	alcohol_mp3dev->miscdev = &st_alcohol_mp3_dev;
	mutex_init(&alcohol_mp3dev->buf_lock);
	wake_lock_init(&alcohol_mp3dev->alcohol_mp3_lock, WAKE_LOCK_SUSPEND, "alcohol_mp3 wakelock");
#ifndef CONFIG_INPUT_EVENT_POLL
	alcohol_mp3dev->alcohol_mp3_input = input_allocate_device();
	if(!alcohol_mp3dev->alcohol_mp3_input){
		AlCOHOL_ERR("crate alcohol_mp3_input faile!\n");
		goto unregister_dev;
	}

	alcohol_mp3dev->alcohol_mp3_input->name=INPUT_DEVICE_NAME;
	
	set_bit(EV_ABS, alcohol_mp3dev->alcohol_mp3_input->evbit);
	input_set_abs_params(alcohol_mp3dev->alcohol_mp3_input, ABS_MISC,
			ALCOHOL_MIN_MEASURE_VAL, ALCOHOL_MAX_MEASURE_VAL, 0, 0);
	if(input_register_device(alcohol_mp3dev->alcohol_mp3_input))
	{
	  input_free_device(alcohol_mp3dev->alcohol_mp3_input);
	  alcohol_mp3dev->alcohol_mp3_input = NULL;
	  goto unregister_dev;
	}
#else
	status = mp3_register_polled_device(alcohol_mp3dev);
		if (status)
			goto unregister_dev;
#endif


	//alcohol_mp3dev->notifier.notifier_call = alcohol_mp3_fb_notifier_callback;
    // fb_register_client(&alcohol_mp3dev->notifier);

	g_alcohol_mp3_data = alcohol_mp3dev;
#ifdef CONFIG_HY_DRV_ASSIST
	cnfc_assist_register_attr("ic",&alcohol_mp3_ic_show,NULL);
#endif
	mp3_add_sysfile();

	alcohol_mp3_init_gpio(g_alcohol_mp3_data);

	AlCOHOL_DBG("end\n");
	return 0;

unregister_dev:
	misc_deregister(&st_alcohol_mp3_dev);
	kfree(alcohol_mp3dev);
	return  status;
}
#ifdef CONFIG_PM
static int alcohol_mp3_suspend(struct device *dev)
{
	int ret = 0;
	AlCOHOL_DBG("enter\n");
	//ret = alcohol_mp3_power_off(g_alcohol_mp3_data);
	PR_DEBUG("atlas --alcohol_mp3_suspend /n");
	return ret;
}

static int alcohol_mp3_resume(struct device *dev)
{
	int ret = 0;
	AlCOHOL_DBG("enter\n");
	//ret = alcohol_mp3_power_on(g_alcohol_mp3_data);
	PR_DEBUG("atlas --alcohol_mp3_resume /n");
	return ret;
}
#endif

static UNIVERSAL_DEV_PM_OPS(alcohol_mp3_pm, alcohol_mp3_suspend, alcohol_mp3_resume, NULL);


static const struct of_device_id alcohol_mp3_of_match[] = {
	{ .compatible = "alcohol_mp3,mp3", },
	{},
};

static const struct platform_device_id alcohol_mp3_id[] = {
	{"alcohol_mp3", 0},
	{}
};

static struct platform_driver alcohol_mp3_driver = {
	.driver = {
		.name = "alcohol_mp3",
		.of_match_table = alcohol_mp3_of_match,
		.pm	= &alcohol_mp3_pm,
	},
	.id_table = alcohol_mp3_id,
	.probe = alcohol_mp3_probe,
};

static int __init alcohol_mp3_init(void)
{
	PR_DEBUG(" -- %s start!\n",__func__);
	return platform_driver_register(&alcohol_mp3_driver);
}

static void __exit alcohol_mp3_exit(void)
{
	platform_driver_unregister(&alcohol_mp3_driver);

}

module_init(alcohol_mp3_init);

module_exit(alcohol_mp3_exit);



MODULE_ALIAS("alcohol_mp3");
MODULE_DESCRIPTION("mp3 alcohol Sensor Driver");
MODULE_AUTHOR("atlas <atlas_li@126.com>");
MODULE_LICENSE("GPL");
