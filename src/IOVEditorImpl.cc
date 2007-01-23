#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
cond::IOVEditorImpl::IOVEditorImpl( cond::PoolStorageManager& pooldb,
				    const std::string& token,
				    cond::Time_t globalSince, 
				    cond::Time_t globalTill
				    ):m_pooldb(pooldb),m_token(token),m_globalSince(globalSince),m_globalTill(globalTill),m_isActive(false){
}
void cond::IOVEditorImpl::init(){
  //m_pooldb.startTransaction(cond::ReadOnly);
  if(!m_token.empty()){
    m_iov=cond::Ref<cond::IOV>(m_pooldb, m_token); 
  }else{
    m_iov=cond::Ref<cond::IOV>(m_pooldb,new cond::IOV);
  }
  *m_iov;
  //m_pooldb.commit();
  m_isActive=true;
}
cond::IOVEditorImpl::~IOVEditorImpl(){
}
void cond::IOVEditorImpl::insert( cond::Time_t tillTime,
				  const std::string& payloadToken
				  ){
  if(!m_isActive) this->init();
  //fix me: throw if beyond global range!!! 
  m_iov->iov.insert(std::make_pair<cond::Time_t, std::string>(tillTime, payloadToken));
  //m_pooldb.startTransaction(cond::ReadWriteCreate);
  if(m_token.empty()){
    m_iov.markWrite(cond::IOVNames::container());
  }else{
    m_iov.markUpdate();
  }
  m_token=m_iov.token();
  //m_pooldb.commit();
}
void cond::IOVEditorImpl::bulkInsert(std::vector< std::pair<cond::Time_t,std::string> >& values){
    if(!m_isActive) this->init();
    //m_pooldb.startTransaction(cond::ReadWriteCreate);
    for(std::vector< std::pair<cond::Time_t,std::string> >::iterator it=values.begin(); it!=values.end(); ++it){
      m_iov->iov.insert(*it);
    }
    if(m_token.empty()){
      m_iov.markWrite(cond::IOVNames::container());
      m_token=m_iov.token();
    }else{
      m_iov.markUpdate();   
    }
    //m_pooldb.commit();
}
void cond::IOVEditorImpl::updateClosure( cond::Time_t newtillTime ){
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::updateClosure cannot change non-existing IOV index");
  if(!m_isActive) this->init();
  cond::Time_t closeIOV=m_iov->iov.rbegin()->first;
  std::string closePayload=m_iov->iov.rbegin()->second;
  m_iov->iov.insert( std::make_pair(newtillTime,closePayload) );
  m_iov->iov.erase( m_iov->iov.find(closeIOV) );
  // m_pooldb.startTransaction(cond::ReadWriteCreate);
  m_iov.markUpdate();
  //m_pooldb.commit();
}
void cond::IOVEditorImpl::append(  cond::Time_t sinceTime ,
				   const std::string& payloadToken
				   ){
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::appendIOV cannot append to non-existing IOV index");
  if(!m_isActive) this->init();
  if( m_iov->iov.size()==0 ) throw cond::Exception("cond::IOVEditorImpl::appendIOV cannot append to empty IOV index");
  cond::Time_t lastIOV=m_iov->iov.rbegin()->first;
  std::string lastPayload=m_iov->iov.rbegin()->second;
  m_iov->iov[lastIOV]=payloadToken;
  m_iov->iov.insert( std::make_pair((sinceTime+1),lastPayload) );
  //m_pooldb.startTransaction(cond::ReadWriteCreate);
  m_iov.markUpdate();
  //m_pooldb.commit();
}
void cond::IOVEditorImpl::deleteEntries(){
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::deleteEntries cannot delete to non-existing IOV index");
  if(!m_isActive) this->init();
  //m_pooldb.startTransaction(cond::ReadWriteCreate);
  m_iov.markDelete();
  //m_pooldb.commit();
}
void cond::IOVEditorImpl::import( const std::string& sourceIOVtoken ){
  if( !m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::import IOV index already exists, cannot import");
  //m_pooldb.startTransaction(cond::ReadWriteCreate);
  m_iov=cond::Ref<cond::IOV>(m_pooldb,sourceIOVtoken);
  m_iov.markWrite(cond::IOVNames::container());
  //m_pooldb.commit();
  m_token=m_iov.token();
}