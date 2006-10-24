#include "IOVIteratorImpl.h"
#include "IOV.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include <map>
#include <algorithm>
cond::IOVIteratorImpl::IOVIteratorImpl( cond::DBSession& session,
					const std::string token)
  : IOVIterator(session,token), m_currentPos(0), m_stop(0), m_isOpen(false){
} 
cond::IOVIteratorImpl::~IOVIteratorImpl(){
}
void cond::IOVIteratorImpl::refresh(){
  if(!m_isOpen){
    m_iov=cond::Ref<cond::IOV>(m_session, m_token);
    m_isOpen=true;
  }
  m_iov.reset();
  m_currentPos=0;
  m_stop=(m_iov->iov.size())-1;
}
bool cond::IOVIteratorImpl::next(){
  if(!m_isOpen){
    m_iov=cond::Ref<cond::IOV>(m_session, m_token);
    m_stop=(m_iov->iov.size())-1;
    m_isOpen=true;
  }
  if(m_currentPos>m_stop){
    return false;
  }
  ++m_currentPos;
  return true;
}
std::string cond::IOVIteratorImpl::payloadToken() const{
  size_t pos=1;
  for( std::map<unsigned long long, std::string>::const_iterator it=m_iov->iov.begin(); it!=m_iov->iov.end(); ++it,++pos ){
    if(m_currentPos==pos){
      return it->second;
    }
  }
  return "";
}
std::pair<unsigned long long, unsigned long long> cond::IOVIteratorImpl::validity() const{
  size_t pos=1;
  unsigned long long since=0;
  unsigned long long till=0;
  std::map<unsigned long long, std::string>::iterator itbeg=m_iov->iov.begin();
  for(std::map<unsigned long long, std::string>::iterator it=itbeg;
      it!=m_iov->iov.end();++it,++pos){
    if(pos==m_currentPos){
      till=it->first;
      if(m_currentPos != 1 ){
	--it;
	since=(it->first)+1;
	++it;
      }
    }
  }
  return std::make_pair<unsigned long long, unsigned long long>(since,till);
}
bool cond::IOVIteratorImpl::isValid( unsigned long long time ) const{
  if(  time <= m_iov->iov.rbegin()->first ) return true;
  return false;
}
