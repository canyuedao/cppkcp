#ifndef NETWORK_IO_H
#define NETWORK_IO_H

#ifdef WIN32
#ifdef NETWORK_IO_EXPORT
#define NETWORK_IO_API __declspec(dllexport)
#else
#define NETWORK_IO_API  
#endif
#else
#define NETWORK_IO_API  
#endif
#include "netdef.h"
#include "netfilter.h"
#ifdef WIN32
#include <memory>
#elif defined(IOS)||defined(MAC_OS)
#include <memory>
#else
#include <tr1/memory>
#endif

/** 
* @brief socket ����
*/
enum ESOCKETTYPE
{
	SOCKETTYPE_NULL = 0,
	SOCKETTYPE_ACCEPT,           //accept������
	SOCKETTYPE_CONNECT           //connect�µ�����
};

/*
 * ö�����ͣ�ö�����е�IO��������
 */
enum IOServiceType
{
	IO_SERVICE_UNKNOW = -1,
	IO_SERVICE_TCP_SERVER,
	IO_SERVICE_TCP_CLIENT,
	IO_SERVICE_KCP_SERVER,
	IO_SERVICE_KCP_CLIENT,
};

/** 
* @brief �������� 
*/
enum ENETWORKTYPE
{
	ENETWORKTYPE_NULL = 0,
	ENETWORKTYPE_TCP,
	ENETWORKTYPE_KCP,
};

/*
 * �����¼�ö�����ͣ�ö������IO�¼�����
 */
enum IOEventType
{
	IO_EVENT_UNKNOW = -1,

	IO_EVENT_INIT,          //��������IP���˿ڳɹ�
	IO_EVENT_OPEN_OK,       //�ɹ�����һ���µ�����
	IO_EVENT_CLIENT_OPEN_OK,//���ӷ������ɹ�
	IO_EVENT_READ_OK,       //��ȡ����
	IO_EVENT_READMSG_OK,    //ֻ��udx/kcp��ʹ��
	IO_EVENT_WRITE_OK,      //�������ݳɹ�
	IO_EVENT_WRITEMSG_OK,   //ֻ��udx/kcp��ʹ��
	IO_EVENT_WRITE_FAILED,  //��������ʧ��

	IO_EVENT_OPEN_FAILED,   //���ӷ�����ʧ��
	IO_EVENT_CLOSED,        //���ӱ��ر�
};



class IOHandle;
class P2PConnector;
//�����¼��ص��ӿ�
class IOEvent
{
	public:
	virtual ~IOEvent(){};
	virtual int OnIOEvent(IOHandle* ioHandle, IOEventType enuEvent, char* dataBuffer, int nLength) = 0;
	virtual int OnP2PIOEvent(P2PConnector* connector, IOEventType enuEvent, char* dataBuffer, int nLength){return 0;}
};

// ��ѯ��Ϣ������
enum QueryInfoType
{
	QUERY_LOCAL_IP = 0,
	QUERY_LOCAL_PORT,
	QUERY_REMOTE_IP,
	QUERY_REMOTE_PORT,
};

//�������Ӷ��󡪡���Ӧÿһ����Ե�����
class IOHandle 
{
public:
	virtual ~IOHandle() {}

	/**
	* @brief д����  add by guozm 2012/12/17 
	*/
	virtual void Write(const char* dataBuffer, int nLength, bool bReliable = true) = 0;

	/** 
	 * @brief  ���Ϳ�������(udx/kcp) add by guozm 2015/04/03 
	 */
	virtual void WriteMsg(const char* dataBuffer, int nLength) = 0;

	/**
	* ��ѯ���ӵ������Ϣ
	*/
	virtual const char* QueryHandleInfo(QueryInfoType nKey) = 0;

	/** 
	* @brief  ��ѯENETWORKTYPE add by guozm 2012/11/01 
	*/
	virtual ENETWORKTYPE QueryENetWorkType() = 0;

	/** 
	* @brief ��ѯENETWORKTYPE add by guozm 2012/11/01 
	*/
	virtual string QueryENetWorkTypeToString() = 0;

	/**
	* �ر�����
	*/
	virtual void CloseHandle() = 0;

	/** 
	* @brief  ע��IOEvent add by guozm 2012/04/13 
	*/
	virtual void Register(IOEvent *pIOEvent, NetFilter *pFilter = NULL) = 0;

	/** 
	* @brief  ���NetFilter ���� add by guozm 2012/12/17 
	*/
	virtual NetFilter *getNetFilter() = 0;

	/** 
	 * @brief  ���socket ���� add by guozm 2014/11/16 
	 */
	virtual ESOCKETTYPE getSocketType() = 0;

	/** 
	 * @brief  only kcp add by guozm 2014/11/16 
	 */
	virtual int getWaitSendSize()
	{
		return 0;
	}

	/** 
	* @brief  Can send data? for udx
	*/
	virtual bool CanSend() {return true;}

	/** 
	* @brief  Set send buf length, for udx
	*/
	virtual void SetSendBuf(int nLen) {}
};

extern "C"
{
	/**
	 *  * �ṩ�����������Ĺ�����ӿ�
	 *   */
	//NETWORK_IO_API IOFactory* CreateIOFacotry();
	NETWORK_IO_API bool CreateServerIOService(IOServiceType serviceType, const char* sSrcIP,
			int nSrcPort, IOEvent *pEventObj, NetFilter *pFilter, int nThreadNum, 
			bool bSync);

	NETWORK_IO_API bool CreateClientIOService(IOServiceType serviceType, const char* sSrcIP,
			int nSrcPort, const char* sDstIP, int nDstPort, IOEvent *pEventObj,
			NetFilter *pFilter, int nThreadNum, bool bSync);
};

#endif
