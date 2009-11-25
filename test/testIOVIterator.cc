#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "FWCore/PluginManager/interface/SharedLibrary.h"

#include "CondCore/DBCommon/interface/DbConnection.h"
#include "CondCore/DBCommon/interface/DbTransaction.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/IOVService/interface/IOVIterator.h"
#include "CondCore/IOVService/interface/IOVProxy.h"
#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>

#include "CondCore/DBCommon/interface/TypedRef.h"
#include "CondCore/IOVService/interface/PayloadProxy.h"


namespace {

  std::string oid(std::string token) {
    int pos = token.rfind('[');
    if (pos<0) return "[]";
    return token.substr(pos);
  }

  void print(cond::IOVElementProxy const & e) {
    std::cout<<"oid "<< oid(e.wrapperToken())
	     <<", since "<< e.since()
	     <<", till "<< e.till()
	     << std::endl;
  }
  
  void print(cond::PayloadProxy<cond::IOVElement> & data, cond::Time_t time) {
    cond::ValidityInterval iov = data.setIntervalFor(time);
    data.make();
    std::cout << "for " << time
	     <<": since "<< iov.first
	     <<", till "<< iov.second;
    if (data.isValid()) 
      std::cout    <<". Message "<< data().wrapperToken()
		   <<", since "<< data().sinceTime();
    else 
      std::cout << ". No data";
    std::cout << std::endl;
  }

}

struct Add {

  Add( cond::DbSession& db,  cond::IOVEditor & e) :
    pooldb(db), editor(e){}


  cond::DbSession pooldb;
  cond::IOVEditor & editor;

    void operator()(int i, std::string mess) {
      pool::Ref<cond::IOVElement> ref = pooldb.storeObject(new cond::IOVElement(i,mess),"SomeWhere");
      editor.append(i,ref.toString());
    }

};

int main(){
  edmplugin::PluginManager::Config config;
  edmplugin::PluginManager::configure(edmplugin::standard::config());
  try{
    cond::DbConnection connection;
    connection.configuration().setPoolAutomaticCleanUp( false );
    connection.configure();
    cond::DbSession pooldb = connection.createSession();
    pooldb.open("sqlite_file:mytest.db");
    pooldb.transaction().start(false);
    cond::IOVService iovmanager( pooldb );
    cond::IOVEditor* editor=iovmanager.newIOVEditor();
    editor->create(cond::timestamp,60);
    Add add(pooldb,*editor);
    add(1,"pay1");
    add(21,"pay2");
    add(41,"pay3");
    pooldb.transaction().commit();
    std::string iovtok=editor->token();
    ///test iterator
    // forward
    cond::IOVIterator* it=iovmanager.newIOVIterator(iovtok);
    std::cout<<"test forward iterator "<<std::endl;
    pooldb.transaction().start(true);
    std::cout << "size " << it->size()
	      <<", Time Type " << it->timetype() << std::endl;
    while( it->next() ){
      std::cout<<"payloadToken "<< oid(it->payloadToken());
      std::cout<<", since "<<it->validity().first;
      std::cout<<", till "<<it->validity().second<<std::endl;
    }
    delete it;
    // backward
    it=iovmanager.newIOVIterator(iovtok,cond::IOVService::backwardIter);
    std::cout<<"test reverse iterator "<<std::endl;
    while( it->next() ){
      std::cout<<"payloadToken "<< oid(it->payloadToken());
      std::cout<<", since "<<it->validity().first;
      std::cout<<", till "<<it->validity().second<<std::endl;
    }
    delete it;

    std::cout<<"is 30 valid? "<<iovmanager.isValid(iovtok,30)<<std::endl;
    std::pair<cond::Time_t, cond::Time_t> v =  iovmanager.validity(iovtok,30);
    std::cout<<"30 validity "<< v.first << " : " << v.second <<std::endl;
    std::cout<<"30 token "<< iovmanager.payloadToken(iovtok,30)<<std::endl;

    pooldb.transaction().commit();
    delete editor;
    // use Proxy
    {
      std::cout<<"test proxy "<<std::endl;
      cond::IOVProxy iov(pooldb,iovtok, true, false);
      std::cout << "size " << iov.size()
		<<", Time Type " << iov.timetype() << std::endl;
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
      std::cout << "range 5,45" << std::endl;
      iov.setRange(5,45);
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
      std::cout << "range 35,45" << std::endl;
      iov.setRange(35,45);
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
      std::cout << "range 45,70" << std::endl;
      iov.setRange(45,70);
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
      std::cout << "range 45,47" << std::endl;
      iov.setRange(45,47);
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
    }
    {
      // test "copy shallow"
      cond::IOVProxy iov( pooldb,iovtok, true, false);
      pooldb.close();
      std::cout << "size " << iov.size()
		<<", Time Type " << iov.timetype() << std::endl;
      std::cout << "head 2" << std::endl;
      iov.head(2);
      std::for_each(iov.begin(),iov.end(),boost::bind(&print,_1));
      std::cout << "find 3,23,43,63" << std::endl;
      print(*iov.find(3));
      print(*iov.find(23));
      print(*iov.find(43));
      print(*iov.find(63));
      iov.setRange(1,90);
      print(*iov.find(63));
      iov.resetRange();
      std::cout << "back" << std::endl;
      print(*(iov.end()-1));
      iov.tail(1);
      print(*iov.begin());

    }
    {
      pooldb.open("sqlite_file:mytest.db");
      // test PayloadProxy
      cond::PayloadProxy<cond::IOVElement> data(pooldb,iovtok,false);
      print(data,3);
      print(data,21);
      print(data,33);
      print(data,43);
      print(data,21);
      print(data,63);
      std::cout << "test refresh" << std::endl;
      // test refresh
      if (data.refresh()) std::cout << "error!, what refresh..." << std::endl;
      std::cout << " size " << data.iov().size() << std::endl;
      {
        cond::DbSession pooldb2 = connection.createSession();
        pooldb2.open("sqlite_file:mytest.db");
        pooldb2.transaction().start(false);
        cond::IOVService iovmanager2(pooldb2);
        cond::IOVEditor* editor=iovmanager2.newIOVEditor(iovtok);
        Add add(pooldb2,*editor);
        add(54,"pay54");
        delete editor;
        pooldb2.transaction().commit();
      }
      if (!data.refresh()) std::cout << "error!, NO refresh..." << std::endl;
      std::cout << " size " << data.iov().size() << std::endl;
      print(data,3);
      print(data,21);
      print(data,33);
      print(data,43);
      print(data,54);
      print(data,57);
      print(data,63);
    }
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
