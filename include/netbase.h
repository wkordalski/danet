#ifndef __DANET_NETBASE_H
#define __DANET_NETBASE_H

namespace danet
{
  class netbase;
  typedef unsigned char byte;
}

#include <boost/asio.hpp>

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace danet
{
  class netbase
  {
  protected:
    typedef unsigned int handle;
    typedef unsigned int user;
    typedef boost::asio::ip::basic_resolver_entry<boost::asio::ip::tcp> address;

    netbase();
    netbase(const netbase& orig);
    ~netbase();

  protected:
    handle listen_at(std::string ip, int port);
    handle connect_to(std::string ip, int port);
    void close_resource(handle h);
    void send_message(const std::vector<byte> &v, const std::vector<user> &s);
    void recv_message(std::vector<byte> &v, user &s);

    address resolve(std::string ip, int port);

    void io_worker();

  private:
protected:

    friend class connection;
    friend class acceptor;

    //void received(message msg);
    //void accepted(const boost::system::error_code & ec, std::shared_ptr<connection>);
    //void connected(const boost::system::error_code & ec, std::shared_ptr< connection > con);
    //void add_worker(int n);
    //void worker();
    //void add_message(message m);
    //void bconnect(std::string ip, int port);
    //void bconnect(std::string ip, int port, int hid);
    //void inform_ips(std::shared_ptr<connection> c);
    //void suggest_id(std::shared_ptr<connection> c);
    //void recv_ips(message m);

    //int myid = -1;
    //std::vector<std::shared_ptr<connection>> pairs;
    //std::mutex parm;
    //int pairs_count;
    //std::queue<message> rcvq;
    //std::mutex rcvm;
    //std::vector<std::shared_ptr<connection>> unsorted;
    //int state = 0;
    //std::mutex unsm;
    boost::asio::io_service service;
    boost::asio::io_service::work work;
    std::vector<std::thread *> workers;
  };
}

#endif	/* NETMAN_H */

