#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "IOVServiceImpl.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
cond::IOVService::IOVService( cond::PoolStorageManager& pooldb ):
  m_pooldb(pooldb),
  m_impl(new cond::IOVServiceImpl(pooldb)){
}
cond::IOVService::~IOVService(){
  delete m_impl;
}
std::string 
cond::IOVService::payloadToken( const std::string& iovToken,
				cond::Time_t currenttime ) const{
  return m_impl->payloadToken(iovToken, currenttime);
}
bool 
cond::IOVService::isValid( const std::string& iovToken,
			   cond::Time_t currenttime ) const{
  return m_impl->isValid(iovToken,currenttime);
}
std::pair<cond::Time_t, cond::Time_t> 
cond::IOVService::validity( const std::string& iovToken, cond::Time_t currenttime ){
  return m_impl->validity(iovToken,currenttime);
}
std::string 
cond::IOVService::payloadContainerName( const std::string& token ){
  return m_impl->payloadContainerName(token);
}
void 
cond::IOVService::deleteAll(){
  m_impl->deleteAll();
}
cond::IOVIterator* 
cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_pooldb,token );
}
cond::IOVEditor* 
cond::IOVService::newIOVEditor( const std::string& token ){
  return new cond::IOVEditorImpl( m_pooldb,token );
}
cond::IOVEditor* 
cond::IOVService::newIOVEditor( ){
  return new cond::IOVEditorImpl( m_pooldb, "");
}
