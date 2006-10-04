#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "IOVEditorImpl.h"
#include "IOV.h"

cond::IOVEditorImpl::IOVEditorImpl( DBSession& session,
				    const std::string& token, 
				    size_t cominterval ):cond::IOVEditor(token,cominterval),m_session(session),m_iov(0),m_counter(0){
  this->init();
}
void cond::IOVEditorImpl::init(){
  m_session.startUpdateTransaction();
  if(m_token.empty()){
    m_iov=new cond::IOV;
    cond::Ref<cond::IOV> myref(m_session,m_iov);
    m_token=myref.token();
  }else{
    m_session.startReadOnlyTransaction();
    cond::Ref<cond::IOV> myref(m_session, m_token); 
    m_iov=myref.ptr();
    m_session.commit();
  }
}
cond::IOVEditorImpl::~IOVEditorImpl(){
  this->flush();
}
void cond::IOVEditorImpl::insert( const std::string& payloadToken, 
	     unsigned long long tillTime ){
   m_iov->iov.insert(std::make_pair<unsigned long long, std::string>(tillTime, payloadToken));
   ++m_counter;
   if(m_counter>=m_cominterval){
     m_session.startUpdateTransaction();
     cond::Ref<cond::IOV> myref(m_session, m_token);
     myref.markWrite(cond::IOVNames::container());
     m_session.commit();
     m_counter=0;
   }
}
void cond::IOVEditorImpl::updateClosure( unsigned long long newtillTime ){
  unsigned long long closeIOV=m_iov->iov.rbegin()->first;
  std::string closePayload=m_iov->iov.rbegin()->second;
  m_iov->iov.insert( std::make_pair(newtillTime,closePayload) );
  m_iov->iov.erase( m_iov->iov.find(closeIOV) );
  m_session.startUpdateTransaction();
  cond::Ref<cond::IOV> myref(m_session, m_token);
  myref.markUpdate();
  m_session.commit();
}
void cond::IOVEditorImpl::deleteEntries(){
  m_session.startUpdateTransaction();
  cond::Ref<cond::IOV> myref(m_session, m_token);
  myref.markDelete();
  m_session.commit();
}
void cond::IOVEditorImpl::flush(){
  m_session.commit();
  m_counter=0;
}
