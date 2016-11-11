/** 
* @file TCPReadThread.h
* @brief libevent �����߳� 
* @add by guozm guozm
* @version 
* @date 2012-04-04
*/

#ifndef __CTCPREADTHREAD__H
#define __CTCPREADTHREAD__H
#include "Session.h"
#include "netdef.h"

#ifndef WIN32
#include "BaseIOThread.h"
#else
#include "BaseIOThreadWin.h"
#endif

#include "IOService.h"

class NetFilter;
#ifndef WIN32
class CTCPReadThread : public CBaseIOThread
#else
class CTCPReadThread : public CBaseIOThreadWin
#endif
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/04/04 
		*/
		CTCPReadThread();

		/** 
		* @brief �������� add by guozm 2012/04/04 
		*/
		~CTCPReadThread();

		/** 
		* @brief  ��ӿ���sockfd add by guozm 2012/04/13 
		*/
		void addAliveSockfdVec(int nfd);

		/** 
		* @brief  ɾ������sockfd  add by guozm 2012/04/13 
		*/
		bool delAliveSockfdVec(int nfd);

		/** 
		 * @brief pipe���ݻص� add by guozm 2012/04/04 
		 */
		static void onPipeProcessCallback(int fd, short which, void *arg);

		/** 
		 * @brief  ������µ�Session add by guozm 2012/04/04 
		 */
		CSession *newSession(int sfd, IBaseIOThread *pWriteIOThread, 
				ENETWORKTYPE eNetworkType, const IPAddr &mIPAddr);

		/** 
		 * @brief �����ȡ���ݻص� add by guozm 2012/04/04 
		 */
		static void onNetReadProcessCallback(int fd, short which, void *arg);

	protected:
		/** 
		 * @brief �ͷ���Դ add by guozm 2012/07/02 
		 */
		virtual void onRelease();

	private:
		vector<int> m_AliveSockfdVec;   //���õ��׽���
};

#endif
