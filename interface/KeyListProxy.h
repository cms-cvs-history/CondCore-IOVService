#ifndef CondCore_IOVService_KeyListProxy_h
#define CondCore_IOVService_KeyListProxy_h

#include "CondCore/IOVService/interface/PayloadProxy.h"
#include "CondCore/IOVService/interface/KeyList.h"
#include <vector>
#include <string>

namespace cond {

  template<> class PayloadProxy<cond::KeyList> : public PayloadProxy<std::vector<cond::Time_t> > {
  public:
    typedef std::vector<cond::Time_t> DataT;
    typedef PayloadProxy<DataT> super;

    PayloadProxy(cond::Connection& conn,
		 const std::string & token, bool errorPolicy) :
      super(conn, token, errorPolicy) {}
    
    virtual ~PayloadProxy(){}

    // dereference (does not load)
    const cond::KeyList & operator()() const {
      return me; 
    }
        
    virtual void invalidateCache() {
      super::invalidateCache();
    }

    virtual void loadMore(CondGetter const & getter){
      me.init(getter.get(m_name));
    }


  protected:
    virtual bool load(pool::IDataSvc * svc, std::string const & token) {
     bool ok = super::load(svc, token);
      me.load(super::operator()());
      return ok;
    }

  private:

    std::string m_name;

    KeyList me;

  };
}
#endif
