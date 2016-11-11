/** 
* @file TCPWriteThread.h
* @brief libevent �����߳� 
* @add by guozm guozm
* @version 
* @date 2012-04-04
*/

#ifndef __CTCPWRITETHREAD__H
#define __CTCPWRITETHREAD__H
#include "Session.h"
#include "netdef.h"

#ifndef WIN32
#include "BaseIOThread.h"
class CTCPWriteThread : public CBaseIOThread
#else
#include "BaseIOThreadWin.h"
class CTCPWriteThread : public CBaseIOThreadWin
#endif
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/04/04 
		*/
		CTCPWriteThread();

		/** 
		* @brief �������� add by guozm 2012/04/04 
		*/
		~CTCPWriteThread();

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
		* @brief  ����д�¼� add by guozm 2012/09/02 
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
