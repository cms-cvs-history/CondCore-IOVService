#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include "IOVServiceImpl.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
cond::IOVServiceImpl::IOVServiceImpl( cond::PoolStorageManager& pooldb ): m_pooldb(pooldb){
}
cond::IOVServiceImpl::~IOVServiceImpl(){
}
std::string 
cond::IOVServiceImpl::payloadToken( const std::string& iovToken,
				    cond::Time_t currenttime ) const{
  //m_pooldb.startTransaction(cond::ReadOnly);
  cond::Ref<cond::IOV> iov(m_pooldb,iovToken);
  std::map<cond::Time_t, std::string>::const_iterator it=iov->iov.lower_bound(currenttime);
  //m_pooldb.commit();
  if( it==iov->iov.end() ){
    return "";
  }else{
    return it->second;
  }
}
bool cond::IOVServiceImpl::isValid( const std::string& iovToken,
				    cond::Time_t currenttime ) const{
  //m_pooldb.startTransaction(cond::ReadOnly);
  cond::Ref<cond::IOV> iov(m_pooldb,iovToken);
  bool result;
  if(  currenttime <= iov->iov.rbegin()->first ){
    result=true;
  }else{
    result=false;
  }
  //m_pooldb.commit();
  return result;
}
std::pair<cond::Time_t, cond::Time_t> 
cond::IOVServiceImpl::validity( const std::string& iovToken, cond::Time_t currenttime ){
  //m_pooldb.startTransaction(cond::ReadOnly);
  cond::Ref<cond::IOV> iov(m_pooldb,iovToken);
  cond::Time_t since=0;
  cond::Time_t till=0;
  std::map<cond::Time_t, std::string>::iterator it=iov->iov.lower_bound(currenttime);
  //m_pooldb.commit();
  if( it!=iov->iov.begin() ){
    --it;
    since=(it->first)+1;
    ++it;
  }
  till=it->first;
  return std::make_pair<cond::Time_t, cond::Time_t>(since,till);
}
std::string 
cond::IOVServiceImpl::payloadContainerName( const std::string& token ){
  //m_pooldb.startTransaction(cond::ReadOnly);
  cond::Ref<cond::IOV> iov(m_pooldb,token);
  std::string payloadtokstr=iov->iov.begin()->second;
  //m_pooldb.commit();
  pool::Token* theTok = new pool::Token;
  theTok->fromString(payloadtokstr);
  std::string result=theTok->contID();
  theTok->release();
  return result;
}
void 
cond::IOVServiceImpl::deleteAll(){
  //m_pooldb.startTransaction(cond::ReadWriteCreate);
  cond::ContainerIterator<cond::IOV> it(m_pooldb,cond::IOVNames::container());
  while ( it.next() ) {
    it.dataRef().markDelete();
  }
  //m_pooldb.commit();
}
cond::IOVIterator* 
cond::IOVServiceImpl::newIOVIterator( const std::string& token ){
  return new cond::IOVIteratorImpl( m_pooldb, token );
}
cond::IOVEditor* 
cond::IOVServiceImpl::newIOVEditor( const std::string& token ){
  return new cond::IOVEditorImpl( m_pooldb, token );
}
cond::IOVEditor* 
cond::IOVServiceImpl::newIOVEditor( ){
  return new cond::IOVEditorImpl( m_pooldb, "");
}
