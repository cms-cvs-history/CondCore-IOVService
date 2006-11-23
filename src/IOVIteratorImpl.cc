#include "IOVIteratorImpl.h"
#include "IOV.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include <map>
#include <algorithm>
cond::IOVIteratorImpl::IOVIteratorImpl( cond::PoolStorageManager& pooldb,
					const std::string token)
  : IOVIterator(pooldb,token), m_currentPos(0), m_stop(0), m_isOpen(false){
} 
cond::IOVIteratorImpl::~IOVIteratorImpl(){
}
void cond::IOVIteratorImpl::refresh(){
  if(!m_isOpen){
    m_iov=cond::Ref<cond::IOV>(m_pooldb, m_token);
    m_isOpen=true;
  }
  m_iov.reset();
  m_currentPos=0;
  m_stop=(m_iov->iov.size())-1;
}
bool cond::IOVIteratorImpl::next(){
  if(!m_isOpen){
    m_iov=cond::Ref<cond::IOV>(m_pooldb, m_token);
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
  for( std::map<cond::Time_t, std::string>::const_iterator it=m_iov->iov.begin(); it!=m_iov->iov.end(); ++it,++pos ){
    if(m_currentPos==pos){
      return it->second;
    }
  }
  return "";
}
std::pair<cond::Time_t, cond::Time_t> cond::IOVIteratorImpl::validity() const{
  size_t pos=1;
  cond::Time_t since=0;
  cond::Time_t till=0;
  std::map<cond::Time_t, std::string>::iterator itbeg=m_iov->iov.begin();
  for(std::map<cond::Time_t, std::string>::iterator it=itbeg;
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
  return std::make_pair<cond::Time_t, cond::Time_t>(since,till);
}
bool cond::IOVIteratorImpl::isValid( cond::Time_t time ) const{
  if(  time <= m_iov->iov.rbegin()->first ) return true;
  return false;
}
