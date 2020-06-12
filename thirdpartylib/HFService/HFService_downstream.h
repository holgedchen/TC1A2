
#ifndef _HFSERVICE_DOWNSTREAM_H_
#define _HFSERVICE_DOWNSTREAM_H_

/********************************************************************************
*	Description	: �յ��ƶ����ݺ�Ĵ�����Ϊ
*	Name		: down_data_action
*	Input		: data
                              len     
*	Returns		: None
 ********************************************************************************/
void down_data_action(unsigned char *data, int len);


/********************************************************************************
*	Description	: �����ݽ��գ�����״̬����
*	Name		: hfservice_cloud_data_recv
*	Input		: type 
                              data
                              len
*	Returns		: None
 ********************************************************************************/
void hfservice_cloud_data_recv(enum CLOUD_DATA_TYPE type, unsigned char *data, int *len);	

#endif/*_HFSERVICE_DOWNSTREAM_H_*/

