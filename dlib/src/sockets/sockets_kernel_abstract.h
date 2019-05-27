#ifndef DLIB_SOCKETS_KERNEl_ABSTRACT_
#define DLIB_SOCKETS_KERNEl_ABSTRACT_

#include <string>
#include <thread>

namespace dlib
{

    int get_local_hostname (
        std::string& hostname
    );
 
    int hostname_to_ip (
        const std::string& hostname,
        std::string& ip,
        int n = 0
    );
 
    int ip_to_hostname (
        const std::string& ip,
        std::string& hostname
    );
 
    int create_listener (
        listener*& new_listener,
        unsigned short port,
        const std::string& ip = ""
    );
    
    int create_listener (
        std::unique_ptr<listener>& new_listener,
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

    int create_connection ( 
        std::unique_ptr<connection>& new_connection,
        unsigned short foreign_port, 
        const std::string& foreign_ip, 
        unsigned short local_port = 0,
        const std::string& local_ip = ""
    );

    class connection
    {
    public:

        ~connection (
        );

        void* user_data;

        long write (
            const char* buf, 
            long num
        );
        
        long read (
            char* buf, 
            long num
        );
    
        long read (
            char* buf, 
            long num,
            unsigned long timeout 
        );
        
        unsigned short get_local_port (
        ) const;
        
        unsigned short get_foreign_port ( 
        ) const;
        
		const std::string& get_local_ip (
        ) const;
        
		const std::string& get_foreign_ip (
        ) const;
        
        int shutdown (
        );
        
		int shutdown_outgoing (
        );
        
        int disable_nagle(
        );
        
        typedef platform_specific_type socket_descriptor_type;
        socket_descriptor_type get_socket_descriptor (
        ) const;
        
    private:
        connection();
        connection(connection&);        // copy constructor
        connection& operator=(connection&);    // assignment operator

    }; 

    class listener
    {
    public:

        ~listener (
        );

        int accept (
            connection*& new_connection,
            unsigned long timeout = 0
        );
        
        int accept (
            std::unique_ptr<connection>& new_connection,
            unsigned long timeout = 0
        );
        
        unsigned short get_listening_port (
        ) const;
        
        const std::string& get_listening_ip (
        ) const;
        
    private:
        listener();
        listener(listener&);        // copy constructor
        listener& operator=(listener&);    // assignment operator
    };
}

#endif // DLIB_SOCKETS_KERNEl_ABSTRACT_