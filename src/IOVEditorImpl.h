#ifndef CondCore_IOVService_IOVEditorImpl_h
#define CondCore_IOVService_IOVEditorImpl_h
#include <string>
#include "CondCore/IOVService/interface/IOVEditor.h"
namespace cond{
  class IOV;
  class DBSession;
  class IOVEditorImpl : virtual public IOVEditor{
  public:
    IOVEditorImpl( DBSession& session,
		   const std::string& token);
    virtual ~IOVEditorImpl();
    virtual void insert( const std::string& payloadToken, 
			 unsigned long long tillTime );
    virtual void updateClosure( unsigned long long newtillTime );
    virtual void append( const std::string& payloadToken, 
			    unsigned long long sinceTime );
    virtual void deleteEntries();
  private:
    void init();
    DBSession& m_session;
    cond::Ref<cond::IOV> m_iov;
    bool m_isActive;
  } ;
}//ns cond
#endif
