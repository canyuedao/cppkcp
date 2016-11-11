/** 
* @file KCPClient.h
* @brief 
* @add by guozm guozm
* @version 
* @date 2016-08-11
*/

#ifndef __CKCPCLIENT__H
#define __CKCPCLIENT__H
#include "Session.h"
#include "netdef.h"
#include "Mutex.h"
#include "IOService.h"

#ifndef WIN32
#include "BaseIOThread.h"
#else
#include "BaseIOThreadWin.h"
#endif

#include "IOService.h"

class NetFilter;
#ifndef WIN32
class CKCPClient : public CBaseIOThread
#else
class CKCPClient : public CBaseIOThreadWin
#endif
{
	public:
		/** 
		 * @brief  ���캯�� add by guozm 2016/08/11 
		 */
		CKCPClient(int nSockfd, IOEvent *pIOEvent, 
				NetFilter *pNetFiler, vector<IBaseIOThread*> mReadVec, 
				vector<IBaseIOThread*> mWriteVec, CIOService *pIOService);

		/** 
		 * @brief �������� add by guozm 2016/08/11 
		 */
		~CKCPClient();

		/** 
		 * @brief  ������µ�Session add by guozm 2012/04/04 
		 */
		CSession *newSession(ENETWORKTYPE eNetworkType, 
				const IPAddr &mIPAddr, long long nServerObj);
		
		/** 
		 * @brief add by guozm 2016/09/13 
		 */
		void notifyCreate();

		/** 
		* @brief add by guozm 2016/09/13 
		*/
		void pipeCreate();

		/** 
		 * @brief  ����CreateOjbMsg add by guozm 2016/08/15 
		 */
		bool sendCreateObjMsg();
		
		/** 
		* @brief  ���IOEvent 
		*/
		IOEvent *getIOEvent()
		{
			return m_pIOEvent;
		}

		/** 
		 * @brief pipe���ݻص� add by guozm 2012/04/04 
		 */
		static void onPipeProcessCallback(int fd, short which, void *arg);

		/** 
		 * @brief �����ȡ���ݻص� add by guozm 2012/04/04 
		 */
		static void onNetReadProcessCallback(int fd, short which, void *arg);

		/** 
		 * @brief �����ȡ���ݻص� add by guozm 2012/04/04 
		 */
		static int udp_output(const char *sBuf, int nLen, ikcpcb *kcp, void *user);

	protected:
		/** 
		 * @brief �ͷ���Դ add by guozm 2012/07/02 
		 */
		virtual void onRelease();

		/** 
		 * @brief  �ͷ��Լ� add by guozm 2016/09/19 
		 */
		virtual void delMySelf();

	private:
		int m_nSockfd;
		struct event m_Event;
		IOEvent *m_pIOEvent;
		NetFilter *m_pNetFiler;
		vector<IBaseIOThread*> m_ReadThreadVec; 
		vector<IBaseIOThread*> m_WriteThreadVec; 
		unsigned int m_nReadCount;
		unsigned int m_nWriteCount;
		CIOService *m_pIOService;
		CSession *m_pSession;
	
	public:
		CMutex m_CreateObjMutex;
		bool volatile m_bCreatedKCPObj;
		bool volatile m_bCreateObjFailure;
		bool volatile m_bRelease;
		int m_nTryCount;
		char sNetBuffer[1500];
		char sKcpBuffer[64000];
};

#endif
