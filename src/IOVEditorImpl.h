#ifndef CondCore_IOVService_IOVEditorImpl_h
#define CondCore_IOVService_IOVEditorImpl_h
#include <string>
#include "CondCore/IOVService/interface/IOVEditor.h"
namespace cond{
  class IOV;
  class PoolStorageManager;
  class IOVEditorImpl : virtual public IOVEditor{
  public:
    IOVEditorImpl( PoolStorageManager& pooldb,
		   const std::string& token);
    virtual ~IOVEditorImpl();
    virtual void insert( const std::string& payloadToken, 
			 cond::Time_t tillTime );
    virtual void bulkInsert( std::vector< std::pair<cond::Time_t,std::string> >& values );
    virtual void updateClosure( cond::Time_t newtillTime );
    virtual void append( const std::string& payloadToken, 
			    cond::Time_t sinceTime );
    virtual void bulkAppend( std::vector< std::pair<cond::Time_t,std::string> >& values );
    virtual void deleteEntries();
    virtual void import( const std::string& sourceIOVtoken ); 
  private:
    void init();
    PoolStorageManager& m_pooldb;
    cond::Ref<cond::IOV> m_iov;
    bool m_isActive;
  } ;
}//ns cond
#endif
