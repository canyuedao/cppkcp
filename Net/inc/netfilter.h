#ifndef NET_FILTER_H
#define NET_FILTER_H

#ifdef WIN32
#ifdef NET_FILTER_EXPORT
#define NET_FILTER_API __declspec(dllexport)
#else
#define NET_FILTER_API
#endif
#else
#define NET_FILTER_API  
#endif
#ifdef WIN32
#include <memory>
#elif defined(IOS)||defined(MAC_OS)
#include <memory>
#else
#include <tr1/memory>
#endif

class CDataPackage
{
public:
    CDataPackage():m_szBuff(0), m_nLength(0){}
    CDataPackage(char* szBuff, unsigned int nLength) :m_szBuff(szBuff), m_nLength(nLength){};

    void SetPackage(char* szBuff, unsigned int  nLength)
    {
        if( m_szBuff != 0 )
        {
            delete []m_szBuff;
            m_szBuff = 0;
        }
        m_szBuff = szBuff;
        m_nLength = nLength;
    };

    void DetachPackage()
    {
        m_szBuff = 0;
        m_nLength = 0;
    };


    ~CDataPackage(void)
    {
        delete []m_szBuff;
    };
    char* GetData() {  return m_szBuff; }
    unsigned int  GetDataLength(){ return m_nLength; }
private:
    char* m_szBuff;
    unsigned int  m_nLength;
};

/**
 * @brief �������ݹ�������������Ӧ��Э���ʽ������������
 */
class NetFilter
{
public:
    virtual ~NetFilter(){}
    virtual bool InitFilter(const char* szBuff, unsigned int nLength) = 0;
    virtual bool FilterRead(const char* szBuff, unsigned int nLength) = 0;
    virtual bool GetPackage(CDataPackage& package) = 0;
    virtual bool FilterWrite(const char* szBuff, unsigned int nLength, CDataPackage& package) = 0;
	virtual bool FilterWrite(const char* szBuff, unsigned int nLength, 
			char **pOutPacket, int &nLen) = 0;
	virtual unsigned int GetHeaderLen() const = 0;
};

/**
 * ö�����ͣ�ö�����е�Filter����
 */
enum NetFilterType
{
    NET_FILTER_UNKNOW = -1,
    NET_TPKT_FILTER
};

extern "C"
{
    /**
    * �ṩ�����������Ĺ�����ӿ�
    */
    NET_FILTER_API NetFilter* CreateNetFilter(NetFilterType enuType); 
};

#endif
