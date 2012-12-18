#include "shared.h"

#include "../include/network.h"
#include "../include/protocols/basic/basic_00.h"
#include "../include/address/tcp_ip_address.h"

#include <iostream>

using namespace std;

int prt;

int main()
{
  cout << "Port: ";
  cin >> prt;
  cin.ignore();
  danet::network<msg> n(danet::protocols::basic<0>::create(-1));
  danet::ip::tcp::address adr("127.0.0.1", prt);
  if(!n.listen(&adr))
  {
    cerr << "Listen error" << endl;
    return 1;
  }
  cerr << "Listening..."<<endl;
  while(1)
  {
    msg m;
    int r = 0;
    if((r = n.receive(m)))
    {
      cout << r << ": " << m.str << endl;
    }
  }
  return 0;
}