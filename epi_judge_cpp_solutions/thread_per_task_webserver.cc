
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>

using boost::system::error_code;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::thread;
namespace asio = boost::asio;
using asio::ip::tcp;

void ProcessReq(shared_ptr<tcp::socket> sock) {
  asio::streambuf sb;
  while (true) {
    error_code e;
    size_t n = asio::read_until(*sock, sb, '\n', e);
    if (e == asio::error::eof) {
      cout << endl << "connection closed" << endl;
      break;
    }
    asio::write(*sock, sb.data());
    cout << &sb;
  }
}

const unsigned short SERVERPORT = 8080;

int main(int argc, char* argv[]) {
  asio::io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), SERVERPORT));
  while (true) {
    shared_ptr<tcp::socket> sock(new tcp::socket(io_service));
    acceptor.accept(*sock);
    thread(ProcessReq, sock).detach();
  }
  return 0;
}
