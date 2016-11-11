/** 
* @file KCPHB.h
* @brief  ����CKCPThread ����
* @add by guozm guozm
* @version 
* @date 2016-08-18
*/

#ifndef __CKCPThread__H
#define __CKCPThread__H
#include "Mutex.h"
#include "Session.h"

class CSession;
class CKCPThread
{
	public:
		static CKCPThread *getKCPThread()
		{
			if(!m_pKCPThread)
			{
				m_pKCPThread = new CKCPThread;
			}
			return m_pKCPThread;
		}

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void addKCPUpdateObj(CSession* pObj);

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void delKCPUpdateObj(CSession* pObj);

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void addSendHBObj(CSession* pObj);

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void delSendHBObj(CSession* pObj, bool bDelRecvHBObj = true);
	
		/** 
		 * @brief  ɾ��kcocbobj add by guozm 2016/08/18 
		 */
		void addNetDelayDelObj(CSession* pObj);

		/** 
		* @brief ����ɾ��Session add by guozm 2011/02/04 
		*/
		void dealNetDelayDelObj();

		/** 
		 * @brief  ɾ��SServerObj add by guozm 2016/10/29 
		 */
		void addSServerObjDelayDelObj(SServerObj *pObj);

		/** 
		 * @brief  KCPClient ��ʱɾ�� add by guozm 2011/02/04 
		 */
		void addKCPClientDelayDelObj(CKCPClient *pKCPClient);

		/** 
		* @brief ����KCPClient ɾ�� add by guozm 2016/09/19 
		*/
		void dealKCPClientDelayDelObj();

		/** 
		 * @brief  ��� add by guozm 2016/09/08 
		 */
		void addCreateKCPObj(CKCPClient *pKCPClient);

		/** 
		 * @brief  ɾ�� add by guozm 2016/09/08 
		 */
		bool delCreateKCPObj(CKCPClient *pKCPClient);

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void addRecvHBObj(CSession* pObj);

		/** 
		 * @brief  ���kcocbobj add by guozm 2016/08/18 
		 */
		void delRecvHBObj(CSession* pObj);

		static int getRecvHBTimeout()
		{
			return RECVHBTIMEOUT;
		}

	private:
		/** 
		 * @brief  �߳� add by guozm 2016/08/18 
		 */
		static void *updateThreadFun(void *argc);

		/** 
		 * @brief  �߳� add by guozm 2016/08/18 
		 */
		static void *threadFun(void *argc);

		/** 
		 * @brief  �߳� add by guozm 2016/08/18 
		 */
		static void *delayThreadFun(void *argc);

		/** 
		 * @brief  ��ʱ�������� add by guozm 2016/08/27 
		 */
		void dealSendHeartbeat();

		/** 
		 * @brief  ����ʱ���� add by guozm 2016/08/29 
		 */
		void dealRecvHeartbeat();

		/** 
		* @brief add by guozm 2016/09/08 
		*/
		void dealCreateKCPObj();

		/** 
		 * @brief �����߳� add by guozm 2012/04/05 
		 */
		void startUpdateThread()
		{
			if(m_bUpdateRun)
			{
				return;
			}
			m_bUpdateRun = true;

			pthread_t       thread;
			int             ret;
			if((ret = pthread_create(&thread, NULL, CKCPThread::updateThreadFun, this)) != 0) 
			{
				assert(0);
			}

			pthread_detach(thread);
		}
		
		void startThread()
		{
			if(m_bRun)
			{
				return;
			}
			m_bRun = true;

			pthread_t       thread;
			int             ret;
			if((ret = pthread_create(&thread, NULL, CKCPThread::threadFun, this)) != 0) 
			{
				assert(0);
			}

			pthread_detach(thread);
		}

		/** 
		 * @brief �����߳� add by guozm 2012/04/05 
		 */
		void startDelayThread()
		{
			if(m_bDelayRun)
			{
				printf("startDelayThread m_bRun = %d return ..", m_bDelayRun);
				return;
			}
			m_bDelayRun = true;

			pthread_t       thread;
			int             ret;
			if((ret = pthread_create(&thread, NULL, CKCPThread::delayThreadFun, this)) != 0) 
			{
				assert(0);
			}

			pthread_detach(thread);
		}

		/** 
		 * @brief ���캯�� add by guozm 2016/08/18 
		 */
		CKCPThread()
		{
			m_bUpdateRun = false;
			m_bRun = false;
			m_bDelayRun = false;
			startUpdateThread();
			startThread();
			startDelayThread();
		}

		/** 
		 * @brief �������� add by guozm 2016/08/18 
		 */
		virtual ~CKCPThread()
		{
		}

	private:
		bool volatile m_bUpdateRun;
		bool volatile m_bRun;
		bool volatile m_bDelayRun;
		CMutex m_UpdateMutex;
		vector<CSession*> m_UpdateObjVec;

		CMutex m_SendHBMutex;
		vector<CSession*> m_SendHBObjVec;

		CMutex m_RecvHBMutex;
		vector<CSession*> m_RecvHBObjVec;

		CMutex m_DelayMutex;
		map<CSession*, int> m_DelaySessionMap;
		map<CKCPClient*, int> m_DelayKCPClientMap;
		vector<pair<SServerObj*, long> > m_DelaySServerObjVec;

		CMutex m_CreateObjMutex;
		vector<CKCPClient*> m_CreateObjVec;

		static CKCPThread *m_pKCPThread;
		static const int RECVHBTIMEOUT = 8;
};

#endif
