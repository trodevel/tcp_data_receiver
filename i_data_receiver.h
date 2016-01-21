/*

Data Receiver interface

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

// $Revision: 3224 $ $Date:: 2016-01-20 #$ $Author: serge $


#ifndef TCP_DATA_RECEIVER_I_DATA_RECEIVER_H
#define TCP_DATA_RECEIVER_I_DATA_RECEIVER_H

#include <cstdint>              // uint32_t

namespace tcp_data_receiver
{

class IDataReceiver
{
public:
    virtual ~IDataReceiver() {};

    virtual void on_received( uint32_t size, const char *data )  = 0;
};

} // namespace tcp_data_receiver

#endif  // TCP_DATA_RECEIVER_I_DATA_RECEIVER_H
