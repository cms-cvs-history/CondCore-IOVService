#include "IOVEditorImpl.h"
#include "IOV.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Ref.h"
cond::IOVEditorImpl::IOVEditorImpl( DBSession& session,
				    const std::string& token, 
				    size_t cominterval=1 ):m_session(session),m_iov(0),m_counter(0),cond::IOVEditor(token, cominterval){
  
}
cond::IOVEditorImpl::~IOVEditorImpl(){
  this->flush();
  m_session.disconnect();
}
void cond::IOVEditorImpl::insert( const std::string& payloadToken, 
	     unsigned long long tillTime ){
   m_iov->iov.insert(std::make_pair<unsigned long long, std::string>(tillTime, payloadToken));
   ++m_counter;
   if(m_counter>=m_cominterval){
     m_session.commit();
     m_counter=0;
   }
}
void cond::IOVEditorImpl::updateClosure( unsigned long long newtillTime ){
  ++m_counter;
}
void cond::IOVEditorImpl::deleteEntries(){
}
void cond::IOVEditorImpl::flush(){
  m_session.commit();
  m_counter=0;
}
