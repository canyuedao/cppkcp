#include "TCPWriteThread.h"
#include "Session.h"
#include <signal.h>
#ifdef WIN32
#define MSG_NOSIGNAL 0
#endif

#ifdef WIN32
#include <io.h>
#define EWOULDBLOCK WSAEWOULDBLOCK
#else
#include <unistd.h>
#endif

CTCPWriteThread::CTCPWriteThread()
{
	initPipeLibevent(onPipeProcessCallback, this);
}

CTCPWriteThread::~CTCPWriteThread()
{
}

void CTCPWriteThread::onRelease()
{
	//֪ͨ�����˶���
	CQ_NOTIFYWRITE_ITEM *notifyitem = new CQ_NOTIFYWRITE_ITEM;
	notifyitem->bDelMySelf = true;

#ifndef WIN32
	int nSize = push((void*)notifyitem);
	if(nSize <= 1)
	{
		if(write(notify_send_fd, "", 1) != 1) 
		{
			printf("send is failure error = %s\n", strerror(errno));
		}
	}
#else
	if (!postToIocp(notifyitem))
		ASSERTS(false, "0x%p postToIocp return false (0x%p)", this, notifyitem);
#endif
}

void CTCPWriteThread::onPipeProcessCallback(int fd, short which, void *arg)
{
	char buf[1];
	if(recv(fd, buf, 1, 0) != 1)
	{
		printf("recv is failure error = %s\n", strerror(errno));
	}

	CTCPWriteThread *pWriteIOThread = (CTCPWriteThread*)arg;
	CQ_NOTIFYWRITE_ITEM *notifyitem = NULL;
	while((notifyitem = (CQ_NOTIFYWRITE_ITEM*)pWriteIOThread->pop()) != NULL) 
	{
		if(notifyitem->bDelMySelf) 
		{
			pWriteIOThread->eventloopexit();
			break;
		}

		CSession *pSession = (CSession*)notifyitem->pSession;
		if(SOCKETSTATUS_CLOSE == notifyitem->eSocketStatus)
		{
			event_del(&pSession->m_writeEvent);
			pSession->onWriteThreadCloseOK();
		}
		else
		{
			event_add(&pSession->m_writeEvent, NULL);
		}

		if(notifyitem)
		{
			delete notifyitem;
			notifyitem = NULL;
		}
	}

	if(notifyitem)
	{
		delete notifyitem;
		notifyitem = NULL;
	}
}

void CTCPWriteThread::setWriteEvent(int nSockfd, CSession *pSession)
{
	//д�¼�
	event_set(&pSession->m_writeEvent, nSockfd, EV_WRITE, onNetWriteProcessCallback, 
			(void *)pSession);
	event_base_set(base, &pSession->m_writeEvent);
}

void CTCPWriteThread::onNetWriteProcessCallback(int fd, short which, void *arg)
{
	CSession *pSession = (CSession*)arg;
	CQ_WRITE_ITEM *writeItem = pSession->writeQueFront();
	if(NULL == writeItem)
	{
		return;
	}

	if(pSession->m_bSendError)
	{
		return;
	}

	int nLen = send(fd, writeItem->buf+writeItem->offset, 
			writeItem->len-writeItem->offset, MSG_NOSIGNAL);
	if(nLen == -1) // nLen == SOCKET_ERROR
	{
#ifndef WIN32
		if((errno == EINTR) || (errno == EAGAIN))
		{
			/* д������һ���źŴ�ϣ��������ǲ���д�����ݣ�����һ�²����ء� */
			printf("%s\n", "errno == EINTR || errno == EAGAIN");
			event_add(&pSession->m_writeEvent, NULL);
			return;
		}
		else 
		{
			//��Ϊ���ͳɹ����Դ���·����������
			printf("socket send is failure fd = %d error = %s removeip = %s removeport = %s\n", 
					fd, strerror(errno), pSession->m_IPAddr.m_sRemoteIP.c_str(), 
					pSession->m_IPAddr.m_sRemotePort.c_str());
			pSession->m_bSendError = true;
			pSession->clearWriteQue();
		}
#else
		int nError = WSAGetLastError();
		switch (nError)
		{
			case WSAEWOULDBLOCK:
				/* ���ǲ���д�����ݣ�����һ�²����ء� */
				ErrorMsg("%s", "errno == WSAEWOULDBLOCK");
				event_add(&pSession->m_writeEvent, NULL);
				return;
			default:
				ErrorMsg("send is failure error = %d", nError);
				pSession->m_bSendError = true;
				pSession->clearWriteQue();
				break;
		}
#endif
	}
	else if((writeItem->offset + nLen) < writeItem->len) 
	{
		/* �������е����ݶ���д���ˣ�����д���ƫ�Ʋ�����д���¼��� */
		writeItem->offset += nLen;
		event_add(&pSession->m_writeEvent, NULL);
	}
	else
	{
		pSession->onWriteOK(writeItem->buf, writeItem->len);
		writeItem->bSendOk = true;
	}
	event_add(&pSession->m_writeEvent, NULL);
}
