#include "shared.h"

#include "../include/network.h"
#include "../include/protocols/basic/basic_00.h"
#include "../include/address/tcp_ip_address.h"
#include "protocols/dummy/dummy.h"

#include <iostream>

using namespace std;

int prt;

int main()
{
  cout << "Port: ";
  cin >> prt;
  cin.ignore();
  danet::network<msg> n(danet::protocols::dummy::create(false));
  danet::ip::tcp::address adr("127.0.0.1", prt);
  if(!n.connect(&adr))
  {
    cerr << "Connect error" << endl;
    return 1;
  }
  cerr << "Connected..."<<endl;
  msg m;
  while(1)
  {
    getline(cin, m.str);
    if(!m.str.empty())
    {
      int rcv;
      cin >> rcv;
      cin.ignore();
      n.send(m, {rcv});
    }
    int r = 0;
    if((r = n.receive(m)))
    {
      cout << r << ": " << m.str << endl;
    }
  }
  return 0;
}