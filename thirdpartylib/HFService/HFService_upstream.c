#include <hsf.h>		
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "HFService_config.h"
#include "HFService_upstream.h"
#include "HFService_localprocess.h"
#include "HFService_helper.h"


// TODO: UARTЭ��ͷ��ʶ
// UART_DATA_LENGTH_MIN�������sizeof(uart_proto_head)
// UART_DATA_LENGTH_MIN����Ϊ��ͷ�ĳ���
#define UART_DATA_LENGTH_MIN			4
#define UART_DATA_LENGTH_MAX			100
unsigned char uart_proto_head[]={0xAA};
#define HFSERV_CMD_SMTLK_OK             1
#define HFSERV_CMD_SMTLK_FAIL           0

// uart���ݽ���Buffer
unsigned char *hfserv_uart_recv_data = NULL;
// uart Buffer�����ݳ���
int hfserv_uart_recv_len=0;
// �ϴν���uart ���ݵ�ʱ��
int hfserv_uart_last_recv;			// �ϴν���ʱ��
// �豸���ڳ�ʼ����ʶ
int HFService_device_inited=0;
// �豸�����ϱ��ɹ���ʶ
int HFService_device_upload=0;

//���洮���ϱ����ݣ����ڱ��汾��״̬
unsigned char *hfserv_up_copy_data = NULL;
int hfserv_up_copy_len=0;
unsigned char product_test_hf = 0;
int previous_len = 0;


/********************************************************************************
*	Description	:  ʶ���ͷ�����ذ�ͷλ��
*	Name		: uart_proto_find_head
*	Input		: data
                              len     
*	Returns		: None
 ********************************************************************************/
static int uart_proto_find_head(unsigned char *data,int len)
{
	u_printf("*********************uart_proto_find_head**************************\r\n");
	int pos=0;
	int head_len=sizeof(uart_proto_head);
	
	if (head_len==0)
		return 0;
	while (pos+head_len<len)
	{
		// TODO: ��ͷʶ��
		if (memcmp(data+pos,uart_proto_head,head_len)==0)
		{
		    u_printf("find head\r\n");
			return pos;
		}
			
		pos++;
	}
	return -1;
}

static int hfwifi_scan_test( PWIFI_SCAN_RESULT_ITEM scan_ret)
{
	if(scan_ret == NULL)
		return 0;
	unsigned char HFTEST[] = "HF-TEST";
	u_printf("%s    \n", scan_ret->ssid);	
	if((memcmp(scan_ret->ssid,HFTEST,7) == 0))
	{
		product_test_hf = 1;
	}
	
	return 0;
}

/********************************************************************************
*	Description	:  ��ȡ���ݳ��ȣ����س���ֵ
*	Name		: uart_proto_get_len
*	Input		: data:uart�յ�������
				  head_pos:��ͷλ��
				  len:�����ܳ���    
*	Returns		: >0 ��Ч���ݰ�����
				  =0 ����û������
				  <0 ��Ч���ݰ�,|ret|Ϊ��Ч���ݳ���
 ********************************************************************************/
static int uart_proto_get_len(unsigned char *data,int head_pos,int len)
{
	// TODO: ��ȡ���ݳ��ȣ����ݳ��Ⱥ����Լ�飬�ظ����ݳ���
	
	return len;
}

/********************************************************************************
*	Description	:   ���ݰ��Ϸ��Լ��,��:CRC check,
*	Name		: uart_proto_legal_check
*	Input		: data:���ݰ���ʼ��ַ
				  proto_len:���ݰ�����
*	Returns		: HFSERV_STATE_OK  or HFSERV_STATE_FAILE
 ********************************************************************************/
	static int uart_proto_legal_check(unsigned char *data,int proto_len)
	{
		// TODO: �Ϸ��Լ��
		return HFSERV_STATE_OK;
	}

/********************************************************************************
*	Description	:  �������
*	Name		: uart_proto_legal_check
*	Input		: data:���ݰ���ʼ��ַ
				  proto_len:���ݰ�����
*	Returns		: HFSERV_UART_CMD_XXXX or HFSERV_UART_DATA_XXX
 ********************************************************************************/
static int uart_cmd_classify(unsigned char *data,int proto_len_data)
{
	if (HFService_device_inited == 0)
		return HFSERV_UART_CMD_INIT;
	switch(data[0])
	{
	case 0x07:
		return HFSERV_UART_CMD_SMTLK;
		break;
	case 0x06:
		return HFSERV_UART_CMD_ERROR;
		break;
	case 0x08:
		return HFSERV_UART_CMD_WORK;
		break;
	case 0x0A:
		return HFSERV_UART_CMD_ACK;
		break;
	case 0x0B:
		return HFSERV_UART_CMD_DACTORY;
		break;
	case 0xAA:
		return HFSERV_UART_CMD_DATA;
		break;
	}
	// TODO:
	//�������
	return HFSERV_UART_CMD_NULL;
}

/********************************************************************************
*	Description	:  ���ݰ�����
*	Name		: uart_proto_legal_check
*	Input		: data:���ݰ���ʼ��ַ
				  proto_len:���ݰ�����
*	Returns		: none
 ********************************************************************************/

void uart_data_process(unsigned char *data,int proto_len)
{

	int uart_cmd=uart_cmd_classify(data,proto_len);
	
	if (uart_cmd==HFSERV_UART_CMD_SMTLK)
	{
	    // TODO: ��ʼSmartLink
		hfsmtlk_start();
	}
	else if (uart_cmd==HFSERV_UART_CMD_RELD)
	{
		// TODO: ��ʼ�ָ��û�Config
		hfsys_reload();
	}
	else if (uart_cmd==HFSERV_UART_CMD_DACTORY)
	{
		// TODO: ����ָ��
		hfwifi_scan(hfwifi_scan_test);
		
	}
	else if (uart_cmd==HFSERV_UART_CMD_RESET)
	{
		// TODO: ��ʼģ�鸴λ
		hfsys_reset();
	}	
	else if (uart_cmd==HFSERV_UART_CMD_INIT)
	{
		// TODO: �豸init����
		
		if(hfserv_down_copy_data != NULL)
		{
			HFService_device_inited = 1;
			HFService_device_upload = 0;
			memcpy((char *)hfserv_down_copy_data, (char *)data, proto_len);
			hfserv_down_copy_len = proto_len;

			hfthread_sem_signal(device_init_ok_sem);
		}
	}
	else if (uart_cmd==HFSERV_UART_CMD_DATA)
	{
		// TODO: ҵ�����ݰ�����
		upstream(data);			// ���ô������ɹ������ɵ�protocol_XXXX.c�ĺ���
		if (outLen<=0)
			return;
		if ((UART_DOWN_NEED_ACK != 0)&&(flag_local_waiting_for_rpl + flag_remote_waiting_for_rpl > 0))
		{
			//char data=HFSERV_UART_ACK_STOP;
			hfthread_sem_signal(uart_ack_sem_stop);
		}
		
		if (flag_local_waiting_for_rpl)
		{
			hfservice_cloud_data_send(HFSERV_RSP_LOCAL_CTRL, dataOut, outLen);
			flag_local_waiting_for_rpl=0;
		}
		else if (flag_remote_waiting_for_rpl)
		{
			hfservice_cloud_data_send(HFSERV_RSP_REMOTE_CTRL, dataOut, outLen);
			flag_remote_waiting_for_rpl=0;
		}
		else
		{
			hfservice_cloud_data_send(HFSERV_UPDATE_DATA, dataOut, outLen);
			HFService_device_upload = 1;
		}
		memcpy((char *)hfserv_up_copy_data, (char *)dataOut, outLen);
		hfserv_up_copy_len = outLen;
	}
	
}

/********************************************************************************
*	Description	: UART���������Լ�飬������һ������
*	Name		: uart_proto_legal_check
*	Input		: data:���ݰ���ʼ��ַ
				  len:���ݰ�����
*	Returns		: ����ʱΪ�ѱ���������ݳ���
 ********************************************************************************/
static int uart_proto_integrateCheck(unsigned char *data,int *len)
{
	int ret=0;
	int proto_head=uart_proto_find_head(data,*len);
	int proto_len;
	if (proto_head!=-1)			// �ҵ���ͷλ��
	{
		proto_len=uart_proto_get_len(data,proto_head,*len);
		if (proto_len>0)			// �յ���������
		{
			if (uart_proto_legal_check(data+proto_head,proto_len)==HFSERV_STATE_OK)
			{
				uart_data_process(data+proto_head,proto_len);
				ret=HFSERV_STATE_OK;
				*len=proto_head+proto_len;
			}
			else
			{
				ret=HFSERV_STATE_FAILE;
				*len=proto_head+proto_len;
			}
		}
		else	 if (proto_len==0)		// ��û��������
		{
			ret=HFSERV_STATE_NONE;
			*len=proto_head;
		}
		else //if (proto_len<0)		// �յ���Ч����
		{
			ret=HFSERV_STATE_FAILE;
			*len=proto_head+(-proto_len);
		}
	}
	else							// û�а�ͷ�����°���ʶ���ȵ�����
	{
		if (*len>sizeof(uart_proto_head))
		{
			ret=HFSERV_STATE_FAILE;
			*len=(*len-sizeof(uart_proto_head));
		}
	}
	return ret;
}

/********************************************************************************
*	Description	: UART ���ݽ��գ�Buf ��������״̬����
*	Name		: hfservice_uart_data_recv
*	Input		: data:���ݰ���ʼ��ַ
				  len:���ݰ�����
*	Returns		: none
 ********************************************************************************/
void hfservice_uart_data_recv(unsigned char *data, int len)
{
	int tmp_len;
	int proto_len,state;

	// ������̫��������ǰ�������
	if ((UART_DATA_BYTE_INTERVAL!=0)&&(hfsys_get_time()-hfserv_uart_last_recv>UART_DATA_BYTE_INTERVAL))
	{
		hfserv_uart_recv_len=0;
	}
	hfserv_uart_last_recv=hfsys_get_time();

	tmp_len=len;
	if (hfserv_uart_recv_len+len>DATA_OUT_MAX_SIZE)
		tmp_len=sizeof(hfserv_uart_recv_data)-hfserv_uart_recv_len;

	// �����ݷ���Buffer
	memcpy(hfserv_uart_recv_data+hfserv_uart_recv_len,data,tmp_len);
	hfserv_uart_recv_len+=tmp_len;

	// ������ݳ��ȴ���UART_DATA_LENGTH_MIN, ��������
	while (hfserv_uart_recv_len>=UART_DATA_LENGTH_MIN)
	{
		proto_len=hfserv_uart_recv_len;
		//uart_data_process(data,len);
		//state=1;
		state=uart_proto_integrateCheck(hfserv_uart_recv_data,&proto_len);
		// return of proto_len is the length of non-used (or processed) data
		if (proto_len!=0)
		{
			memcpy(hfserv_uart_recv_data,hfserv_uart_recv_data+proto_len,hfserv_uart_recv_len-proto_len);
			// ע��memcpy�Ƿ����BUG��
			hfserv_uart_recv_len-=proto_len;
		}
		if (state==HFSERV_STATE_NONE)
		{
			// ���ݳ��ȴ���UART_DATA_LENGTH_MIN��������û��ȫ
			break;
		}
	}
}


/********************************************************************************
*	Description	: ������ʼ����localProcess����
*	Name		: HFService_init
*	Input		: none
*	Returns		: HFSERV_STATE_FAILE or HFSERV_STATE_OK
 ********************************************************************************/
int HFService_init(void)
{
	extern void setHeap(void *heap, int size);

	hfserv_uart_recv_data  = hfmem_malloc(DATA_OUT_MAX_SIZE);
	if(hfserv_uart_recv_data == NULL)
		return HFSERV_STATE_FAILE;
	hfserv_down_copy_data = hfmem_malloc(DATA_OUT_MAX_SIZE);
	if(hfserv_down_copy_data == NULL)
		return HFSERV_STATE_FAILE;

	hfserv_up_copy_data = hfmem_malloc(DATA_OUT_MAX_SIZE);
	if(hfserv_up_copy_data == NULL)
	{
		hfmem_free(hfserv_down_copy_data);
		return HFSERV_STATE_FAILE;
	}

	char *heap = hfmem_malloc(DATA_OUT_MAX_SIZE);
	if(heap == NULL)
	{
		hfmem_free(hfserv_down_copy_data);
		hfmem_free(hfserv_up_copy_data);
		return HFSERV_STATE_FAILE;
	}
	setHeap((void *)heap, DATA_OUT_MAX_SIZE);
	
	if (UART_DOWN_NEED_ACK==1)
	{
		hfthread_sem_new (&uart_ack_sem_start, 0);
		hfthread_sem_new (&uart_ack_sem_stop, 0);
		hfthread_create(process_uart_ack,"process_uart_ack", 256, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL);
	}
	hfthread_sem_new (&device_init_ok_sem, 0);
	hfthread_create(process_device_init,"process_device_init", 512, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL);
    
	if (DEVICE_DONT_NEED_MCU==1)
	{
		hfthread_sem_new (&local_mcu_sem, 0);
		hfthread_create(process_local_mcu,"process_local_mcu", 256, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL);
	}
	return HFSERV_STATE_OK;
}

