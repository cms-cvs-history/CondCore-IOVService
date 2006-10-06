#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "IOVEditorImpl.h"
#include "IOV.h"

cond::IOVEditorImpl::IOVEditorImpl( DBSession& session,
				    const std::string& token, 
				    size_t cominterval ):cond::IOVEditor(token,cominterval),m_session(session),m_counter(0){
  this->init();
}
void cond::IOVEditorImpl::init(){
  if(!m_token.empty()){
    try{
      m_session.startReadOnlyTransaction();
      m_iov=cond::Ref<cond::IOV>(m_session, m_token); 
      m_session.commit();
    }catch( const cond::RefException& er ){
      std::cout<<er.what()<<std::endl;
    }
  }
}
cond::IOVEditorImpl::~IOVEditorImpl(){
  this->flush();
}
void cond::IOVEditorImpl::insert( const std::string& payloadToken, 
				  unsigned long long tillTime ){
  try{
    m_session.startUpdateTransaction();
    if(m_token.empty()){
      cond::IOV* myiov=new cond::IOV;
      myiov->iov.insert(std::make_pair<unsigned long long, std::string>(tillTime, payloadToken));
      m_iov=cond::Ref<cond::IOV>(m_session,myiov);
      m_iov.markWrite(cond::IOVNames::container());
      m_token=m_iov.token();
    }else{
      m_iov->iov.insert(std::make_pair<unsigned long long, std::string>(tillTime, payloadToken));
      ++m_counter;
      m_iov.markUpdate();
    }
    ++m_counter;
    if(m_counter>=m_cominterval){
      m_session.commit();
      m_counter=0;
    }
  }catch( const cond::RefException& er ){
    std::cout<<er.what()<<std::endl;
  }
}
void cond::IOVEditorImpl::updateClosure( unsigned long long newtillTime ){
  if( !m_token.empty() ){
    try{
      unsigned long long closeIOV=m_iov->iov.rbegin()->first;
      std::string closePayload=m_iov->iov.rbegin()->second;
      m_iov->iov.insert( std::make_pair(newtillTime,closePayload) );
      m_iov->iov.erase( m_iov->iov.find(closeIOV) );
      m_session.startUpdateTransaction();
      m_iov.markUpdate();
      m_session.commit();
    }catch( const cond::RefException& er ){
      std::cout<<er.what()<<std::endl;
    }
  }
}
void cond::IOVEditorImpl::deleteEntries(){
  if( !m_token.empty() ){  
    try{
      m_session.startUpdateTransaction();
      m_iov.markDelete();
      m_session.commit();
    }catch( const cond::RefException& er ){
      std::cout<<er.what()<<std::endl;
    }
  }
}
void cond::IOVEditorImpl::flush(){
  m_session.commit();
  m_counter=0;
}
