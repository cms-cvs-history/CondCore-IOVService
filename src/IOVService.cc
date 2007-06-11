#include "CondCore/IOVService/interface/IOVService.h"
//#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "IOVServiceImpl.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
cond::IOVService::IOVService( cond::PoolStorageManager& pooldb,cond::TimeType timetype ):
  m_pooldb(pooldb),
  m_impl(new cond::IOVServiceImpl(pooldb,timetype)){
}
cond::IOVService::~IOVService(){
  delete m_impl;
}
std::string 
cond::IOVService::payloadToken( const std::string& iovToken,
				cond::Time_t currenttime ){
  return m_impl->payloadToken(iovToken, currenttime);
}
bool 
cond::IOVService::isValid( const std::string& iovToken,
			   cond::Time_t currenttime ){
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
cond::IOVService::deleteAll( bool withPayload ){
  m_impl->deleteAll( withPayload );
}
cond::IOVIterator* 
cond::IOVService::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_pooldb,token,m_impl->globalSince(),m_impl->globalTill());
}
cond::IOVEditor* 
cond::IOVService::newIOVEditor( const std::string& token ){
  return new cond::IOVEditorImpl( m_pooldb,token,m_impl->globalSince(),m_impl->globalTill());
}
cond::IOVEditor* 
cond::IOVService::newIOVEditor( ){
  return new cond::IOVEditorImpl( m_pooldb,"",m_impl->globalSince(),m_impl->globalTill());
}
cond::TimeType 
cond::IOVService::timeType() const{
  return m_impl->timeType();
}
cond::Time_t 
cond::IOVService::globalSince() const{
  return m_impl->globalSince();
}
cond::Time_t 
cond::IOVService::globalTill() const{
  return m_impl->globalTill();
}
std::string 
cond::IOVService::exportIOVWithPayload( cond::PoolStorageManager& destDB,
					const std::string& iovToken,
					const std::string& payloadObjectName ){
  return m_impl->exportIOVWithPayload( destDB,
				       iovToken,
				payloadObjectName); 
}
std::string
cond::IOVService::exportIOVRangeWithPayload( cond::PoolStorageManager& destDB,
					     const std::string& iovToken,
					     cond::Time_t since,
					     cond::Time_t till,
					     const std::string& payloadObjectName ){
  return  m_impl->exportIOVRangeWithPayload( destDB,
					     iovToken,
					     since,
					     till,
					     payloadObjectName); 
}

