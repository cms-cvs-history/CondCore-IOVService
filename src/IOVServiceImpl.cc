#include "FWCore/Framework/interface/IOVSyncValue.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include "CondCore/DBCommon/interface/GenericRef.h"
#include "IOVServiceImpl.h"
#include "IOVIteratorImpl.h"
#include "IOVEditorImpl.h"
#include "POOLCore/Token.h"

cond::IOVServiceImpl::IOVServiceImpl( cond::PoolTransaction& pooldb ,
				      cond::TimeType timetype ): 
  m_pooldb(&pooldb), m_timetype(timetype){
  switch (m_timetype) {
  case cond::runnumber:
    m_beginOftime=(cond::Time_t)edm::IOVSyncValue::beginOfTime().eventID().run();
    m_endOftime=(cond::Time_t)edm::IOVSyncValue::endOfTime().eventID().run();
    break;
  case cond::timestamp:
    m_beginOftime=(cond::Time_t)edm::IOVSyncValue::beginOfTime().eventID().run();
    m_endOftime=(cond::Time_t)edm::IOVSyncValue::endOfTime().eventID().run();
    break;
  default:
    m_beginOftime=(cond::Time_t)edm::IOVSyncValue::beginOfTime().eventID().run();
    m_endOftime=(cond::Time_t)edm::IOVSyncValue::endOfTime().eventID().run();
    break;
  }
}
cond::IOVServiceImpl::~IOVServiceImpl(){
}
std::string 
cond::IOVServiceImpl::payloadToken( const std::string& iovToken,
				    cond::Time_t currenttime ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  std::map<cond::Time_t, std::string>::const_iterator iEnd=iov->iov.lower_bound(currenttime);
  if( iEnd==iov->iov.end() ){
    return "";
  }else{
    return iEnd->second;
  }
}
bool cond::IOVServiceImpl::isValid( const std::string& iovToken,
				    cond::Time_t currenttime ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  bool result;
  if(  currenttime <= iov->iov.rbegin()->first ){
    result=true;
  }else{
    result=false;
  }
  return result;
}
std::pair<cond::Time_t, cond::Time_t> 
cond::IOVServiceImpl::validity( const std::string& iovToken, cond::Time_t currenttime ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  cond::Time_t since=m_beginOftime;
  cond::Time_t till=m_endOftime;
  std::map<cond::Time_t, std::string>::iterator iEnd=iov->iov.lower_bound(currenttime);
  if( iEnd!=iov->iov.begin() ){
    std::map<cond::Time_t, std::string>::iterator iStart(iEnd); 
    iStart--;
    since=iStart->first+m_beginOftime;
  }
  till=iEnd->first;
  return std::make_pair<cond::Time_t, cond::Time_t>(since,till);
}
std::string 
cond::IOVServiceImpl::payloadContainerName( const std::string& iovToken ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  std::string payloadtokstr=iov->iov.begin()->second;
  pool::Token* theTok = new pool::Token;
  theTok->fromString(payloadtokstr);
  std::string result=theTok->contID();
  theTok->release();
  return result;
}
void 
cond::IOVServiceImpl::deleteAll(bool withPayload){
  cond::ContainerIterator<cond::IOV> it(*m_pooldb,cond::IOVNames::container());
  while ( it.next() ) {
    if(withPayload){
      std::string tokenStr;
      std::map<cond::Time_t,std::string>::iterator payloadIt;
      std::map<cond::Time_t,std::string>::iterator payloadItEnd=it.dataRef()->iov.end();
      for(payloadIt=it.dataRef()->iov.begin();payloadIt!=payloadItEnd;++payloadIt){
	tokenStr=payloadIt->second;
	pool::Token token;
	const pool::Guid& classID=token.fromString(tokenStr).classID();
	cond::GenericRef ref(*m_pooldb,tokenStr,pool::DbReflex::forGuid(classID).TypeInfo());
	ref.markDelete();
	ref.reset();
      }
    }
    it.dataRef().markDelete();
  }
}
cond::IOVIterator* 
cond::IOVServiceImpl::newIOVIterator( const std::string& iovToken ){
  return new cond::IOVIteratorImpl(*m_pooldb, iovToken, m_beginOftime, m_endOftime );
}
cond::IOVEditor* 
cond::IOVServiceImpl::newIOVEditor( const std::string& iovToken ){
  return new cond::IOVEditorImpl(*m_pooldb, iovToken, m_beginOftime, m_endOftime );
}
cond::IOVEditor* 
cond::IOVServiceImpl::newIOVEditor( ){
  return new cond::IOVEditorImpl(*m_pooldb, "", m_beginOftime, m_endOftime );
}
cond::TimeType 
cond::IOVServiceImpl::timeType() const{
  return m_timetype;
}
cond::Time_t 
cond::IOVServiceImpl::globalSince() const{
  return m_beginOftime;
}
cond::Time_t 
cond::IOVServiceImpl::globalTill() const{
  return m_endOftime;
}
std::string
cond::IOVServiceImpl::exportIOVWithPayload( cond::PoolTransaction& destDB,
					    const std::string& iovToken,
					    const std::string& payloadObjectName ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  cond::IOV* newiov=new cond::IOV;
  for( std::map<cond::Time_t,std::string>::iterator it=iov->iov.begin();
       it!=iov->iov.end(); ++it){
    cond::GenericRef payloadRef(*m_pooldb,it->second,payloadObjectName);
    std::string newPToken=payloadRef.exportTo(destDB);
    newiov->iov.insert(std::make_pair<cond::Time_t,std::string>(it->first,newPToken));
  }
  cond::TypedRef<cond::IOV> newiovref(destDB,newiov);
  newiovref.markWrite(cond::IOVNames::container());
  return newiovref.token();
}

std::string 
cond::IOVServiceImpl::exportIOVRangeWithPayload( cond::PoolTransaction& destDB,
						 const std::string& iovToken,
						 const std::string& destToken,
						 cond::Time_t since,
						 cond::Time_t till,
						 const std::string& payloadObjectName ){
  std::map< std::string,cond::TypedRef<cond::IOV> >::iterator it=m_iovcache.find(iovToken);
  if(it==m_iovcache.end()){
    m_iovcache.insert(std::make_pair< std::string,cond::TypedRef<cond::IOV> >(iovToken,cond::TypedRef<cond::IOV>(*m_pooldb,iovToken)));
  }
  cond::TypedRef<cond::IOV> iov=m_iovcache[iovToken];
  std::map<cond::Time_t, std::string>::const_iterator ifirstTill=iov->iov.lower_bound(since);
  std::map<cond::Time_t, std::string>::const_iterator isecondTill=iov->iov.lower_bound(till);
  if( isecondTill!=iov->iov.end() ){
    isecondTill++;
  }
  cond::TypedRef<cond::IOV> newiovref;
  if (destToken.empty()) {
    // create a new one 
    newiovref = cond::TypedRef<cond::IOV>(destDB,new cond::IOV);
    newiovref.markWrite(cond::IOVNames::container());
  } else {
    newiovref = cond::TypedRef<cond::IOV>(destDB,destToken);
    newiovref.markUpdate(cond::IOVNames::container());   
  }
  cond::IOV & newiov = *newiovref;
  for( std::map<cond::Time_t,std::string>::const_iterator it=ifirstTill;
       it!=isecondTill; ++it){
    cond::GenericRef payloadRef(*m_pooldb,it->second,payloadObjectName);
    std::string newPtoken=payloadRef.exportTo(destDB);
    newiov.iov.insert(std::make_pair<cond::Time_t,std::string>(it->first,newPtoken));
  }  
  return newiovref.token();
}
