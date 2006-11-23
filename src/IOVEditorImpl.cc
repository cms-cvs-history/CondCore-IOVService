#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/IOVService/interface/IOVNames.h"
#include "IOVEditorImpl.h"
#include "IOV.h"
cond::IOVEditorImpl::IOVEditorImpl( cond::PoolStorageManager& pooldb,
				    const std::string& token
				  ):cond::IOVEditor(token),m_pooldb(pooldb),m_isActive(false){
}
void cond::IOVEditorImpl::init(){
  if(m_isActive) return;
  if(!m_token.empty()){
    try{
      m_iov=cond::Ref<cond::IOV>(m_pooldb, m_token); 
    }catch( const cond::RefException& er ){
      std::cout<<er.what()<<std::endl;
    }
  }
  m_isActive=true;
}
cond::IOVEditorImpl::~IOVEditorImpl(){
}
void cond::IOVEditorImpl::insert( const std::string& payloadToken, 
				  cond::Time_t tillTime ){
  try{
    if(!m_isActive) this->init();
    if(m_token.empty()){
      cond::IOV* myiov=new cond::IOV;
      myiov->iov.insert(std::make_pair<cond::Time_t, std::string>(tillTime, payloadToken));
      m_iov=cond::Ref<cond::IOV>(m_pooldb,myiov);
      m_iov.markWrite(cond::IOVNames::container());
      m_token=m_iov.token();
    }else{
      m_iov->iov.insert(std::make_pair<cond::Time_t, std::string>(tillTime, payloadToken));
       m_iov.markUpdate();
    }
   }catch( const cond::RefException& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const cond::Exception& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const std::exception& er ){
    std::cout<<er.what()<<std::endl;
  }
}
void cond::IOVEditorImpl::updateClosure( cond::Time_t newtillTime ){
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::updateClosure cannot change non-existing IOV index");
  try{
    if(!m_isActive) this->init();
    cond::Time_t closeIOV=m_iov->iov.rbegin()->first;
    std::string closePayload=m_iov->iov.rbegin()->second;
    m_iov->iov.insert( std::make_pair(newtillTime,closePayload) );
    m_iov->iov.erase( m_iov->iov.find(closeIOV) );
    m_iov.markUpdate();
  }catch( const cond::RefException& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const cond::Exception& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const std::exception& er ){
    std::cout<<er.what()<<std::endl;
  }
}
void cond::IOVEditorImpl::append( const std::string& payloadToken, cond::Time_t sinceTime ){
  std::cout<<"IOVEditorImpl::append "<<payloadToken<<" "<<sinceTime<<std::endl;
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::appendIOV cannot append to non-existing IOV index");
  //  try{
  if(!m_isActive) this->init();
  std::cout<<"inited"<<std::endl;
  cond::Time_t lastIOV=m_iov->iov.rbegin()->first;
  std::cout<<"lastIOV "<<lastIOV<<std::endl;
  std::string lastPayload=m_iov->iov.rbegin()->second;
  std::cout<<"lastPayload "<<lastPayload<<std::endl;
  m_iov->iov[lastIOV]=payloadToken;
  std::cout<<"token changed"<<std::endl;
  m_iov->iov.insert( std::make_pair((sinceTime+1),lastPayload) );
  std::cout<<"inserted"<<std::endl;
  m_iov.markUpdate();
  std::cout<<"marked write"<<std::endl;
  /*}catch( const cond::RefException& er ){
    std::cout<<er.what()<<std::endl;
    }catch( const cond::Exception& er ){
    std::cout<<er.what()<<std::endl;
    }catch( const std::exception& er ){
    std::cout<<er.what()<<std::endl;
    }
  */
}

void cond::IOVEditorImpl::deleteEntries(){
  if( m_token.empty() ) throw cond::Exception("cond::IOVEditorImpl::deleteEntries cannot delete to non-existing IOV index");
  try{
    if(!m_isActive) this->init();
    m_iov.markDelete();
  }catch( const cond::RefException& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const cond::Exception& er ){
    std::cout<<er.what()<<std::endl;
  }catch( const std::exception& er ){
    std::cout<<er.what()<<std::endl;
  }
}
