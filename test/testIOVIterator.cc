#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/IOVService/interface/IOVIterator.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/IOVService/src/IOV.h"
int main(){
  try{
    cond::DBSession* session=new cond::DBSession("sqlite_file:test.db");
    session->sessionConfiguration().setMessageLevel(cond::Error);
    session->open(true);
    cond::PoolStorageManager& pooldb=session->poolStorageManager("file:mycatalog.xml");
    pooldb.connect(cond::ReadWriteCreate);
    cond::IOVService iovmanager(pooldb);
    cond::IOVEditor* editor=iovmanager.newIOVEditor();
    pooldb.startTransaction(false);
    editor->insert(20,"pay1tok");
    editor->insert(40,"pay2tok");
    editor->insert(60,"pay3tok");
    pooldb.commit();
    std::string iovtok=editor->token();
    ///test iterator
    cond::IOVIterator* it=iovmanager.newIOVIterator(iovtok);
    std::cout<<"test iterator "<<std::endl;
    pooldb.startTransaction(true);
    while( it->next() ){
      std::cout<<"payloadToken "<<it->payloadToken()<<std::endl;
      std::cout<<"since "<<it->validity().first<<std::endl;
      std::cout<<"till "<<it->validity().second<<std::endl;
    }
    std::cout<<"is 30 valid? "<<it->isValid(30)<<std::endl;
    pooldb.commit();
    pooldb.disconnect();
    session->close();
    delete editor;
    delete it;
    delete session;
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
