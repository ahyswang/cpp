// Copyright (C) 2003  Davis E. King (davis@dlib.net), Miguel Grinberg
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_SOCKETS_KERNEl_2_
#define DLIB_SOCKETS_KERNEl_2_

#ifdef DLIB_ISO_CPP_ONLY
#error "DLIB_ISO_CPP_ONLY is defined so you can't use this OS dependent code.  Turn DLIB_ISO_CPP_ONLY off if you want to use it."
#endif

#include "../platform.h"

//#include "sockets_kernel_abstract.h"

#define _BSD_SOCKLEN_T_

#include <ctime>
#include <memory>
#include <string>
#include <mutex>

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#ifndef HPUX
#include <sys/select.h>
#endif
#include <arpa/inet.h>
#include <signal.h>
#include <inttypes.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/param.h>

#include <netinet/in.h>


namespace dlib
{

// ----------------------------------------------------------------------------------------

    // forward declarations
    class socket_factory;
    class listener;

// ----------------------------------------------------------------------------------------

    // lookup functions

    int get_local_hostname ( std::string& hostname );

    int hostname_to_ip ( const std::string& hostname, std::string& ip, int n = 0);

    int ip_to_hostname ( const std::string& ip, std::string& hostname );

    class connection
    {
        friend class listener;                // make listener a friend of connection
        // make create_connection a friend of connection
        friend int create_connection ( 
            connection*& new_connection,
            unsigned short foreign_port, 
            const std::string& foreign_ip, 
            unsigned short local_port,
            const std::string& local_ip
        );

    public:

        ~connection();

        void* user_data;

        long write ( const char* buf, long num );

        long read ( char* buf, long num );

        long read ( char* buf, long num, unsigned long timeout );

        int get_local_port ( ) const { return connection_local_port; }

        int get_foreign_port ( ) const { return connection_foreign_port; }

        const std::string& get_local_ip ( ) const { return connection_local_ip; }

        const std::string& get_foreign_ip ( ) const { return connection_foreign_ip; }

        int shutdown_outgoing () 
        {
            sd_mutex.lock();
            if (sdo || sd)
            {
                sd_mutex.unlock();
                return sdr;
            }
            sdo = true;
            sdr = ::shutdown(connection_socket,SHUT_WR); 
            int temp = sdr;
            sd_mutex.unlock();
            return temp;  
        }

        int shutdown () 
        {
            sd_mutex.lock();
            if (sd)
            {
                sd_mutex.unlock();
                return sdr;
            }
            sd = true;
            sdr = ::shutdown(connection_socket,SHUT_RDWR); 
            int temp = sdr;
            sd_mutex.unlock();            
            return temp;
        }

        int disable_nagle();

        typedef int socket_descriptor_type;

        socket_descriptor_type get_socket_descriptor () const { return connection_socket; }

    private:

        bool readable ( unsigned long timeout ) const;
       
        bool sd_called ()const
        {
            //sd_mutex.lock();
            bool temp = sd;
            //sd_mutex.unlock();
            return temp;
        }

        bool sdo_called ()const
        {
            //sd_mutex.lock();
            bool temp = false;
            if (sdo || sd)
                temp = true;
            //sd_mutex.unlock();
            return temp;
        }

        // data members
        int connection_socket;
        const int connection_foreign_port;
        const std::string connection_foreign_ip; 
        const int connection_local_port;
        const std::string connection_local_ip;

        bool sd;  // called shutdown
        bool sdo; // called shutdown_outgoing
        int sdr; // return value for shutdown 
        std::mutex sd_mutex; // a lock for the three above vars

        connection(
            int sock,
            int foreign_port, 
            const std::string& foreign_ip, 
            int local_port,
            const std::string& local_ip
        ); 
        /*!
            requires
                - sock is a socket handle 
                - sock is the handle for the connection between foreign_ip:foreign_port 
                  and local_ip:local_port
            ensures
                - *this is initialized correctly with the above parameters
        !*/


        // restricted functions
        connection();
        connection(connection&);        // copy constructor
        connection& operator=(connection&);    // assignement opertor
    }; 

    class listener
    {
        // make the create_listener a friend of listener
        friend int create_listener (
            listener*& new_listener,
            unsigned short port,
            const std::string& ip
        );

    public:

        ~listener();

        int accept ( connection*& new_connection, unsigned long timeout = 0 );

        int accept ( std::unique_ptr<connection>& new_connection, unsigned long timeout = 0 );

        int get_listening_port ( ) const { return listening_port; }

        const std::string& get_listening_ip ( ) const { return listening_ip; }

    private:

        // data members
        int listening_socket;
        const int listening_port;
        const std::string listening_ip;
        const bool inaddr_any;

        listener(
            int sock,
            int port,
            const std::string& ip
        );

        // restricted functions
        listener();
        listener(listener&);        // copy constructor
        listener& operator=(listener&);    // assignement opertor
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

// ----------------------------------------------------------------------------------------

}

#ifdef NO_MAKEFILE
#include "sockets_kernel_2.cpp"
#endif

#endif // DLIB_SOCKETS_KERNEl_2_

