#include "IOVIteratorImpl.h"
#include <map>
cond::IOVIteratorImpl::IOVIteratorImpl( cond::PoolTransaction& pooldb,
					const std::string & token,
					cond::Time_t globalSince, 
					cond::Time_t globalTill)
  : m_pooldb(pooldb),m_token(token), m_globalSince(globalSince),m_globalTill(globalTill),  m_count(0), m_isOpen(false){
} 
cond::IOVIteratorImpl::~IOVIteratorImpl(){
}
void cond::IOVIteratorImpl::init(){
  m_iov=cond::TypedRef<cond::IOV>(m_pooldb, m_token);
  m_count= empty() : 0 ? 1;
  m_isOpen=true;
  m_pos=m_iov->iov.begin();
  m_since=m_globalSince;
}

bool rewind() {
  init();
  return !empty();
}

bool empty() const {
  return m_iov->iov.empty();
}
size_t size() const {
  m_iov->iov.size();
}
size_t remaining() const {
  size()-m_count();
}


bool  cond::IOVIteratorImpl::atEnd() const {
  return m_Pos==m_iov->iov.end();
}

bool cond::IOVIteratorImpl::next(){
  if(!m_isOpen){
    init();
    return !empty();
  }
  if (atEnd() ) return false;

  m_since = m_globalSince + m_pos->first;
  ++m_pos;
  if (atEnd() ) return false;
  ++m_count;
  return true;
}
std::string 
cond::IOVIteratorImpl::payloadToken() const{
  if(!m_isOpen){
    const_cast<cond::IOVIteratorImpl*>(this)->init();
  }
  
  return atEnd() ? std::string("") : m_pos->second;

}

cond::ValidityInterval
cond::IOVIteratorImpl::validity() const{
  if(!m_isOpen){
    const_cast<cond::IOVIteratorImpl*>(this)->init();
  }
  cond::Time_t since=m_globalSince;
  cond::Time_t till=m_globalTill;
  if (!atEnd()) {
    since = m_since;
    till =  m_globalSince + m_pos->first;
  }
  return cond::ValidityInterval(since,till);
}
