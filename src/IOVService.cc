#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVNames.h"
//#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
cond::IOVService::IOVService( cond::PoolStorageManager& pooldb ): m_pooldb(pooldb){
}
cond::IOVService::~IOVService(){
}
cond::IOVIterator* cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_pooldb, token );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( const std::string& token ){
  return new cond::IOVEditorImpl( m_pooldb, token );
}
cond::IOVEditor* cond::IOVService::newIOVEditor( ){
  return new cond::IOVEditorImpl( m_pooldb, "");
}
std::string cond::IOVService::payloadContainerName( const std::string& token ){
  cond::Ref<cond::IOV> iov(m_pooldb,token);
  std::string payloadtokstr=iov->iov.begin()->second;
  pool::Token* theTok = new pool::Token;
  theTok->fromString(payloadtokstr);
  std::string result=theTok->contID();
  theTok->release();
  return result;
}
void cond::IOVService::deleteAll(){
  cond::ContainerIterator<cond::IOV> it(m_pooldb,cond::IOVNames::container());
  while ( it.next() ) {
    it.dataRef().markDelete();
  }
}
