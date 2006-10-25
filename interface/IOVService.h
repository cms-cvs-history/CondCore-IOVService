#ifndef CondCore_IOVService_IOVService_h
#define CondCore_IOVService_IOVService_h
#include <string>
namespace cond{
  class DBSession;
  class IOVIterator;
  class IOVEditor;
  class IOVService{
  public:
    explicit IOVService( DBSession& session );
    virtual ~IOVService();
    //iterator interface
    //user has the ownership of the iterator, should explicitly delete
    IOVIterator* newIOVIterator( const std::string& token );
    //editor interface
    //user has the ownership of the editor, should explicitly delete
    IOVEditor* newIOVEditor( const std::string& token );
    IOVEditor* newIOVEditor();
    std::string payloadContainerName( const std::string& token );
    //clean up the entire IOV container
    void deleteAll();
  private:
    cond::DBSession& m_session;
  } ;
}//ns cond
#endif
