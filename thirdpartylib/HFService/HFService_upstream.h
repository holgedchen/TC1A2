
#ifndef _HFSERVICE_UPSTREAM_H_
#define _HFSERVICE_UPSTREAM_H_

/********************************************************************************
*	Description	:  ���ݰ�����
*	Name		: uart_proto_legal_check
*	Input		: data:���ݰ���ʼ��ַ
				  proto_len:���ݰ�����
*	Returns		: none
 ********************************************************************************/
void uart_data_process(unsigned char *data,int proto_len);

/********************************************************************************
*	Description	: UART ���ݽ��գ�Buf ��������״̬����
*	Name		: hfservice_uart_data_recv
*	Input		: data:���ݰ���ʼ��ַ
				  len:���ݰ�����
*	Returns		: none
 ********************************************************************************/
void hfservice_uart_data_recv(unsigned char *data, int len);

/********************************************************************************
*	Description	: ������ʼ����localProcess����
*	Name		: HFService_init
*	Input		: none
*	Returns		: HFSERV_STATE_FAILE or HFSERV_STATE_OK
 ********************************************************************************/
int HFService_init(void);

//void wifi_uart_send(unsigned char* data,int len);


#endif/*_HFSERVICE_UPSTREAM_H_*/

