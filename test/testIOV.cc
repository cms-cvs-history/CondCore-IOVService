#include "CondCore/IOVService/src/IOV.h"

#include<iostream>

int main() {


  cond::IOV iov;
  
  iov.add(10,"a");
  iov.add(20,"b");
  if (iov.add(30,"c")!=2) std::cerr << "error pos" << std::endl;
  iov.add(40,"d");


  if (iov.size()!=4) std::cerr << "error size" << std::endl;
  
  if (iov.find(0)!=iov.begin()) std::cerr << "error find 0" << std::endl;
  if (iov.find(5)!=iov.begin()) std::cerr << "error find 5" << std::endl;
  if (iov.find(10)!=iov.begin()) std::cerr << "error find 10" << std::endl;
  if (iov.find(25)!=(iov.begin()+2)) std::cerr << "error find 25" << std::endl;
  if (iov.find(45)!=(iov.end)) std::cerr << "error find 45" << std::endl;


  return 0;

}
