#ifndef __CBASEIOTHREADWIN__H
#define __CBASEIOTHREADWIN__H
#include "IBaseIOThread.h"

class CSession;
class CIOService;
class CBaseIOThreadWin : public IBaseIOThread
{
public:
	/** 
	* @brief ���캯��
	*/
	CBaseIOThreadWin();

	/** 
	* @brief ��������
	*/
	virtual ~CBaseIOThreadWin();

	/**
	 * @brief ֪ͨ����ɶ˿�
	 */
	virtual bool postToIocp(void* item);

	/** 
	* @brief �Ƴ�ѭ��
	*/
	virtual void eventloopexit();

	/** 
	* @brief pipe libevent ��ʼ��
	*/
	typedef void (*CBFUNC)(int fd, short which, void *arg);
	void initPipeLibevent(CBFUNC callback, void *p);

protected:

	/** 
	 * @brief  �̺߳��� add by guozm 2012/10/11 
	 */
	virtual void onThreadRun();

	/** 
	 * @brief  �ͷ��Լ� add by guozm 2016/09/19 
	 */
	virtual void delMySelf()
	{
		delete this;
	}

protected:
	bool m_bRun;
	HANDLE m_iocp;
	struct event_base *base;    /* libevent handle this thread uses */
	struct event notify_event;  /* listen event for notify pipe */
};

#endif
