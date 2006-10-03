#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "IOVIteratorImpl.h"
cond::IOVService::IOVService( DBSession& session ): m_session(session){
}
cond::IOVService::~IOVService(){
}
cond::IOVIterator* cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_session, token );
}
void cond::IOVService::deleteAllIndices(){
  //use implicit collection to delete all in cond::IOV container
}
