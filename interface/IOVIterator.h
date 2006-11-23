#ifndef CondCore_IOVService_IOVIterator_h
#define CondCore_IOVService_IOVIterator_h
#include <string>
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/DBCommon/interface/Time.h"
namespace cond{
  class PoolStorageManager;
  class IOVIterator{
  public:
    IOVIterator(cond::PoolStorageManager& pooldb,
		const std::string token):m_pooldb(pooldb),m_token(token){}
    virtual ~IOVIterator(){}
    virtual void refresh()=0;
    virtual bool next()=0;
    virtual std::string payloadToken() const=0;
    virtual std::pair<cond::Time_t, cond::Time_t> validity() const=0;
    virtual bool isValid( cond::Time_t time ) const=0;
  protected:
    cond::PoolStorageManager& m_pooldb;
    std::string m_token;
  };
}//ns cond
#endif
