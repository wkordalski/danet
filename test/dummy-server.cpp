/*
 * Dummy serwer (using danet library)
 * This file is a sample application to run networks using dummy protocol.
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

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <boost/program_options.hpp>

#include <network.h>
#include <protocol_dummy.h>
#include <address_ip_tcp.h>

using namespace std;

class nill
{
  // EMPTY CLASS
};

class nill_serializer : public danet::serializer<nill>
{
  std::shared_ptr<std::vector<danet::byte>> save(const nill &)
  {
    return std::shared_ptr<vector<unsigned char>>(new vector<unsigned char>());
  }
  nill load(vector<unsigned char> v)
  {
    return nill();
  }
};

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

int main(int argc, char *argv[])
{
  opts aval("Allowed options");
  aval.add_options()
      ("help,h", "Prints this help message.")
      ("listen,l", po::value<vector<string>>(), "Listen on these address and ports")
      ("connect,c", po::value<vector<string>>(), "Connect to these address and ports");
  varmap vm;
  po::store(po::parse_command_line(argc, argv, aval), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << "Dummy server (from libdanet)" << endl;
    cout << endl;
    cout << aval << endl;
    cout << "Please report bugs to email@server.com (address invalid now)" << endl;
    return 1;
  }

  clog << "Starting dummmy server (from libdanet)" << endl;
  danet::network<nill> n(danet::protocols::dummy::create(true), std::shared_ptr<danet::serializer<nill>>(new nill_serializer()));
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
  clog << "Ready (press Ctrl+C to quit)" << endl;
  while(1)
  {
    this_thread::sleep_for(chrono::milliseconds(1000));
  }
  return 0;
}
