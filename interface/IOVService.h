#ifndef CondCore_IOVService_IOVService_h
#define CondCore_IOVService_IOVService_h
#include <string>
#include "CondCore/DBCommon/interface/Time.h"
#include "CondCore/DBCommon/interface/DbSession.h"
namespace cond{
  class IOVServiceImpl;
  class IOVIterator;
  class IOVEditor;
  class IOVService{
  public:
    static const bool forwardIter=true;
    static const bool backwardIter=!forwardIter;


    IOVService( cond::DbSession& pooldb);

    virtual ~IOVService();

    std::string payloadToken( const std::string& iovToken,
                              cond::Time_t currenttime );

    bool isValid( const std::string& iovToken,
                  cond::Time_t currenttime );

    std::pair<cond::Time_t, cond::Time_t>
      validity( const std::string& iovToken, cond::Time_t currenttime );

    std::string payloadContainerName( const std::string& iovtoken );

    void loadDicts( const std::string& iovToken);

    void deleteAll( bool withPayload=false );

    /**
    create an iterator to on the iov selected by the token
    user aquires the ownership of the pointer. Need explicit delete after usage
    */
    IOVIterator* newIOVIterator( const std::string& iovToken,  bool forward=forwardIter);

    /**
    create an editor to the iov selected by the token
    user aquires the ownership of the pointer. Need explicit delete after usage
    */
    IOVEditor* newIOVEditor( const std::string& token="" );

   /**
       export IOV selected by token and associated payload to another database
       return new iov token string 
    */

    std::string exportIOVWithPayload( cond::DbSession& destDB,
                                      const std::string& iovToken );
    /**
       export IOV selected by token within selected range and associated 
       payload to another database
       return new iov token string 
    */
    std::string exportIOVRangeWithPayload( cond::DbSession& destDB,
                                           const std::string& iovToken,
                                           const std::string& destToken,
                                           cond::Time_t since,
                                           cond::Time_t till,
                                           bool outOfOrder);
  private:
    cond::DbSession m_pooldb;
    cond::IOVServiceImpl* m_impl;
  };

}//ns cond
#endif
