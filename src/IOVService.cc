#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
cond::IOVService::IOVService( DBSession& session ): m_session(session){
}
cond::IOVService::~IOVService(){
}
cond::IOVIterator* cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_session, token );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( const std::string& token ){
  return new cond::IOVEditorImpl( m_session, token );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( ){
  return new cond::IOVEditorImpl( m_session, "");
}
std::string cond::IOVService::payloadContainerName( const std::string& token ){
  cond::Ref<cond::IOV> iov(m_session,token);
  std::string payloadtokstr=iov->iov.begin()->second;
  pool::Token* theTok = new pool::Token;
  theTok->fromString(payloadtokstr);
  std::string result=theTok->contID();
  theTok->release();
  return result;
}
void cond::IOVService::deleteAll(){
  cond::ContainerIterator<cond::IOV> it(m_session,cond::IOVNames::container());
  while ( it.next() ) {
    it.dataRef().markDelete();
  }
}
