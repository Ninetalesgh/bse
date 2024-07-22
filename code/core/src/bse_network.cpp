#include "core/bse_core.h"

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

  bool socket_bind( Socket socket, Ipv4AddressWithPort const& ipv4Address )
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

  bool socket_accept( Socket socket, Socket* out_socket, Ipv4AddressWithPort* out_remoteAddressOptional )
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

  bool socket_connect( Socket socket, Ipv4AddressWithPort const& ipv4Address )
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

  bool socket_connect( Socket socket, char const* hostname, u16 port )
  {
    bse::Vector<Ipv4Address> addresses;
    bse::Vector<Ipv6Address> addresses6;
    ResolveHostnameResult hostnameResult;
    if ( platform->dns_resolve_hostname( hostname, &hostnameResult ) )
    {
      if ( addresses.size() )
      {
        return socket_connect( socket, { addresses[0], port } );
      }
    }

    return false;
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

  template<> INLINE s32 string_format_internal<Ipv4AddressWithPort>( char* destination, s32 capacity, Ipv4AddressWithPort connection )
  {
    return string_format( destination, capacity, connection.ipv4.addressPart0, ".", connection.ipv4.addressPart1, ".",
                              connection.ipv4.addressPart2, ".", connection.ipv4.addressPart3,
                              ":", connection.port ) - 1;
  }

  template<> INLINE s32 string_format_internal<Ipv4Address>( char* destination, s32 capacity, Ipv4Address address )
  {
    return string_format( destination, capacity, address.addressPart0, ".", address.addressPart1, ".",
                              address.addressPart2, ".", address.addressPart3 ) - 1;
  }
};