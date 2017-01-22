#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <thread>

class pgr_serial_impl 
{
public:
    void destroy() {
        std::cout << "Destroy" << std::endl;
    }

    template <typename MutableBufferSequence>
    std::size_t read_some(const MutableBufferSequence& buffers) {
        return 10;
    }

};

// ----- Service

template <typename Implementation = pgr_serial_impl>
class pgr_serial_service
	: public boost::asio::io_service::service
{
public:
	static boost::asio::io_service::id id;

	explicit pgr_serial_service(boost::asio::io_service &io_service)
		: boost::asio::io_service::service(io_service),
		async_work_(new boost::asio::io_service::work(async_io_service_)),
		async_thread_(boost::bind(&boost::asio::io_service::run, &async_io_service_))
	{
	}

	typedef boost::shared_ptr<Implementation> implementation_type;

	void construct(implementation_type &impl)
	{
		impl.reset(new Implementation());
	}

	void destroy(implementation_type &impl)
	{
		impl->destroy();
		impl.reset();
	}

     /// Start an asynchronous write.
    template <typename MutableBufferSequence, typename ReadHandler>
    std::size_t async_read_some(implementation_type& impl,
                          const MutableBufferSequence& buffers,
                          ReadHandler handler) {
        auto er = boost::system::error_code();
        handler(er, std::size_t(10));
    }

private:
	void shutdown_service()
	{
		async_work_.reset();
		async_io_service_.stop();
		async_thread_.join();
	}

	boost::asio::io_service async_io_service_;
	boost::scoped_ptr<boost::asio::io_service::work> async_work_;
	boost::thread async_thread_;
};

template <typename Implementation>
boost::asio::io_service::id pgr_serial_service<Implementation>::id;

// ----- I/O object

template <typename Service>
class pgr_serial
	: public boost::asio::basic_io_object<Service>
{
public:
	explicit pgr_serial(boost::asio::io_service &io_service)
		: boost::asio::basic_io_object<Service>(io_service)
	{
	}

    template <typename MutableBufferSequence, typename ReadHandler>
    void async_read_some(const MutableBufferSequence& buffers,
                                ReadHandler handler) {
        this->get_service().async_read_some(this->get_implementation(), buffers,
                                            handler);
    }

};

// ----- main

void wait_handler(const boost::system::error_code &ec, std::size_t bytes)
{
	std::cout << "handler (" << ec << ")" << std::endl;
    std::cout << "Bytes: " << bytes << std::endl;
}

typedef pgr_serial<pgr_serial_service<> > serial;

int main()
{
    std::vector<int> buffer(10);
    
	boost::asio::io_service io_service;
	serial t(io_service);
    boost::asio::async_read(t, boost::asio::buffer(buffer), wait_handler);
    //io_service.run();
	std::cin.get();

}
