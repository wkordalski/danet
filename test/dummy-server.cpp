#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <network.h>
#include <protocols/dummy/dummy.h>
#include <address/tcp_ip_address.h>

using namespace std;

class nill
{
  static vector<unsigned char> get_data(const nill &m)
  {
    return vector<unsigned char>();
  }
  static nill set_data(const vector<unsigned char> &v)
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
  danet::network<nill> n(danet::protocols::dummy::create(true));
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
  while(1);
  return 0;
}
