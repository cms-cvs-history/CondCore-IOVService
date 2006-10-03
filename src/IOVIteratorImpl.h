#ifndef CondCore_IOVService_IOVIteratorImpl_h
#define CondCore_IOVService_IOVIteratorImpl_h
#include <string>
#include "CondCore/IOVService/interface/IOVIterator.h"
//#include "CondCore/DBCommon/interface/Ref.h"
namespace cond{
  class DBSession;
  class IOV;
  class IOVIteratorImpl : virtual public cond::IOVIterator{
  public:
    IOVIteratorImpl( DBSession& sessionHandle,
		     const std::string token );
    virtual ~IOVIteratorImpl();
    virtual void open( bool isReadOnly=true );
    virtual void close();
    virtual bool gotoNext();
    virtual std::string payloadToken() const;
    virtual std::pair<unsigned long long, unsigned long long> validity() const;
    virtual size_t size() const;
    virtual bool isValid( unsigned long long time ) const;
    virtual void insert( const std::string& payloadToken, 
			 unsigned long long tillTime );
    virtual void deleteEntries();
  private:
    bool m_isActive;
    bool m_isReadOnly;
    cond::IOV* m_iov;
    size_t m_currentPos;
  };
}//ns cond
#endif
