#include <string>
#include <vector>

using namespace std;

struct msg
{
  string str;
  static vector<unsigned char> get_data(const msg &m)
  {
    vector<unsigned char> v;
    v.reserve(m.str.size());
    for(char c: m.str)
    {
      v.push_back((unsigned char)c);
    }
    return v;
  }
  static msg set_data(const vector<unsigned char> &v)
  {
    msg m;
    m.str.reserve(v.size());
    for(unsigned char c: v)
    {
      m.str.push_back((char)c);
    }
    return m;
  }
};
