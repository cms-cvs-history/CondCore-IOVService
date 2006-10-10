#ifndef CondCore_IOVService_IOVIterator_h
#define CondCore_IOVService_IOVIterator_h
#include <string>
#include "CondCore/DBCommon/interface/Ref.h"
namespace cond{
  class DBSession;
  class IOVIterator{
  public:
    IOVIterator(cond::DBSession& session,
		  const std::string token):m_session(session),m_token(token){}
    virtual ~IOVIterator(){}
    virtual void open()=0;
    virtual void close()=0;
    virtual bool next()=0;
    virtual std::string payloadToken() const=0;
    virtual std::pair<unsigned long long, unsigned long long> validity() const=0;
    virtual size_t size() const=0;
    virtual bool isValid( unsigned long long time ) const=0;
  protected:
    cond::DBSession& m_session;
    std::string m_token;
  } ;
}//ns cond
#endif
