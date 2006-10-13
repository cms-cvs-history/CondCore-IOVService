#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/Ref.h"
#include "CondCore/IOVService/src/IOV.h"
int main(){
  try{
    cond::ServiceLoader* loader=new cond::ServiceLoader;
    loader->loadMessageService(cond::Error);
    cond::DBSession session("sqlite_file:test.db");
    session.setCatalog("file:mycatalog.xml");
    session.connect(cond::ReadWriteCreate);
    cond::IOVService iovmanager(session);
    cond::IOVEditor* editor=iovmanager.newIOVEditor();
    session.startUpdateTransaction();
    editor->insert("pay1tok",20);
    editor->insert("pay2tok",40);
    editor->insert("pay3tok",60);
    editor->updateClosure(50);
    std::string token=editor->token();
    cond::IOVEditor* bomber=iovmanager.newIOVEditor(token);
    bomber->deleteEntries();
    session.commit();
    session.disconnect();
    delete editor;
    delete bomber;
    delete loader;
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
