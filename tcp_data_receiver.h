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

#ifndef TCP_DATA_RECEIVER_H
#define TCP_DATA_RECEIVER_H

#include <boost/asio.hpp>

namespace tcp_data_receiver
{

class IDataReceiver;

class Session;

class Server
{
public:
    Server();
    ~Server();

    bool init( IDataReceiver * receiver, unsigned short port );
    void shutdown();
    void worker();

private:
    void start_accept();

    void handle_accept( Session* session, const boost::system::error_code& error );

private:
    bool                            is_inited_;

    boost::asio::io_service         io_service_;
    boost::asio::ip::tcp::acceptor  * acceptor_;

    IDataReceiver                   * receiver_;
};

} // namespace tcp_data_receiver

#endif  // TCP_DATA_RECEIVER_H
