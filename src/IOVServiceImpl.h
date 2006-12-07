#ifndef CondCore_IOVService_IOVServiceImpl_h
#define CondCore_IOVService_IOVServiceImpl_h
#include <string>
namespace cond{
  class PoolStorageManager;
  class IOVIterator;
  class IOVEditor;
  class IOVServiceImpl{
  public:
    explicit IOVServiceImpl( cond::PoolStorageManager& pooldb );
    ~IOVServiceImpl();
    std::string payloadToken( const std::string& iovToken,
			      cond::Time_t currenttime ) const;
    bool isValid( const std::string& iovToken,
		  cond::Time_t currenttime ) const;
    std::pair<cond::Time_t, cond::Time_t> 
      validity( const std::string& iovToken, cond::Time_t currenttime );
    std::string payloadContainerName( const std::string& iovtoken );
    void deleteAll();
    IOVIterator* newIOVIterator( const std::string& iovToken );
    IOVEditor* newIOVEditor( const std::string& token );
    IOVEditor* newIOVEditor();
  private:
    cond::PoolStorageManager& m_pooldb;
  };
}//ns cond
#endif
