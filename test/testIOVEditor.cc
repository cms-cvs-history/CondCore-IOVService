#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/Ref.h"
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
    editor->insert("pay1tok",20);
    editor->insert("pay2tok",40);
    editor->insert("pay3tok",60);
    //editor->updateClosure(50);
    std::string token=editor->token();
    std::cout<<"iov token "<<token<<std::endl;
    cond::IOVEditor* bomber=iovmanager.newIOVEditor(token);
    bomber->deleteEntries();
    pooldb.commit();
    pooldb.disconnect();
    session->close();
    delete editor;
    delete bomber;
    delete session;
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
