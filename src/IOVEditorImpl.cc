#include "CondCore/DBCommon/interface/GenericRef.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
#include "POOLCore/Token.h"
#include "StorageSvc/DbReflex.h"

namespace cond {

  IOVEditorImpl::IOVEditorImpl( cond::PoolTransaction& pooldb,
				const std::string& token,
				cond::Time_t globalSince, 
				cond::Time_t globalTill
				):m_pooldb(&pooldb),m_token(token),
				  m_globalSince(globalSince), m_globalTill(globalTill),
				  m_isActive(false){
  }
  
  void IOVEditorImpl::create( cond::Time_t firstSince,
			      cond::TimeType timetype) {

    if(!m_token.empty()){
      // problem??
      throw cond::Exception("cond::IOVEditorImpl::create cannot create a IOV using an initialized Editor");
    }

    m_iov=cond::TypedRef<cond::IOV>(*m_pooldb,new cond::IOV);
    m_iov->timetype=(int)timetype;
    m_iov->firstsince=firstSince;
    
    m_iov.markWrite(cond::IOVNames::container());
    m_token=m_iov.token();
    m_isActive=true;
 
  }
 



  void IOVEditorImpl::init(){
    if(m_token.empty()){
      // problem?
      throw cond::Exception("cond::IOVEditorImpl::init cannot init w/o token change");
      
    }
    
    m_iov=cond::TypedRef<cond::IOV>(*m_pooldb, m_token); 
    m_isActive=true;
    
}



  IOVEditorImpl::~IOVEditorImpl(){
  }
  
  Time_t IOVEditorImpl::firstSince() const {
    return m_iov->firstsince;
  }
  
  TimeType IOVEditorImpl::timetype() const {
    return (TimeType)(m_iov->timetype);

  }
  
  
  unsigned int
  IOVEditorImpl::insert( cond::Time_t tillTime,
			 const std::string& payloadToken
			 ){
    if(!m_isActive) this->init();
    
    //fix me: throw if beyond global range! 
    m_iov.markUpdate();
    return m_iov->add(tillTime, payloadToken);
    
  }
  
  void 
  IOVEditorImpl::bulkInsert(std::vector< std::pair<cond::Time_t,std::string> >& values){
    if(!m_isActive) this->init();
    m_iov.markUpdate();   
    m_iov->iov.insert(m_iov->iov.end(), values.begin(),values.end());
  }


  void 
  IOVEditorImpl::updateClosure( cond::Time_t newtillTime ){
    if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::updateClosure cannot change non-existing IOV index");
    if(!m_isActive) this->init();
    m_iov->iov.rbegin()->first=newtillTime;
    m_iov.markUpdate();
  }
  
  unsigned int 
  cond::IOVEditorImpl::append(  cond::Time_t sinceTime ,
				const std::string& payloadToken
				){
    if( m_token.empty() ) {
      throw cond::Exception("cond::IOVEditorImpl::appendIOV cannot append to non-existing IOV index");
    }
    
    if(!m_isActive) {
      this->init();
    }
    
    if( m_iov->iov.size()==0 ) throw cond::Exception("cond::IOVEditorImpl::appendIOV cannot append to empty IOV index");
    
    if (sinceTime<=firstSince())  throw cond::Exception("IOVEditor::append Error: since time out of range, below first since");
    
    
    if(  m_iov->iov.size()>1 ){
      //range check in case 
      cond::Time_t lastValidTill=(--m_iov->iov.rbegin())->first;
      //std::cout<<"lastValidTill "<<lastValidTill<<std::endl;
      if( (sinceTime-1)<= lastValidTill){
	throw cond::Exception("IOVEditor::append Error: since time out of range: below last since");
      }
    }

    std::string lastIOV=m_iov->iov.back().first;
    m_iov.markUpdate();
    m_iov->iov.back().first = sinceTime-1;
    return m_iov->add(lastIOV,payloadToken);


  }

  void 
  IOVEditorImpl::deleteEntries(bool withPayload){
    if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::deleteEntries cannot delete to non-existing IOV index");
    if(!m_isActive) this->init();
    if(withPayload){
      std::string tokenStr;
      IOV::iterator payloadIt;
      IOV::iterator payloadItEnd=m_iov->iov.end();
      for(payloadIt=m_iov->iov.begin();payloadIt!=payloadItEnd;++payloadIt){
	tokenStr=payloadIt->second;
	pool::Token token;
	const pool::Guid& classID=token.fromString(tokenStr).classID();
	cond::GenericRef ref(*m_pooldb,tokenStr,pool::DbReflex::forGuid(classID).TypeInfo());
	ref.markDelete();
	ref.reset();
      }
    }
    m_iov.markDelete();
  }

  void 
  IOVEditorImpl::import( const std::string& sourceIOVtoken ){
    if( !m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::import IOV index already exists, cannot import");
    m_iov=cond::TypedRef<cond::IOV>(*m_pooldb,sourceIOVtoken);
    m_iov.markWrite(cond::IOVNames::container());
    m_token=m_iov.token();
  }
  

}
