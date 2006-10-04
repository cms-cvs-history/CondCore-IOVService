#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
cond::IOVService::IOVService( DBSession& session ): m_session(session){
}
cond::IOVService::~IOVService(){
}
cond::IOVIterator* cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_session, token );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( const std::string& token,
						 size_t cominterval ){
  return new cond::IOVEditorImpl( m_session, token, cominterval );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( size_t cominterval ){
  return new cond::IOVEditorImpl( m_session, "", cominterval );
}
void cond::IOVService::deleteAll(){
  //use implicit collection to delete all in cond::IOV container
}
