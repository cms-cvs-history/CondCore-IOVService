#ifndef CondCore_IOVService_IOVService_h
#define CondCore_IOVService_IOVService_h
#include <string>
#include "CondCore/DBCommon/interface/Time.h"
namespace cond{
  class PoolStorageManager;
  class IOVServiceImpl;
  class IOVIterator;
  class IOVEditor;
  class IOVService{
  public:
    explicit IOVService( cond::PoolStorageManager& pooldb );
    virtual ~IOVService();
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
    cond::IOVServiceImpl* m_impl;
  };
}//ns cond
#endif
