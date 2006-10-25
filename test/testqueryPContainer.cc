#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/IOVService/src/IOV.h"
#include "testPayloadObj.h"
int main(){
  try{
    cond::ServiceLoader* loader=new cond::ServiceLoader;
    loader->loadMessageService(cond::Error);
    cond::DBSession session("sqlite_file:testqueryc.db");
    session.setCatalog("file:mycatalog.xml");
    session.connect(cond::ReadWriteCreate);
    testPayloadObj* myobj=new testPayloadObj;
    myobj->data.push_back(1);
    myobj->data.push_back(10);
    cond::Ref<testPayloadObj> myref(session,myobj);
    session.startUpdateTransaction();
    myref.markWrite("mypayloadcontainer");
    std::string token=myref.token();
    std::cout<<"payload token "<<token<<std::endl;
    cond::IOVService iovmanager(session);
    cond::IOVEditor* editor=iovmanager.newIOVEditor();
    editor->insert(token,20);
    std::string iovtok=editor->token();
    std::string cname=iovmanager.payloadContainerName(iovtok);
    session.commit();
    session.disconnect();
    std::cout<<"Payload Container Name: "<<cname<<std::endl;
    delete editor;
    delete loader;
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
