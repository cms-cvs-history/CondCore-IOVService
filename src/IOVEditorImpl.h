#ifndef CondCore_IOVService_IOVEditorImpl_h
#define CondCore_IOVService_IOVEditorImpl_h
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/DBCommon/interface/TypedRef.h"
namespace cond{
  class PoolTransaction;
  class IOV;
  class IOVEditorImpl : virtual public cond::IOVEditor{
  public:
    IOVEditorImpl( cond::PoolTransaction& pooldb,
		   const std::string& token,
		   cond::Time_t globalSince, 
		   cond::Time_t globalTill);
    /// Destructor
    virtual ~IOVEditorImpl();
    /// Assign a payload with till time. Returns the payload index in the iov sequence
    virtual unsigned int insert( cond::Time_t tillTime,
				 const std::string& payloadToken
				 );
    void bulkInsert( std::vector< std::pair<cond::Time_t,std::string> >& values );
    virtual void updateClosure( cond::Time_t newtillTime );
    /// Append a payload with known since time. The previous last payload's till time will be adjusted to the new payload since time. Returns the payload index in the iov sequence
    virtual unsigned int append(  cond::Time_t sinceTime,
				  const std::string& payloadToken
				  );
    virtual void deleteEntries( bool withPayload=false );
    virtual void import( const std::string& sourceIOVtoken );
    std::string token() const {
      return m_token;
    }
  private:
    void init();
    cond::PoolTransaction* m_pooldb;
    std::string m_token;
    cond::Time_t m_globalSince;
    cond::Time_t m_globalTill;
    bool m_isActive;
    cond::TypedRef<cond::IOV> m_iov;
  };
}//ns cond
#endif
