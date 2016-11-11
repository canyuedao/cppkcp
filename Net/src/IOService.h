/** 
* @file IOService.h
* @brief ����������Ӳ�dispatcher���д���
* @add by guozm guozm
* @version 
* @date 2012-03-31
*/

#ifndef __CIOSERVICE__H
#define __CIOSERVICE__H
#include <event.h>
#include <iostream>
#include "common.h"
#include "netdef.h"
#include "networkio.h"
#include "IBaseIOThread.h"

class NetFilter;
class CIOService
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/03/31 
		*/
		CIOService();

		/** 
		* @brief  �������� add by guozm 2012/03/31 
		*/
		virtual ~CIOService();

		/** 
		 * @brief �����߳� add by guozm 2012/04/05 
		 */
		void startThread();

		/** 
		 * @brief  �̺߳��� add by guozm 2012/04/04 
		 */
		static void *threadFun(void *argc);

		/** 
		* @brief  ��ip�Ͷ˿� add by guozm 2012/03/31 
		*/
		//void bind(ENETWORKTRANSPORT eTransport, const string &sIP, int nPort);

		/** 
		 * @brief  ip�Ͷ˿� add by guozm 2012/04/06 
		 */
		virtual bool StartUp(IOServiceType serviceType, const string &sSrcIP, int nSrcPort, 
				const string &sDstIP, int nDstPort, IOEvent *pEventObj, NetFilter *pFilter, 
				int nThreadNum, bool bSync);

		/** 
		* @brief  tcpAccept�ص� add by guozm 2012/04/04 
		*/
		static void onTcpAcceptCallback(int fd, short which, void *arg);

		/** 
		 * @brief  ���µ����� add by guozm 2012/04/04 
		 */
		void dispatcherNewCon(int nfd, ESOCKETTYPE eSocketType, ENETWORKTYPE eNetworkType, 
				const string &sIP, const string &sPort);

	private:
		/** 
		* @brief ����Startup����
		*/
		struct tStartUpArg
		{
			IOServiceType serviceType;
			IPAddr mIPAddr;
			int nThreadNum;
			IOEvent *pEventObj;
			NetFilter *pFilter;
			bool volatile bSync;
		};

		struct event_base *m_pAcceptEventBase;    /* libevent handle this thread uses */
		struct event m_acceptEvent;
		vector<IBaseIOThread*> m_ReadThreadVec;    //����IO�̳߳� 
		vector<IBaseIOThread*> m_WriteThreadVec;   //����IO�̳߳� 
		tStartUpArg m_StartUpArg;
		pthread_mutex_t m_lock;
		pthread_cond_t m_cond;

		bool volatile m_bSuccess;             //connect����bind�Ƿ�ɹ�
		int m_nCount;
};

#endif
