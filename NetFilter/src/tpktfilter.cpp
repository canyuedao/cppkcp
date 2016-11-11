#include "tpktfilter.h"
#include "memory.h"
#include <assert.h>
#include <stdlib.h>

CTpktFilter::CTpktFilter(void) : m_pFilterBuff(NULL), m_nCurPos(0), m_nWaitingLength(0)
{
    m_pFilterBuff = (char*)malloc(PER_ALLOC_SIZE);
    if( m_pFilterBuff == NULL )
    {
        assert(0);
    }
    m_nMaxPos = PER_ALLOC_SIZE;
}

CTpktFilter::~CTpktFilter(void)
{
    free(m_pFilterBuff);
    m_pFilterBuff = NULL;
}

bool CTpktFilter::FilterRead(const char* szBuff, unsigned int nLength)
{
    bool bHasPackage = false;
    while ( m_nCurPos + nLength > m_nMaxPos )
    {
        // ���������ڴ�
        m_nMaxPos += PER_ALLOC_SIZE;
		m_pFilterBuff = (char*)realloc(m_pFilterBuff, m_nMaxPos);        
    }

    // �˴������Ż�����m_pFilterBuff��û�����ݻ���ʱ������ֱ����szBuff�Ͻ��з��������ٿ�������
    memcpy(m_pFilterBuff + m_nCurPos, szBuff, nLength);
    m_nCurPos += nLength;

    if( m_nWaitingLength > nLength )
    {
        // �������ݲ������ȴ������İ�
        m_nWaitingLength -= nLength;
        return false;
    }
    else
    {
        // ��ʼ���
        bool    bFinished = false;
        unsigned int     nFilterPos = 0; // �Ѿ���õİ�λ��
        
        while (!bFinished)
        {
            // TPKT��ͷ������
            if( nFilterPos + sizeof(TPKT_HEADER) > m_nCurPos )
            {
                // �ȴ��ֽڳ�����Ϊ0,��֤�´ΰ�������ִ��
                m_nWaitingLength = 0;
                
                // ǰ�ƻ�����λ��
                unsigned nStartPos = nFilterPos;
                m_nCurPos -= nStartPos;
                memmove(m_pFilterBuff, m_pFilterBuff + nStartPos, m_nCurPos);
                break;
            }
            TPKT_HEADER header;

            memcpy(&header, m_pFilterBuff + nFilterPos, sizeof(TPKT_HEADER));
            nFilterPos += sizeof(TPKT_HEADER); // ���Ӱ�ͷ�ĳ���
            if( header.size == 0 )
            {
				return false;
            }

            if( header.reverse != 0x01) 
            {
				return false;
            }

            if(header.version != 0x03)
            {
				return false;
            }

            if(nFilterPos + header.size > m_nCurPos )
            {
                // ���������������ƶ����ڴ濪ʼ��λ�ò��ȴ���������.��ʹ��
                // memcpy��ԭ����src��dst�Ĳ�����������ص�
                // û�н��������ʱ����Ҫ�����ͷ��������
                m_nWaitingLength = nFilterPos + header.size - m_nCurPos;
                unsigned nStartPos = nFilterPos - sizeof(TPKT_HEADER);
                m_nCurPos -= nStartPos;
                memmove(m_pFilterBuff, m_pFilterBuff + nStartPos, m_nCurPos);
                break;
            }
            else
            {
                // ����������һ����
                char *pPackage = new char[header.size];
                memcpy(pPackage, m_pFilterBuff + nFilterPos, header.size);
                
                // ���棬�ȴ��ⲿ����
                m_dqPackage.push_back(DataInfo(pPackage, header.size));
                nFilterPos += header.size;

                bHasPackage = true;
                // ȫ��������ɣ������������еİ�����������
                if( m_nCurPos == nFilterPos)
                {
                    m_nWaitingLength = 0;
                    m_nCurPos = 0;
                    break;
                }
            }
            
        }
    }
	return bHasPackage;
}

bool CTpktFilter::GetPackage(CDataPackage& package)
{
    if( m_dqPackage.size() == 0 )
    {
        return false;
    }
    else
    {
        package.SetPackage(m_dqPackage[0].m_pData, m_dqPackage[0].m_nLength);
        m_dqPackage.pop_front();
    }
    return true;
}

bool CTpktFilter::FilterWrite(const char* szBuff, unsigned int nLength, CDataPackage& package)
{
    if( nLength > 0xFFFFFFF )
    {
        assert(0);
        return false;
    }
    
    TPKT_HEADER header;
    size_t hdSize = sizeof(TPKT_HEADER);
    char *pPackage = new char[nLength + hdSize];
	memset(pPackage, 0, nLength+hdSize);

    if( pPackage == NULL )
    {
        assert(0);
        return false;
    }

    header.size = (unsigned int)nLength;
    header.version = 0x03;
    header.reverse = 0x01;

    memcpy(pPackage, &header, hdSize);
    memcpy(pPackage+hdSize, szBuff, nLength);
    package.SetPackage(pPackage, nLength + hdSize);
	return true;
}

bool CTpktFilter::FilterWrite(const char* szBuff, unsigned int nLength, 
		char **pOutPacket, int &nLen)
{
	if( nLength > 0xFFFFFFF )
    {
        assert(0);
        return false;
    }

	TPKT_HEADER header;
	size_t hdSize = sizeof(TPKT_HEADER);
    *pOutPacket = new char[nLength + hdSize];
	memset(*pOutPacket, 0, nLength+hdSize);

	header.size = (unsigned int)nLength;
	header.version = 0x03;
	header.reverse = 0x01;

	memcpy(*pOutPacket, &header, hdSize);
	memcpy(*pOutPacket+hdSize, szBuff, nLength);
	nLen =  nLength + hdSize;
	return true;
}
