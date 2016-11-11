/** 
* @file BaseIOThread.h
* @brief �̻߳���
* @add by guozm guozm
* @version 
* @date 2012-04-05
*/

#ifndef __CBASEIOTHREAD__H
#define __CBASEIOTHREAD__H
#ifdef WIN32
#define close closesocket
#endif
#include "IBaseIOThread.h"

class CIOService;
class CBaseIOThread : public IBaseIOThread
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/04/05 
		*/
		CBaseIOThread();

		/** 
		* @brief �������� add by guozm 2012/04/05 
		*/
		virtual ~CBaseIOThread();

		/** 
		* @brief �˳�ѭ�� add by guozm 2012/07/06 
		*/
		void eventloopexit();

		/** 
		 * @brief pipe libevent ��ʼ�� add by guozm 2012/04/05 
		 */
		void initPipeLibevent(void (*callback)(int , short, void *), void *p);
	
	protected:
		/** 
		 * @brief �߳����� add by guozm 2012/10/11 
		 */
		virtual void onThreadRun();

		/** 
		* @brief  �ͷ��Լ� add by guozm 2016/09/19 
		*/
		virtual void delMySelf()
		{
			delete this;
		}

	public:
		struct event_base *base;    /* libevent handle this thread uses */
		struct event notify_event;  /* listen event for notify pipe */
};

#endif
