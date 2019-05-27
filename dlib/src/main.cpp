#include <iostream>
#include <thread>
#include "sockets.h"
#include "dlib.h"


std::unique_ptr<dlib::listener> l;

void* listen_work()
{
	while (true)
	{
		std::cout << "listen on ip:" << l->get_listening_ip() << "\tport:" << l->get_listening_port() << std::endl;
		dlib::connection* c1;
		if (l->accept(c1) == 0)
		{
			std::thread th([&]()
				{
					std::cout << "listen_work create new client" << std::endl;
					std::cout << "ip:" << c1->get_foreign_ip() << std::endl;
					std::cout << "port:" << c1->get_foreign_port() << std::endl;
					while (true)
					{
						char buf[1024];
						long status = c1->read(buf, 1024);
						if (status <= 0)
						{
							std::cout << "read error : status = " << status << std::endl;
							break;
						}
						c1->write(buf, status);
					}
					free(c1);
				});
			th.detach();
		}
	}
}

int main()
{
	try
	{
		std::string hostname;

		DLIBM_CASSERT(dlib::get_local_hostname(hostname) == 0, "get_local_hostname error");

		hostname = "www.baidu.com";
		std::string ip;
		DLIBM_CASSERT(dlib::hostname_to_ip("www.baidu.com", ip) == 0, "hostname_to_ip error");
		std::cout << "hostname : " << hostname << ", ip : " << ip << std::endl;

		std::cout << "hostname : " << hostname << std::endl;

		DLIBM_CASSERT(dlib::create_listener(l, 9090) == 0, "create_listener error");

		std::thread listen_th(listen_work);

		std::chrono::seconds timespan(2);
		std::this_thread::sleep_for(timespan);

		std::unique_ptr<dlib::connection> c;
		DLIBM_CASSERT(dlib::create_connection(c, 9090, "127.0.0.1") == 0, "create_connection error");

		DLIBM_CASSERT(c->write("hello", 5) == 5, "write");

		char buf[6] = { 0 };
		int status = c->read(buf, 5);
		if (status > 0)
		{
			buf[status] = '\0';
		}
		std::cout << "client : recv "<< buf << std::endl;

		c->shutdown();

		listen_th.join();
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}