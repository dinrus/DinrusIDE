#include "uchardet.h"
#include "nscore.h"
#include "nsUniversalDetector.h"
#include <Core/Core.h>

using Upp::String;

class HandleUniversalDetector : public nsUniversalDetector
{
protected:
	String m_charset;

public:
    HandleUniversalDetector()
    : nsUniversalDetector(NS_FILTER_ALL)
    {
        m_charset = "";
    }

    virtual ~HandleUniversalDetector()
    {}

    virtual void Report(const char* charset)
    {
        m_charset = charset;
    }

    virtual void Reset()
    {
        nsUniversalDetector::Reset();
        m_charset = "";
    }

    const String* GetCharset() const
    {
        return &m_charset;
    }
};

uchardet_t uchardet_new()
{
    return reinterpret_cast<uchardet_t> (new HandleUniversalDetector());
}

void uchardet_delete(uchardet_t ud)
{
    delete reinterpret_cast<HandleUniversalDetector*>(ud);
}

int uchardet_handle_data(uchardet_t ud, const char * data, size_t len)
{
    nsresult ret = reinterpret_cast<HandleUniversalDetector*>(ud)->HandleData(data, (PRUint32)len);
    return (ret != NS_OK);
}

void uchardet_data_end(uchardet_t ud)
{
    reinterpret_cast<HandleUniversalDetector*>(ud)->DataEnd();
}

void uchardet_reset(uchardet_t ud)
{
    reinterpret_cast<HandleUniversalDetector*>(ud)->Reset();
}

const String* uchardet_get_charset(uchardet_t ud)
{
    return reinterpret_cast<HandleUniversalDetector*>(ud)->GetCharset();
}
