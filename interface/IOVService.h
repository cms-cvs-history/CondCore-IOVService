#ifndef CondCore_IOVService_IOVService_h
#define CondCore_IOVService_IOVService_h
#include <string>
namespace cond{
  class DBSession;
  class IOVIterator;
  class IOVService{
  public:
    explicit IOVService( DBSession* session );
    virtual ~IOVService();
    IOVIterator* newIOVIterator( const std::string& token );
    void deleteAllIndices();
  private:
    DBSession* m_session;
  } ;
}//ns cond
#endif
