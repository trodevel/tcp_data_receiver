/*

TCP Data Receiver

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

// $Revision: 3225 $ $Date:: 2016-01-20 #$ $Author: serge $

#include "tcp_data_receiver.h"      // self

#include <functional>               // std::bind
#include <boost/asio.hpp>

#include "i_data_receiver.h"        // IDataReceiver

namespace tcp_data_receiver
{

using boost::asio::ip::tcp;

class Session
{
public:
    Session( boost::asio::io_service& io_service, IDataReceiver * receiver ) :
            socket_( io_service ), receiver_( receiver )
    {
    }

    tcp::socket& get_socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.async_read_some( boost::asio::buffer( data_, max_length ),
                std::bind( &Session::handle_read, this, std::placeholders::_1, std::placeholders::_2 ) );
    }

private:
    void handle_read( const boost::system::error_code& error, size_t bytes_transferred )
    {
        if( !error )
        {
            if( receiver_ )
                receiver_->on_received( bytes_transferred, data_ );

            socket_.async_read_some( boost::asio::buffer( data_, max_length ),
                    std::bind( &Session::handle_read, this, std::placeholders::_1, std::placeholders::_2 ) );
        }
        else
        {
            if( receiver_ )
                receiver_->on_received( 0, nullptr );

            delete this;
        }
    }

private:

    static const int max_length = 1024;

    tcp::socket     socket_;
    char            data_[max_length];

    IDataReceiver   * receiver_;
};

Server::Server() :
        is_inited_( false ), acceptor_( nullptr ), receiver_( nullptr )
{
}

Server::~Server()
{
    if( is_inited_ )
    {
        delete acceptor_;
    }
}

bool Server::init( IDataReceiver * receiver, unsigned short port )
{
    if( is_inited_ )
        throw std::runtime_error( "already inited" );

    if( receiver == nullptr )
        throw std::runtime_error( "receiver is null" );

    acceptor_   = new boost::asio::ip::tcp::acceptor( io_service_, tcp::endpoint( tcp::v4(), port ) );
    receiver_   = receiver;
    is_inited_  = true;

    start_accept();

    return true;
}

void Server::shutdown()
{
    if( is_inited_ == false )
        throw std::runtime_error( "not inited" );

    io_service_.stop();
}

void Server::worker()
{
    if( is_inited_ == false )
        throw std::runtime_error( "not inited" );

    io_service_.run();
}

void Server::start_accept()
{
    Session* session = new Session( io_service_, receiver_ );
    acceptor_->async_accept( session->get_socket(),
            std::bind( &Server::handle_accept, this, session, std::placeholders::_1 ) );
}

void Server::handle_accept( Session* session, const boost::system::error_code& error )
{
    if( !error )
    {
        session->start();
    }
    else
    {
        delete session;
    }

    start_accept();
}

} // namespace tcp_data_receiver
