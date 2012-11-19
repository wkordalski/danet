/*
 * File:   connection.h
 * Author: wojtex
 *
 * Created on 11 listopad 2012, 16:46
 */

#ifndef __DANET_CONNECTION_H
#define	__DANET_CONNECTION_H

namespace danet
{
  class connection;
}

#include "acceptor.h"
#include "netbase.h"

#include <boost/asio.hpp>

#include <mutex>
#include <string>
#include <vector>

namespace danet
{
  class connection
  {
    friend class netbase;
    friend class acceptor;
  public:
    virtual ~connection();

    void listen();

    //void close();
    //void send(std::shared_ptr<std::vector<unsigned char>> data);
    //void recv();

  protected:
    connection(netbase *nb, std::string ip, int port);
    connection(netbase *nb);

    void on_connect(const boost::system::error_code &ec);
    void on_header(const boost::system::error_code &ec, const size_t &bt);
    void on_body(const boost::system::error_code &ec, const size_t &bt);
    void on_send(const boost::system::error_code &ec);

    // Netbase object pointer
    netbase *nb;

    // Socket used to communicate with other machine
    boost::asio::ip::tcp::socket *sck;

    // Buffer for header
    byte header_buff[8];

    // Strands
    boost::asio::strand *strd_r;     // Reading strand
    //boost::asio::strand strda;    // Send
    //boost::asio::strand strdb;    // Recv
    //std::queue<std::shared_ptr<std::vector<unsigned char>>> sndq;
    //std::mutex sndm;

    //unsigned char head[8];
    //std::vector<unsigned char> buff;
    //std::mutex rcvm;
    //int ct, cs, cr;     // Receiving data
  private:

  };
}
#endif

