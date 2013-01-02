/*
 * Chat (using danet library)
 * This file is a sample application to test danet library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For a list of authors see AUTHORS file.
 */

#include <iostream>
#include <map>
#include <string>

#include <boost/program_options.hpp>

#include <network.h>
#include <protocol_dummy.h>
#include <address_ip_tcp.h>

using namespace std;

struct msg
{
  int id;
  string str;
};

class msg_serializer : public danet::serializer<msg>
{
  std::shared_ptr<std::vector<danet::byte>> save(const msg &m)
  {
    std::shared_ptr<vector<unsigned char>> v(new vector<unsigned char>());
    v->reserve(m.str.size()+4);
    int tid = m.id;
    for(int i = 0; i < 4; i++)
    {
      v->push_back(tid & 0xFF);
      tid >>= 8;
    }
    for(char c: m.str)
    {
      v->push_back((unsigned char)c);
    }
    return v;
  }
  msg load(vector<unsigned char> v)
  {
    if(v.size() < 4)
    {
      // BŁĘDNY PAKIET
      assert(0);
      return msg();
    }
    msg m;
    for(int i = 3; i >= 0; i--)
    {
      m.id <<= 8;
      m.id |= v[i];
    }
    m.str = "";
    if(v.size() > 4)
    {
      m.str.reserve(v.size()-4);
      for(int i = 4; i < v.size(); i++)
      {
        m.str.push_back((char)v[i]);
      }
    }
    return m;
  }
};

map<int, string> us;


string t;

string uname;
// <editor-fold defaultstate="collapsed" desc="Command line parsing friendly functions">

namespace po = boost::program_options;
typedef boost::program_options::variables_map varmap;
typedef boost::program_options::options_description_easy_init opts_init;
typedef boost::program_options::options_description opts;

pair<string,int> parse(string s)
{
  bool adr = true;
  string fr = "";
  string sc = "";
  for(char C : s)
  {
    if(C == ':')
    {
      adr = false;
      continue;
    }
    if(adr) fr += C;
    else sc += C;
  }
  int ccs = std::stoi(sc);
  return pair<string,int>(fr,ccs);
}

// </editor-fold>


int main(int argc, char *argv[])
{
  // <editor-fold defaultstate="collapsed" desc="Command line parsing">
  opts aval("Allowed options");
  aval.add_options()
      ("help,h", "Prints this help message.")
      ("nick,n", po::value<string>(), "Your nick name")
      ("listen,l", po::value<vector<string>>(), "Listen on these address and ports")
      ("connect,c", po::value<vector<string>>(), "Connect to these address and ports");
  varmap vm;
  po::store(po::parse_command_line(argc, argv, aval), vm);
  po::notify(vm);

  if (vm.count("help") > 0 || vm.count("nick") == 0)
  {
    cout << "Chat - dummy protocol (from libdanet)" << endl;
    cout << endl;
    cout << aval << endl;
    cout << "Please report bugs to email@server.com (address invalid now)" << endl;
    return 1;
  }
  else
  {
    uname = vm["nick"].as<string>();
  }
  // </editor-fold>

  clog << "Starting chat (from libdanet)" << endl;
  danet::network<msg> n(danet::protocols::dummy::create(false), std::shared_ptr<danet::serializer<msg>>(new msg_serializer()));
  // <editor-fold defaultstate="collapsed" desc="Adding connections">
  vector<string> ls;
  if(vm.count("listen"))
  {
    ls = vm["listen"].as<vector<string>>();
  }
  vector<string> cn;
  if(vm.count("connect"))
  {
    cn = vm["connect"].as<vector<string>>();
  }
  for(string S : ls)
  {
    auto P = parse(S);
    danet::ip::tcp::address adr(P.first, P.second);
    if(n.listen(&adr) == 0)
    {
      clog << "Error listening on " << P.first << " at port " << P.second << endl;
    }
  }
  for(string S : cn)
  {
    auto P = parse(S);
    danet::ip::tcp::address adr(P.first, P.second);
    if(n.connect(&adr) == 0)
    {
      clog << "Error connecting to " << P.first << " at port " << P.second << endl;
    }
  }
  // </editor-fold>
  cout << "Press enter to start" << endl;
  getline(cin, t);
  msg mmm;
  mmm.id = n.id();
  mmm.str = uname;
  vector<danet::network<msg>::user> usr2;
  auto nusers = n.users();
  for(danet::network<msg>::user U : nusers)
    if(U != mmm.id)
      usr2.push_back(U);
  n.send(mmm, usr2);
  clog << "Ready (send quit() to quit)" << endl;
  while(1)
  {
    msg m;
    getline(cin, m.str);
    if(m.str == "quit()")
    {
      break;
    }
    if(!m.str.empty())
    {
      while(1)
      {
        string rcv;
        cout << "To: ";
        cin >> rcv;
        if(rcv == "@") break;
        cin.ignore();
        int rcvi = 0;
        for(pair<int,string> P : us)
          if(P.second == rcv)
          {
            rcvi = P.first;
            break;
          }
        if(rcvi)
        {
          m.id = 0;
          n.send(m, {rcvi});
          break;
        }
        else
        {
          cout << "User not found..." << endl;
        }
      }
    }
    int r = 0;
    while((r = n.receive(m)) > 0)
    {
      if(m.id == 0)
      {
        cout << us[r] << ": " << m.str << endl;
      }
      else
      {
        if(m.str == "")
        {
          if(us.find(m.id) != us.end())
            us.erase(m.id);
          clog << "User with id " << m.id << " leaved" << endl;
        }
        else
        {
          if(m.str[0] == '!')
          {
            us[m.id] = m.str.erase(0,1);
          }
          else
          {
            us[m.id] = m.str;
            msg mm;
            mm.id = n.id();
            mm.str = "!" + uname;
            n.send(mm, {m.id});
          }
          clog << "User with id " << m.id << " connected as " << m.str << endl;
        }
      }
    }
  }
  msg m;
  m.id = n.id();
  m.str = "";
  vector<danet::network<msg>::user> usr;
  for(pair<int,string> P : us)
  {
    if(P.first != m.id)
    {
      usr.push_back(P.first);
    }
  }
  n.send(m, usr);
  cout << "Press enter to exit..." << endl;
  getline(cin, m.str);
  return 0;
}
