#include "io_context.hh"
#include "socket.hh"
#include "task.hh"
#include <thread>

/*
std::task<bool> inside_loop(Socket& socket)
{
    char buffer[42] = {0};
    ssize_t nbRecv = co_await socket.recv(buffer, sizeof buffer);
    ssize_t nbSend = 0;
    while (nbSend < nbRecv)
    {
        ssize_t res = co_await socket.send(buffer, sizeof buffer);
        if (res <= 0)
            co_return false;
        nbSend += res;
    }
    //std::cout << "DONE (" << nbRecv << "):" << '\n';
    if (nbRecv <= 0)
        co_return false;
    printf("%s\n", buffer);
    co_return true;
}
*/
#define BUFFER                                                                  \
  "HTTP/1.1 200 OK\r\nContent-Length: 104\r\nConnection: "                      \
  "close\r\nContent-Type: text/html;charset=utf-8\r\n\r\n <html> 响应数据 " \
  "<head> <title>HTTP响应示例<title> </head>  <body> Hello HTTP! </body> "  \
  "</html>"

std::task<bool> inside_loop(Socket &socket) {
  char buffer[1024] = {0};
  
    ssize_t nbRecv = co_await socket.recv(buffer, sizeof buffer);

    char send_buffer[] = BUFFER;
    ssize_t res = co_await socket.send(send_buffer, strlen(BUFFER));
    if (res <= 0)
      co_return false;

    socket.closeFd();
    // std::cout << "DONE (" << nbRecv << "):" << '\n';
    if (nbRecv <= 0)
      co_return false;
    // printf("%s\n", buffer);
    
  

  co_return false;
}

std::task<> echo_socket(std::shared_ptr<Socket> socket) {
  bool run = true;
  while (run) {
    //std::cout << "BEGIN\n";
    run = co_await inside_loop(*socket);
    //std::cout << "END\n";
    break;
  }
}

std::task<> accept(Socket &listen) {
  while (true) {
    auto socket = co_await listen.accept();
    auto t = echo_socket(socket);

      //std::thread([&]() {
        // 协程真正有意思的地方是，它可以由不同的线程去resume，这会很有意义。
        t.resume();
        
        // 这里依旧输出9，上面一个c.resume()之后，test函数已经跳出循环返回了，已经满足了_Handle.done()，不会再继续真正的_Handle.resume();
        //std::cout << (int)c.resume() << std::endl;
      //}).join();

    
  }
}

int main() {
  IOContext io_context{};
  Socket listen{"3490", io_context};
  auto t = accept(listen); 
  // 协程真正有意思的地方是，它可以由不同的线程去resume，这会很有意义。
  t.resume();

  io_context.run();
}
