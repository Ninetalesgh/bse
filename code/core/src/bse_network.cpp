#include "bse_core.h"

namespace bse
{
  Socket socket_create_tcp()
  {
    return platform->socket_create( SocketType::TCP );
  }

  Socket socket_create_udp()
  {
    return platform->socket_create( SocketType::UDP );
  }

  void socket_destroy( Socket socket )
  {
    platform->socket_destroy( socket );
  }

  bool socket_bind( Socket socket, Ipv4Address const& ipv4Address )
  {
    if ( !platform->socket_bind( socket, ipv4Address ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }

  bool socket_listen( Socket socket )
  {
    if ( !platform->socket_listen( socket ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }

  bool socket_accept( Socket socket, Socket* out_socket, Ipv4Address* out_remoteAddressOptional )
  {
    if ( !platform->socket_accept( socket, out_socket, out_remoteAddressOptional ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }

  bool socket_connect( Socket socket, Ipv4Address const& ipv4Address )
  {
    if ( !platform->socket_connect( socket, ipv4Address ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }

  bool socket_send( Socket socket, char const* data, s32 size )
  {
    if ( !platform->socket_send( socket, data, size ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }

  bool socket_receive( Socket socket, char* receiveBuffer, s32 receiveBufferSize, s32* out_bytesReceived )
  {
    if ( !platform->socket_receive( socket, receiveBuffer, receiveBufferSize, out_bytesReceived ) )
    {
      char errorMessageBuffer[BSE_STACK_BUFFER_SMALL];
      platform->socket_get_last_error_message( errorMessageBuffer, array_count( errorMessageBuffer ) );
      BREAK;
      log_warning( errorMessageBuffer );
      return false;
    }
    return true;
  }




  u32 parse_ipv4( char const* from )
  {
    u32 result = 0;
    char const* reader = from;

    for ( s32 i = 0; i < 4; ++i )
    {
      char const* section = reader;
      s32 digit = 0;
      while ( *reader != '.' && *reader != '\0' )
      {
        ++reader;
        if ( ++digit > 3 ) return 0;
      }
      ++reader;

      u8 multiplier = 1;
      u8 u8result = 0;
      for ( ;digit--; multiplier *= 10 )
      {
        u8result += multiplier * (section[digit] - '0');
      }

      result += u32( u8result ) << (i * 8);
    }

    return result;
  }


  template<> INLINE s32 string_format_internal<Ipv4Address>( char* destination, s32 capacity, Ipv4Address connection )
  {
    return string_format( destination, capacity, connection.addressPart0, ".", connection.addressPart1, ".",
                              connection.addressPart2, ".", connection.addressPart3,
                              ":", connection.port ) - 1;
  }

};