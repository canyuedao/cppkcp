/** 
* @file KCPWriteThread.h
* @brief libevent �����߳� 
* @add by guozm guozm
* @version 
* @date 2012-04-04
*/

#ifndef __CKCPWRITETHREAD__H
#define __CKCPWRITETHREAD__H
#include "Session.h"
#include "netdef.h"
#include "kcpinc.h"

#ifndef WIN32
#include "BaseIOThread.h"
class CKCPWriteThread : public CBaseIOThread
#else
#include "BaseIOThreadWin.h"
class CKCPWriteThread : public CBaseIOThreadWin
#endif
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/04/04 
		*/
		CKCPWriteThread();

		/** 
		* @brief �������� add by guozm 2012/04/04 
		*/
		~CKCPWriteThread();

		/** 
		 * @brief ��ն��� add by guozm 2012/07/02 
		 */
		virtual void clearQue()
		{
			CMutexProxy mLock(m_Mutex);
			while(!m_Queue.empty())
			{
				void *p = m_Queue.front();
				m_Queue.pop();
				CQ_NOTIFYWRITE_ITEM *pDel = (CQ_NOTIFYWRITE_ITEM*)p;
				delete pDel;
				pDel = NULL;
			}
		}

		/** 
		 * @brief  ����kcp�ӿ� add by guozm 2016/08/17 
		 */
		static void dokcp(CSession *pSession, const char *sBuf, int nLen);

		/** 
		 * @brief  ����kcp�ӿ� add by guozm 2016/08/17 
		 */
		static void doUnreliabledata(CSession *pSession, const char *sBuf, int nLen);

		/** 
		 * @brief  ����д�¼�
		 */
		void setWriteEvent(int nSockfd, CSession *pSession);

		/** 
		 * @brief pipe���ݻص� add by guozm 2012/04/04 
		 */
		static void onPipeProcessCallback(int fd, short which, void *arg);

		/** 
		 * @brief �����ȡ���ݻص� add by guozm 2012/04/04 
		 */
		static void onNetWriteProcessCallback(int fd, short which, void *arg);

	protected:
		/** 
		 * @brief �ͷ���Դ add by guozm 2012/07/02 
		 */
		virtual void onRelease();
};

#endif
