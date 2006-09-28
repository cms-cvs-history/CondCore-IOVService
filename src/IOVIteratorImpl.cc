#include "IOVIteratorImpl.h"
#include "CondCore/IOVService/interface/IOV.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/DBWriter.h"
#include <map>
#include <algorithm>
cond::IOVIteratorImpl::IOVIteratorImpl( cond::DBSession* sessionHandle,
					const std::string token)
  : IOVIterator(sessionHandle,token){  
} 
cond::IOVIteratorImpl::~IOVIteratorImpl(){
}
void cond::IOVIteratorImpl::open( bool isReadOnly ){
  m_isReadOnly = isReadOnly;
  if(!m_isActive) {
    if( m_isReadOnly ){
      m_session->startReadOnlyTransaction();
    }else{
      m_session->startUpdateTransaction();
    }
  }
  m_iov=pool::Ref<cond::IOV>(&(m_session->DataSvc()), m_token);
}
void cond::IOVIteratorImpl::close(){
  m_iov->iov.clear();
  if(m_isActive){
    m_session->commit();
  }
  m_isActive=false;
}
bool cond::IOVIteratorImpl::gotoNext(){
  if(m_currentPos>m_iov->iov.size()){
    return false;
  }
  ++m_currentPos;
  return true;
}
std::string cond::IOVIteratorImpl::payloadToken() const{
  size_t pos=0;
  for( std::map<unsigned long long, std::string>::const_iterator it=m_iov->iov.begin(); it!=m_iov->iov.end(); ++it,++pos ){
    if(m_currentPos==pos){
      return it->second;
    }
  }
  return "";
}
std::pair<unsigned long long, unsigned long long> cond::IOVIteratorImpl::validity() const{
  size_t pos=0;
  unsigned long long since=0;
  unsigned long long till=0;
  std::map<unsigned long long, std::string>::iterator itbeg=m_iov->iov.begin();
  for(std::map<unsigned long long, std::string>::iterator it=itbeg;
      it!=m_iov->iov.end();++it,++pos){
    if(pos==m_currentPos){
      till=it->first;
      --it;
      since=it->first;
      ++it;
    }
  }
  return std::make_pair<unsigned long long, unsigned long long>(since,till);
}
size_t cond::IOVIteratorImpl::size() const{
  return m_iov->iov.size();
}
bool cond::IOVIteratorImpl::isValid( unsigned long long time ) const{
  if(  time <= m_iov->iov.rbegin()->first ) return true;
  return false;
}
void cond::IOVIteratorImpl::insert( const std::string& payloadToken, 
				    unsigned long long tillTime ){
  m_iov->iov.insert(std::make_pair<unsigned long long, std::string>(tillTime, payloadToken));
}
void cond::IOVIteratorImpl::deleteEntries(){
}
