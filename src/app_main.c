/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *-pipe -fno-strict-aliasing -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -std=gnu99 -ffunction-sections -fdata-sections -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wbad-function-cast -Wwrite-strings -Wsign-compare -Waggregate-return  -Wmissing-declarations -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Wlong-long -Wunreachable-code -Wcast-align --param max-inline-insns-single=500
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <hsf.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "components/user/include/user_init.h"

EXTERNC const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE];

int g_module_id= HFM_TYPE_LPT230;

const int hf_gpio_fid_to_pid_map_table[HFM_MAX_FUNC_CODE]=
{
    HFM_NOPIN,      //HFGPIO_F_JTAG_TCK
    HFM_NOPIN,//HFGPIO_F_JTAG_TDO
    HFM_NOPIN,//HFGPIO_F_JTAG_TDI
    HFM_NOPIN,//HFGPIO_F_JTAG_TMS
    HFM_NOPIN,//HFGPIO_F_USBDP
    HFM_NOPIN,//HFGPIO_F_USBDM
    LPx30_GPIO2,//HFGPIO_F_UART0_TX
    HFM_NOPIN,//HFGPIO_F_UART0_RTS
    LPx30_GPIO1,//HFGPIO_F_UART0_RX
    HFM_NOPIN,//HFGPIO_F_UART0_CTS

    HFM_NOPIN,//HFGPIO_F_SPI_MISO
    HFM_NOPIN,//HFGPIO_F_SPI_CLK
    HFM_NOPIN,//HFGPIO_F_SPI_CS
    HFM_NOPIN,//HFGPIO_F_SPI_MOSI

    HFM_NOPIN,//HFGPIO_F_UART1_TX,
    HFM_NOPIN,//HFGPIO_F_UART1_RTS,
    HFM_NOPIN,//HFGPIO_F_UART1_RX,
    HFM_NOPIN,//HFGPIO_F_UART1_CTS,

    HFM_NOPIN,//HFGPIO_F_NLINK
    HFM_NOPIN,//HFGPIO_F_NREADY
    HFM_NOPIN,//HFGPIO_F_NRELOAD
    HFM_NOPIN,//HFGPIO_F_SLEEP_RQ
    HFM_NOPIN,//HFGPIO_F_SLEEP_ON

    HFM_NOPIN,//HFGPIO_F_WPS
    HFM_NOPIN,//HFGPIO_F_RESERVE1
    HFM_NOPIN,//HFGPIO_F_RESERVE2
    HFM_NOPIN,//HFGPIO_F_RESERVE3
    HFM_NOPIN,//HFGPIO_F_RESERVE4
    HFM_NOPIN,//HFGPIO_F_RESERVE5

    LPx30_GPIO25,//HFGPIO_F_USER_DEFINE     key
    LPx30_GPIO5,//HFGPIO_F_USER_DEFINE+1   led
    LPx30_GPIO22,//HFGPIO_F_USER_DEFINE+2   sw1
    LPx30_GPIO24,//HFGPIO_F_USER_DEFINE+3   sw2
    LPx30_GPIO8,//HFGPIO_F_USER_DEFINE+4   sw3
    LPx30_GPIO23,//HFGPIO_F_USER_DEFINE+5   sw4
    LPx30_GPADC0,//HFGPIO_F_USER_DEFINE+6   sw5
    LPx30_GPIO3,//HFGPIO_F_USER_DEFINE+7   sw6
    LPx30_GPIO7,//HFGPIO_F_USER_DEFINE+8   POWER
    LPx30_GPIO12,//HFGPIO_F_USER_DEFINE+9   set VOLTAGE ELECTRIC CURRENT
    LPx30_GPIO4,//HFGPIO_F_USER_DEFINE+10   VOLTAGE ELECTRIC CURRENT

};

//For type written and read
static int dev_param_read(char *wechat_type) {
    unsigned char type[21];
    hffile_userbin_read(0, (char*) type, sizeof(type));

    unsigned char crc = 0;
    if (type[0] == 0x5A) {
        int i;
        char *p = (char *) (type + 2);
        if (strlen(p) > 18)
            return -1;

        for (i = 0; i < strlen(p); i++)
            crc += (unsigned char) *(p + i);

        if (type[1] == crc) {
            strcpy(wechat_type, p);
            return HF_SUCCESS;
        }
    }

    return -1;
}

static int dev_param_write(char *wechat_type) {
    char type[21];
    unsigned char crc = 0;

    int i;
    for (i = 0; i < strlen(wechat_type); i++)
        crc += (unsigned char) wechat_type[i];

    type[0] = 0x5A;
    type[1] = crc;
    strcpy(type + 2, wechat_type);

    hffile_userbin_write(0, (char*) type, sizeof(type));
    return HF_SUCCESS;
}

static int hf_cmd_get_version(pat_session_t s, int argc, char *argv[],
        char *rsp, int len) {
    if (argc == 0) {
        sprintf(rsp, "=%s(%s %s)", "Airkiss 2", __DATE__, __TIME__);

        return 0;
    }
    return -3;
}

static int hf_atcmd_set_type(pat_session_t s, int argc, char *argv[], char *rsp,
        int len) {
    if (argc == 0) {
        char type[21];
        if (dev_param_read(type) == HF_SUCCESS)
            sprintf(rsp, "=%s", type);
        else
            sprintf(rsp, "=%s", "");
        return 0;
    } else if (argc == 1) {
        if (strlen(argv[0]) < 18) {
            if (dev_param_write(argv[0]) == HF_SUCCESS)
                return 0;
            else
                return -5;
        }
        return -4;
    }
    return -3;
}


static int atcmd_hass_mqtt(pat_session_t s,int argc,char *argv[],char *rsp,int len){
    return user_hass_mqtt_cmd_cb(argv,argc);
}

static int atcmd_power_cf(pat_session_t s,int argc,char *argv[],char *rsp,int len){
    if (argc != 1)
        return -1;

    set_power_multiplier(atoi(argv[0]));
    return 0;
}

const hfat_cmd_t user_define_at_cmds_table[] = {
        {"HASSMQTT",atcmd_hass_mqtt,"   AT+HASSMQTT=address,port,username,password,powermeterMode,discoveryPrefix\r\n",NULL},
        {"POWERCF",atcmd_power_cf,"   AT+POWERCF=powerMultiplie\r\n",NULL},
        { "TYPE", hf_atcmd_set_type, "   AT+TYPE: airkiss device type.\r\n",
                NULL }, //Modify TYPE
        { "APPVER", hf_cmd_get_version, "   AT+APPVER: get version\r\n", NULL },
        { NULL, NULL, NULL, NULL } };

/**
 * wifi 回调函数
 */
static int sys_event_callback(uint32_t event_id, void * param) {
    switch (event_id) {
    case HFE_WIFI_STA_CONNECTED:
        u_printf("[System]:wifi sta connected!!\n");
        break;
    case HFE_WIFI_STA_DISCONNECTED:
        u_printf("[System]:wifi sta disconnected!!\n");
        break;
    case HFE_CONFIG_RELOAD:
        u_printf("[System]:reload!\n");
        break;
    case HFE_DHCP_OK: {
        uint32_t *p_ip;
        p_ip = (uint32_t*) param;
        u_printf("[System]:dhcp ok %s!\n", inet_ntoa(*p_ip));
        user_ip_set(inet_ntoa(*p_ip));
    }
        break;
    case HFE_SMTLK_OK: {
        u_printf("[System]:smtlk ok!\n");
        char *pwd = (char*) param;
        if (pwd == NULL)
            u_printf("[System]:key is null!\n");
        else {
            int i;
            for (i = 0; i < (int) strlen(pwd); i++) {
                if (pwd[i] == 0x1b)
                    break;
            }
            for (i++; i < (int) strlen(pwd); i++)
                u_printf("[System]:user info 0x%02x-[%c]\n", pwd[i], pwd[i]);
        }
        msleep(100);
        return 1;
    }
        break;

    case HFE_WPS_OK: {
        u_printf("[System]:wps ok, key: %s!\n", (char* )param);
        //return 1;
    }
        break;
    }
    return 0;
}

//// socketa 回调函数
//static int USER_FUNC socketa_recv_callback(uint32_t event,char *data,uint32_t len,uint32_t buf_len)
//{
//    if(event==HFNET_SOCKETA_DATA_READY)
//        HF_Debug(DEBUG_LEVEL_LOW,"socketa recv %d bytes %d\n",len,buf_len);
//    else if(event==HFNET_SOCKETA_CONNECTED)
//        u_printf("socket a connected!\n");
//    else if(event==HFNET_SOCKETA_DISCONNECTED)
//        u_printf("socket a disconnected!\n");
//
//    return len;
//}
//// socketb 回调函数
//static int USER_FUNC socketb_recv_callback(uint32_t event,char *data,uint32_t len,uint32_t buf_len)
//{
//  if(event==HFNET_SOCKETB_DATA_READY)
//      HF_Debug(DEBUG_LEVEL_LOW,"socketb recv %d bytes %d\n",len,buf_len);
//  else if(event==HFNET_SOCKETB_CONNECTED)
//      u_printf("socket b connected!\n");
//  else if(event==HFNET_SOCKETB_DISCONNECTED)
//      u_printf("socket b disconnected!\n");
//
//  return len;
//}

static int USER_FUNC uart_recv_callback(uint32_t event, char *data,
        uint32_t len, uint32_t buf_len) {
    HF_Debug(DEBUG_LEVEL_LOW, "[%d]uart recv %d bytes data %d\n", event, len,
            buf_len);
    if (hfsys_get_run_mode() == HFSYS_STATE_RUN_CMD)
        return len;

    return len;
}

static void show_reset_reason(void) {
    uint32_t reset_reason = 0;
    reset_reason = hfsys_get_reset_reason();

#if 1
    u_printf("reset_reasion:%08x\n", reset_reason);
#else   
    if(reset_reason&HFSYS_RESET_REASON_ERESET)
    {
        u_printf("ERESET\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_IRESET0)
    {
        u_printf("IRESET0\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_IRESET1)
    {
        u_printf("IRESET1\n");
    }
    if(reset_reason==HFSYS_RESET_REASON_NORMAL)
    {
        u_printf("RESET NORMAL\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_WPS)
    {
        u_printf("RESET FOR WPS\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_START)
    {
        u_printf("RESET FOR SMARTLINK START\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_SMARTLINK_OK)
    {
        u_printf("RESET FOR SMARTLINK OK\n");
    }
    if(reset_reason&HFSYS_RESET_REASON_WPS_OK)
    {
        u_printf("RESET FOR WPS OK\n");
    }
#endif

    return;
}

void app_init(void) {
    u_printf("app_init\n");
}

int USER_FUNC app_main(void) {

    HF_Debug(DEBUG_LEVEL, "sdk version(%s),the app_main start time is %s %s\n",
            hfsys_get_sdk_version(), __DATE__, __TIME__);
    // 检查fid表定义是否正常
    if (hfgpio_fmap_check(g_module_id) != 0) {
        while (1) {
            HF_Debug(DEBUG_ERROR, "gpio map file error\n");
            msleep(1000);
        }
    }


    // 设置wifi状态回调函数
    hfsys_register_system_event(sys_event_callback);
    // 显示重启原因
    show_reset_reason();

    // 检查是否在 smartlink 或者 smart aplink 配网中
    while (hfsmtlk_is_start()) {
        // 智能配网快闪led
        user_led_blink();
        msleep(100);
    }

    // 启动UART0（由AT+UART设置），并注册回调函数，当系统发生下列情况时调用,低优先级优先权,不等于 HF_SUCCESS 就启动失败
    if (hfnet_start_uart(HFTHREAD_PRIORITIES_LOW,
            (hfnet_callback_t) uart_recv_callback) != HF_SUCCESS) {
        HF_Debug(DEBUG_WARN, "start uart fail!\n");
    }

    // 判断wifi驱动是否初始化成功
    while (!hfnet_wifi_is_active()) {
        // 检查wifi是否初始化成功慢闪led
        user_led_blink();
        msleep(500);
    }

    user_relay_aLL_set(RELAY_STATUS_OPEN);
    user_led_on();
    user_init();

    //See Wi-Fi Config tools APP for detailed usage of this thread
    // 启动一个UDP套接字，用于在CMD上扫描和处理局域网。
    if (hfnet_start_assis(ASSIS_PORT) != HF_SUCCESS) {
        HF_Debug(DEBUG_WARN, "start assis fail\n");
    }

//    //AT+NETP socket
//    // 启动SoCKETA（由AT+NETP设置），并注册回调函数，当系统发生下列情况时将调用
//    if(hfnet_start_socketa(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)socketa_recv_callback)!=HF_SUCCESS)
//    {
//      HF_Debug(DEBUG_WARN,"start socketa fail\n");
//    }
//
//    //AT+SOCKB socket
//    // 启动SoCKETB（由AT+SOCKB设置），并注册回调函数，当系统发生下列情况时将调用
//    if(hfnet_start_socketb(HFTHREAD_PRIORITIES_LOW,(hfnet_callback_t)socketb_recv_callback)!=HF_SUCCESS)
//    {
//      HF_Debug(DEBUG_WARN,"start socketb fail\n");
//    }
//
    //Web Server
    if (hfnet_start_httpd(HFTHREAD_PRIORITIES_MID) != HF_SUCCESS) {
        HF_Debug(DEBUG_WARN, "start httpd fail\n");
    }

    //For Wechat airkiss 2.0 device find, can be commened if not use.
    char wechat_type[21] = { 0 };
    char wechat_id[21] = { 0 };
    if (dev_param_read(wechat_type) == HF_SUCCESS) {
        hfnet_get_mac_address(wechat_id);
        if (hfnet_start_airlink(wechat_type, wechat_id) != HF_SUCCESS)
            HF_Debug(DEBUG_WARN, "start airlink fail!\n");
    }

    return 1;
}

// 关闭 WPS
int hfwifi_wps_main(void) {
    return 0;
}
// 关闭 sockea socketb 相关 at 命令
void hfnet_register_socketa_atcmd(void) {
}
void hfnet_register_socketb_atcmd(void) {
}
