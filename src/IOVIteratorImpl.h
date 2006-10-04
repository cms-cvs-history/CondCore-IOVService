#ifndef CondCore_IOVService_IOVIteratorImpl_h
#define CondCore_IOVService_IOVIteratorImpl_h
#include <string>
#include "CondCore/IOVService/interface/IOVIterator.h"
namespace cond{
  class DBSession;
  class IOV;
  class IOVIteratorImpl : virtual public cond::IOVIterator{
  public:
    IOVIteratorImpl( DBSession& sessionHandle,
		     const std::string token );
    virtual ~IOVIteratorImpl();
    virtual void open();
    virtual void close();
    virtual bool gotoNext();
    virtual std::string payloadToken() const;
    virtual std::pair<unsigned long long, unsigned long long> validity() const;
    virtual size_t size() const;
    virtual bool isValid( unsigned long long time ) const;
  private:
    bool m_isActive;
    cond::IOV* m_iov;
    size_t m_currentPos;
  };
}//ns cond
#endif
