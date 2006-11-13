#ifndef CondCore_IOVService_IOVIteratorImpl_h
#define CondCore_IOVService_IOVIteratorImpl_h
#include <string>
#include "CondCore/IOVService/interface/IOVIterator.h"
#include "CondCore/DBCommon/interface/Ref.h"
namespace cond{
  class PoolStorageManager;
  class IOV;
  class IOVIteratorImpl : virtual public cond::IOVIterator{
  public:
    IOVIteratorImpl( PoolStorageManager& pooldb,
		     const std::string token );
    virtual ~IOVIteratorImpl();
    virtual void refresh();
    virtual bool next();
    virtual std::string payloadToken() const;
    virtual std::pair<unsigned long long, unsigned long long> validity() const;
    virtual bool isValid( unsigned long long time ) const;
  private:
    cond::Ref<cond::IOV> m_iov;
    size_t m_currentPos;
    size_t m_stop;
    bool m_isOpen;
  };
}//ns cond
#endif
