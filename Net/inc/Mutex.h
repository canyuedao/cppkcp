/** 
* @file Mutex.h
* @brief ���ķ�װ
* @add by guozm guozm
* @version 
* @date 2012-06-15
*/

#ifndef __CMUTEX__H
#define __CMUTEX__H
#include <pthread.h>
#include <string>
#ifdef WIN32
#ifdef MUTEX_EXPORT
#define MUTEX_EXPORT __declspec(dllexport)
#else
#define MUTEX_EXPORT __declspec(dllimport)
#endif 
#else
#define MUTEX_EXPORT
#endif
class MUTEX_EXPORT CMutex 
{

public:
	/** 
	 * @brief  ���캯�� add by guozm 2012/06/15 
	 */
	CMutex();

	/** 
	* @brief �������� add by guozm 2012/06/15 
	*/
	~CMutex();

	/** 
	* @brief lock add by guozm 2012/06/15 
	*/
	void lock();

	/** 
	* @brief unlock add by guozm 2012/06/15 
	*/
	void unlock();

	pthread_mutex_t& getMutex()
	{
		return m_lock;
	}

private:
	pthread_mutex_t m_lock;
	pthread_t m_pid;

};

class MUTEX_EXPORT CMutexProxy
{
public:
	/** 
	* @brief  ���캯�� add by guozm 2012/06/15 
	*/
	CMutexProxy(CMutex& m);

	/** 
	* @brief �������� add by guozm 2012/06/15 
	*/
	~CMutexProxy();

private:
	CMutex &m_m;
};

/** 
* @brief ��д��
*/
class MUTEX_EXPORT CRWMutex
{
	public:
		/** 
		* @brief ���캯�� add by guozm 2012/06/15 
		*/
		CRWMutex();

		/** 
		* @brief �������� add by guozm 2012/06/15 
		*/
		~CRWMutex();

		/** 
		* @brief ������ add by guozm 2012/06/19 
		*/
		void rLock();

		/** 
		* @brief ������ add by guozm 2012/06/19 
		*/
		void rUnLock();

		/** 
		* @brief д���� add by guozm 2012/06/19 
		*/
		void wLock();

		/** 
		* @brief д���� add by guozm 2012/06/19 
		*/
		void wUnLock();

	private:
		pthread_rwlock_t m_lock;
};


class MUTEX_EXPORT CReadMutexProxy
{
public:
	/** 
	* @brief  ���캯�� add by guozm 2012/06/15 
	*/
	CReadMutexProxy(CRWMutex& m);

	/** 
	* @brief �������� add by guozm 2012/06/15 
	*/
	~CReadMutexProxy();

private:
	CRWMutex &m_m;
};

class MUTEX_EXPORT CWriteMutexProxy
{
public:
	/** 
	* @brief  ���캯�� add by guozm 2012/06/15 
	*/
	CWriteMutexProxy(CRWMutex& m);

	/** 
	* @brief �������� add by guozm 2012/06/15 
	*/
	~CWriteMutexProxy();

private:
	CRWMutex &m_m;
};

#endif
