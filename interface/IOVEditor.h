#ifndef CondCore_IOVService_IOVEditor_h
#define CondCore_IOVService_IOVEditor_h
#include <string>
namespace cond{
  class IOVEditor{
  public:
    explicit IOVEditor( const std::string& token, size_t cominterval=1 ):m_token(token), m_cominterval(cominterval){
    }
    virtual ~IOVEditor(){}
    virtual void insert( const std::string& payloadToken, 
			 unsigned long long tillTime ) = 0;
    virtual void updateClosure( unsigned long long newtillTime ) = 0;
    virtual void deleteEntries() = 0;
  protected:
    std::string m_token;
    size_t m_cominterval;
  } ;
}//ns cond
#endif
