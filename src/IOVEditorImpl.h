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
		   const std::string& token, 
		   size_t cominterval=1 );
    virtual ~IOVEditorImpl();
    virtual void insert( const std::string& payloadToken, 
			 unsigned long long tillTime );
    virtual void updateClosure( unsigned long long newtillTime );
    virtual void deleteEntries();
    virtual void flush();
  private:
    DBSession& m_session;
    IOV* m_iov;
    size_t m_counter;
  } ;
}//ns cond
#endif
