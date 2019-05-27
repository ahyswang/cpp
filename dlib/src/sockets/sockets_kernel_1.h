#ifndef DLIB_SOCKETS_KERNEl_1_
#define DLIB_SOCKETS_KERNEl_1_

#ifdef DLIB_ISO_CPP_ONLY
#error "DLIB_ISO_CPP_ONLY is defined so you can't use this OS dependent code.  Turn DLIB_ISO_CPP_ONLY off if you want to use it."
#endif

#include "../platform.h"

//#include "sockets_kernel_abstract.h"

#include <memory>
#include <string>
#include <thread>
#include <mutex>


namespace dlib
{

    class socket_factory;
    class listener;
    class SOCKET_container;

    int get_local_hostname ( std::string& hostname );

    int hostname_to_ip ( const std::string& hostname, std::string& ip, int n = 0 );

    int ip_to_hostname ( const std::string& ip, std::string& hostname );

    class connection
    {
        friend class listener;                
        friend int create_connection ( connection*& new_connection, unsigned short foreign_port, const std::string& foreign_ip, 
            unsigned short local_port, const std::string& local_ip);

    public:

        ~connection ();

        void* user_data;

        long write ( const char* buf, long num);

        long read ( char* buf, long num);

        long read ( char* buf, long num, unsigned long timeout);

        unsigned short get_local_port () const {  return connection_local_port; }

        unsigned short get_foreign_port () const { return connection_foreign_port; }

        const std::string& get_local_ip () const { return connection_local_ip; }

        const std::string& get_foreign_ip () const { return connection_foreign_ip; }

        int shutdown_outgoing ();

        int shutdown ();

        //typedef unsigned_type<void*>::type socket_descriptor_type;
		
        int disable_nagle();

        //socket_descriptor_type get_socket_descriptor () const;

    private:

        bool readable ( unsigned long timeout ) const;
        
        bool sd_called () const
        {
            //sd_mutex.lock();
            bool temp = sd;
            //sd_mutex.unlock();
            return temp;
        }

        bool sdo_called () const
        {
            //sd_mutex.lock();
            bool temp = false;
            if (sdo || sd)
                temp = true;
            //sd_mutex.unlock();
            return temp;
        }
		

        // data members
        SOCKET_container& connection_socket;
        const unsigned short connection_foreign_port;
        const std::string connection_foreign_ip; 
        const unsigned short connection_local_port;
        const std::string connection_local_ip;

        bool sd;  // called shutdown
        bool sdo; // called shutdown_outgoing
        int sdr; // return value for shutdown 
        std::mutex sd_mutex; // a lock for the three above vars


        connection(
            SOCKET_container sock,
            unsigned short foreign_port, 
            const std::string& foreign_ip, 
            unsigned short local_port,
            const std::string& local_ip
        ); 
        
        // restricted functions
        connection(connection&);        // copy constructor
        connection& operator=(connection&);    // assignment operator
    }; 


    class listener
    {
        friend int create_listener ( listener*& new_listener, unsigned short port, const std::string& ip );

    public:

        ~listener ();

        int accept ( connection*& new_connection, unsigned long timeout = 0);

        int accept ( std::unique_ptr<connection>& new_connection, unsigned long timeout = 0);

        unsigned short get_listening_port ( ) { return listening_port; }

        const std::string& get_listening_ip ( ) { return listening_ip; }

    private:

        SOCKET_container& listening_socket;
        const unsigned short listening_port;
        const std::string listening_ip;
        const bool inaddr_any;

        listener( SOCKET_container sock, unsigned short port, const std::string& ip );

        listener(listener&);        
        listener& operator=(listener&); 
    };

// ----------------------------------------------------------------------------------------

    int create_listener (
        listener*& new_listener,
        unsigned short port,
        const std::string& ip = ""
    );

    int create_connection ( 
        connection*& new_connection,
        unsigned short foreign_port, 
        const std::string& foreign_ip, 
        unsigned short local_port = 0,
        const std::string& local_ip = ""
    );

    int create_listener (
        std::unique_ptr<listener>& new_listener,
        unsigned short port,
        const std::string& ip = ""
    );

    int create_connection ( 
        std::unique_ptr<connection>& new_connection,
        unsigned short foreign_port, 
        const std::string& foreign_ip, 
        unsigned short local_port = 0,
        const std::string& local_ip = ""
    );


}

#ifdef NO_MAKEFILE
#include "sockets_kernel_1.cpp"
#endif

#endif // DLIB_SOCKETS_KERNEl_1_

