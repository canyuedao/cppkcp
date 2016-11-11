/** 
* @file Session.h
* @brief �Ự�� 
* @add by guozm guozm
* @version 
* @date 2012-04-04
*/

#ifndef __CSESSION__H
#define __CSESSION__H
#include "netdef.h"
#include "networkio.h"
#include "IBaseIOThread.h"
#include "netfilter.h"
#include "IOService.h"
#include "ikcp.h"
#include "kcpinc.h"

class CKCPClient;
class CSession : public IOHandle
{
	public:
		/** 
		 * @brief ���캯�� add by guozm 2012/04/04 
		 */
		CSession(IBaseIOThread *pWriteIOThread, IBaseIOThread *pReadIOThread, 
				ENETWORKTYPE eNetworkType, const IPAddr &mIPAddr);

		/** 
		 * @brief ���캯�� add by guozm 2012/04/04 
		 */
		CSession(IBaseIOThread *pWriteIOThread, IBaseIOThread *pReadIOThread, 
				ENETWORKTYPE eNetworkType, const IPAddr &mIPAddr,
				struct sockaddr_in addr, int nfd, ESOCKETTYPE eSocketType);

		/** 
		 * @brief ���캯�� add by guozm 2012/04/04 
		 */
		CSession(IBaseIOThread *pWriteIOThread, IBaseIOThread *pReadIOThread, 
				ENETWORKTYPE eNetworkType, const IPAddr &mIPAddr,
				long long nServerObj, CKCPClient *pKCPClient);

		/** 
		 * @brief �������� add by guozm 2012/04/04 
		 */
		virtual ~CSession();

		/** 
		* @brief �ͷ�kcp add by guozm 2016/09/08 
		*/
		void releaseKcp();

		/** 
		* @brief  ��ʼ�� add by guozm 2016/08/25 
		*/
		void initObj();

		/** 
		 * @brief  ����KCPServerObj add by guozm 2016/10/28 
		 */
		void setKCPServerObj(SServerObj *pObj);

		/** 
		 * @brief  ��� add by guozm 2016/10/29 
		 */
		SServerObj *getSServerObj();

		/** 
		* @brief  ������Ϣ add by guozm 2012/04/12 
		*/
		virtual void Write(const char* dataBuffer, int nLength, bool bReliable = true);

		/** 
		 * @brief  ���Ϳ�������(kcp) add by guozm 2015/04/03 
		 */
		virtual void WriteMsg(const char* dataBuffer, int nLength);

		/** 
		* @brief  ��ѯ��Ϣ add by guozm 2012/06/14 
		*/
		virtual const char* QueryHandleInfo(QueryInfoType nKey);

		/** 
		 * @brief  ��ѯENETWORKTYPE add by guozm 2012/11/01 
		 */
		virtual ENETWORKTYPE QueryENetWorkType();

		/** 
		 * @brief ��ѯENETWORKTYPE add by guozm 2012/11/01 
		 */
		virtual string QueryENetWorkTypeToString();

		/** 
		* @brief �ر����� add by guozm 2012/04/24  
		*/
		virtual void CloseHandle();

		/** 
		* @brief  ע���¼������� add by guozm 2012/04/12 
		*/
		virtual void Register(IOEvent *pIOEvent, NetFilter *pFilter = NULL);

		/** 
		* @brief  ���NetFilter add by guozm 2012/12/17 
		*/
		NetFilter *getNetFilter();

		/** 
		 * @brief  ���socket ���� add by guozm 2014/11/16 
		 */
		virtual ESOCKETTYPE getSocketType();

		/** 
		 * @brief  only kcpadd by guozm 2014/11/16 
		 */
		virtual int getWaitSendSize();

		/** 
		 * @brief  Can send data? for udx/kcp
		 */
		virtual bool CanSend();

	public:
		/** 
		 * @brief  ����kcp���� add by guozm 2016/08/16 
		 */
		void createKcpObj(long long nKcpNum);

		/** 
		 * @brief  ���kcp���� add by guozm 2016/08/19 
		 */
		CMutexProxy *getKcpObj(ikcpcb **pKcpObj);

		/** 
		 * @brief  kcpconnect�ɹ� add by guozm 2016/08/18 
		 */
		void onKcpConnectSuccess(int nfd, CIOService *pIOService);

		/** 
		 * @brief  �յ�kcp���� add by guozm 2016/08/12 
		 */
		void onRecvKcpData(char *sData, int nLen);

		/** 
		 * @brief  �յ�kcp���� add by guozm 2016/08/12 
		 */
		void onRecvKcpDataMsg(char *sData, int nLen);

		/** 
		 * @brief  �Ͽ����� add by guozm 2012/04/12 
		 */
		void onRecvKcpClose();

		/** 
		 * @brief  �յ�readcloseok add by guozm 2016/08/16 
		 */
		void onRecvKcpReadCloseOK();

		/** 
		 * @brief ����close��Ϣ add by guozm 2016/08/16 
		 */
		void sendKcpClose();

		/** 
		* @brief  ����create��Ϣ add by guozm 2016/08/25 
		*/
		void sendKcpCreate();

		/** 
		* @brief ����kcp������ add by guozm 2016/08/27 
		*/
		void sendKcpHeartbeat();

		/** 
		 * @brief ����kcp���� add by guozm 2016/08/27 
		 */
		void sendKcpData(ESOCKETSTATUS eSocketStatus, const char* dataBuffer = NULL, 
				int nLength = 0);

		/** 
		 * @brief  ���ӷ������ɹ� add by guozm 2012/04/13 
		 */
		void onConnectSuccess(int nfd, CIOService *pIOService);

		/** 
		 * @brief  �µ����� add by guozm 2012/04/12 
		 */
		void onNetConnect(int nfd);

		/** 
		 * @brief  �µ����� add by guozm 2012/04/12 
		 */
		void onNetKcpConnect();

		/** 
		 * @brief  �յ��������� add by guozm 2012/04/12 
		 */
		void onRecvData(char *sData, int nLen);

		/** 
		 * @brief  �յ���������(kcp) add by guozm 2012/04/12 
		 */
		void onRecvDataMsg(char *sData, int nLen);

		/** 
		 * @brief  �Ͽ����� add by guozm 2012/04/12 
		 */
		void onRecvClose();

		/** 
		 * @brief  �Ͽ����� add by guozm 2012/04/12 
		 */
		void onRecvTcpClose();

		/** 
		* @brief �Ͽ����� add by guozm 2012/04/12 
		*/
		void onRecvAppClose();

		/** 
		* @brief д�߳������˳� add by guozm 2012/06/13 
		*/
		void onWriteThreadCloseOK();

		/** 
		 * @brief д�߳������˳� add by guozm 2012/06/13 
		 */
		void onTcpWriteThreadCloseOK();

		/** 
		 * @brief д�߳������˳� add by guozm 2012/06/13 
		 */
		void onKcpWriteThreadCloseOK();

		/** 
		* @brief д����ʧ�� add by guozm 2012/08/28 
		*/
		void onWriteFailure();

		/** 
		* @brief  д�¼��ɹ� add by guozm 2012/11/09 
		*/
		void onWriteOK(char *dataBuffer, int nLength);

		/** 
		 * @brief  д�¼��ɹ� add by guozm 2012/11/09 
		 */
		void onWriteMsgOK(char *dataBuffer, int nLength);

		/** 
		* @brief  �յ������¼� add by guozm 2012/06/15 
		*/
		void onIOEvent(IOEventType enuEvent, char* dataBuffer, int nLength);

	public:
		/** 
		 * @brief �����ȡ���ݻص� add by guozm 2012/04/04 
		 */
		static void onNetReadProcessCallback(int fd, short which, void *arg);

	private:

		/** 
		* @brief  ��ӵ�д�Ķ��� add by guozm 2012/06/14 
		*/
		void addWriteQue(const char* dataBuffer, int nLength, ESOCKETSTATUS eSocketStatus);

		/** 
		* @brief ֪ͨд�߳̽��йر� add by guozm 2012/07/02 
		*/
		void notifyCloseToWriteThread();

		/** 
		 * @brief ֪ͨ���̹߳ر� add by guozm 2012/07/03 
		 */
		void notifyTcpCloseToReadThread();

		/** 
		 * @brief ֪ͨ���̹߳ر� add by guozm 2012/07/03 
		 */
		void notifyKcpCloseToReadThread();

		/** 
		 * @brief ������Ϣ add by guozm 2012/07/03 
		 */
		void writedata(const char* dataBuffer, int nLength, ESOCKETSTATUS eSocketStatus);

	public:
		/** 
		* @brief  �������� add by guozm 2012/09/02 
		*/
		int pushWriteQue(CQ_WRITE_ITEM *pItme);

		/** 
		* @brief  д����ͷ add by guozm 2012/09/02 
		*/
		CQ_WRITE_ITEM *writeQueFront();

		/** 
		* @brief ��շ��ͻ����� add by guozm 2012/09/02 
		*/
		void clearWriteQue();

		/** 
		* @brief  ���д�С add by guozm 2012/09/02 
		*/
		int writeQueSize();

	private:
		friend class CTCPReadThread;
		friend class CTCPWriteThread;
		friend class CKCPReadThread;
		friend class CKCPWriteThread;
		friend class CKCPClient;
		friend class CKCPServer;
		struct event m_readEvent;
		struct event m_writeEvent;
		IBaseIOThread *m_pWriteThread;
		IBaseIOThread *m_pReadThread;
		IOEvent *m_pIOEvent;
		NetFilter *m_pNetFilter;
		int m_nfd;                   //������
		static const int m_nBufferLen = 65535;
		char m_pBuffer[m_nBufferLen];
		CIOService *m_pIOService;   //����ΪClient����CIOService ��Close��ʱ��ɾ������

		pthread_mutex_t m_mutex;
		queue<CQ_WRITE_ITEM*> m_WriteQueue;   //����   
		SServerObj *m_pSServerObj;
	public:
		bool volatile m_bClose;               //��ʶ�����Ƿ񱻹ر�
		bool volatile m_bSendCloseHandle;
		bool volatile m_bSendError;           //����ʧ��, ���ڷ������ݵȴ�����ر�
		bool volatile m_bKcpNetConnect;
		IPAddr m_IPAddr;
		struct sockaddr_in m_addr;            //kcp�ô˱���
		ENETWORKTYPE m_eNetWorkType;
		ESOCKETTYPE m_eSocketType;
		long long m_nServerObj;                    //kcp���server����
		ikcpcb* m_pkcp;
		long long m_nKcpNum;
		CMutex m_pkcpMutex;
		int m_nRecvHBCount;
		int m_nCloseCount;
		CKCPClient *m_pKCPClient;
		int m_nKcpServerVersion;
};

#endif
