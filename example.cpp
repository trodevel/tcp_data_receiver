/*

TCP Data Receiver example

Copyright (C) 2016 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 3219 $ $Date:: 2016-01-20 #$ $Author: serge $

#include <iostream>
#include <thread>               // std::thread
#include <functional>           // std::bind
#include <sstream>              // std::stringstream

#include "tcp_data_receiver.h"  // tcp_data_receiver
#include "i_data_receiver.h"    // tcp_server::IDataReceiver

class Receiver: public tcp_data_receiver::IDataReceiver
{
public:
    Receiver():
        total_( 0 ), s_( nullptr )
    {
    }

    void init( tcp_data_receiver::Server  * s )
    {
        s_  = s;
    }

    virtual void on_received( uint32_t size, const char *data )
    {
        total_ += size;

        std::cout << "received " << size << " bytes, total " << total_ << std::endl;

        std::string input( data, size );

        std::string cmd;

        std::stringstream stream( input );
        if( stream >> cmd )
        {
            if( cmd == "shutdown" || cmd == "stop" )
            {
                std::cout << "got shutdown" << std::endl;
                s_->shutdown();
            }
        }
   }

private:
    int                 total_;
    tcp_data_receiver::Server  * s_;
};

int main( int argc, char* argv[] )
{
    try
    {
        if( argc != 2 )
        {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 1;
        }

        Receiver r;

        tcp_data_receiver::Server s;

        r.init( &s );
        s.init( &r, atoi( argv[1] ) );

        auto t = std::thread( std::bind( &tcp_data_receiver::Server::worker, &s ) );

        t.join();

    } catch( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
