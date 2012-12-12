#include "shared.h"

#include "../include/network.h"
#include "../include/protocols/basic_00.h"
#include "../include/address/tcp_ip_address.h"

#include <iostream>

using namespace std;

int main()
{
  danet::network<msg> n(danet::protocols::basic<0>::create(-1));
  danet::ip::tcp::address adr("127.0.0.1", 7777);
  if(!n.connect(&adr))
  {
    cerr << "Connect error" << endl;
    return 1;
  }
  cerr << "Connected..."<<endl;
  msg m;
  while(1)
  {
    cin >> m.str;
    if(!m.str.empty())
    {
      cout << m.str << endl;
      n.send(m, {0});
    }
    int r = 0;
    if((r = n.receive(m)))
    {
      cout << r << ": " << m.str << endl;
    }
  }
  return 0;
}