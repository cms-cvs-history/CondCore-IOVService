#ifndef CondCore_IOVService_IOVEditor_h
#define CondCore_IOVService_IOVEditor_h
#include <string>
#include <vector>
#include <map>
#include "CondCore/DBCommon/interface/Time.h"
namespace cond{
  class IOVEditor{
  public:
    IOVEditor():m_token(""){}
    explicit IOVEditor( const std::string& token ):m_token(token){
    }
    virtual ~IOVEditor(){}
    virtual void insert( const std::string& payloadToken, 
			 cond::Time_t tillTime ) = 0;
    virtual void bulkInsert( std::vector< std::pair<cond::Time_t,std::string> >& values ) = 0;
    virtual void updateClosure( cond::Time_t newtillTime ) = 0;
    virtual void append( const std::string& payloadToken,
			 cond::Time_t sinceTime ) = 0;
    virtual void bulkAppend( std::vector< std::pair<cond::Time_t,std::string> >& values ) = 0;
    virtual void deleteEntries() = 0;
    virtual void import( const std::string& sourceIOVtoken ) = 0;
    std::string token() const {
      return m_token;
    }
  protected:
    std::string m_token;
  };
}//ns cond
#endif
