#include <boost/asio.hpp> 
#include <boost/system/error_code.hpp> 
#include <string> 
#include <iostream>
#include <memory>

namespace boost  {
namespace asio {

template <typename SerialPortService> 
class pgr_serial_port 
    : public boost::asio::basic_io_object<SerialPortService> 
{ 
public: 
    explicit pgr_serial_port(boost::asio::io_service &io_service) 
        : boost::asio::basic_io_object<SerialPortService>(io_service) 
    { 
    } 

  void open(const std::string& device)
  {
  }

  bool is_open() const
  {
  }

  void close()
  {
  }


  void cancel()
  {
  }

  void send_break()
  {
  }

  template <typename ConstBufferSequence, typename WriteHandler>
  void async_write_some(const ConstBufferSequence& buffers,
      WriteHandler handler)
  {
      
  }



  template <typename MutableBufferSequence, typename ReadHandler>
  void async_read_some(const MutableBufferSequence& buffers, ReadHandler handler)
  {


  }
}; 

template <typename PgrSerialPortImpl>
class pgr_serial_port_service : public boost::asio::io_service::service 
{
public:

    using implementation_t = std::shared_ptr<PgrSerialPortImpl>;
    
    static boost::asio::io_service::id id; 
    
    explicit
    pgr_serial_port_service(boost::asio::io_service &io_service) 
        : boost::asio::io_service::service(io_service)
        { 

        } 

          
    ~pgr_serial_port_service()
        { 

        } 
    void construct(implementation_t &impl) 
        { 
            impl.reset(new PgrSerialPortImpl()); 
            impl->begin_read(); 
    } 

    void destroy(implementation_t &impl) 
        { 
            impl->destroy(); 

            impl.reset(); 
        } 
}; 

class pgr_serial_impl : 
    public std::enable_shared_from_this<pgr_serial_impl> 
{ 
public: 
    pgr_serial_impl() 
    { 
    } 


    void destroy() 
    { 

    } 

}; 


}
}

int main(int argc, char *argv[])
{
    std::cout << "Hello" << std::endl;
}


