#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/PoolStorageManager.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/IOVService/src/IOV.h"
#include "testPayloadObj.h"
int main(){
  try{
    cond::DBSession* session=new cond::DBSession("sqlite_file:testqueryc.db");
    session->sessionConfiguration().setMessageLevel(cond::Error);
    session->open(true);
    cond::PoolStorageManager& pooldb=session->poolStorageManager("file:mycatalog.xml");
    pooldb.connect(cond::ReadWriteCreate);
    testPayloadObj* myobj=new testPayloadObj;
    myobj->data.push_back(1);
    myobj->data.push_back(10);
    cond::Ref<testPayloadObj> myref(pooldb,myobj);
    pooldb.startTransaction(false);
    myref.markWrite("mypayloadcontainer");
    std::string token=myref.token();
    std::cout<<"payload token "<<token<<std::endl;
    cond::IOVService iovmanager(pooldb);
    cond::IOVEditor* editor=iovmanager.newIOVEditor();
    editor->insert(token,20);
    std::string iovtok=editor->token();
    std::string cname=iovmanager.payloadContainerName(iovtok);
    pooldb.commit();
    pooldb.disconnect();
    std::cout<<"Payload Container Name: "<<cname<<std::endl;
    delete editor;
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
