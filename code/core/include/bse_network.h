#pragma once


namespace bse
{
  enum class SocketType : s32
  {
    TCP = 1,
    UDP = 2
  };

  enum class NetworkServiceType : s32
  {
    SSH = 22,
    HTTP = 80,
    HTTPS = 443
  };

  constexpr u32 IPv4_ADDRESS_ANY = 0;
  constexpr u32 IPv4_ADDRESS_INVALID = U32_MAX;
  constexpr u16 PORT_ANY = 0;
  constexpr u16 PORT_INVALID = U16_MAX;
  constexpr s32 BSE_SOCKET_ERROR = -1;
  using Socket = s32;

  struct Ipv4Address
  {
    Ipv4Address() : _address( 0 ) {}
    Ipv4Address( Ipv4Address const& other ) : _address( other ) {}
    Ipv4Address( u32 address ) : _address( address ) {}
    union
    {
      u32 _address;
      struct
      {
        u8 addressPart0;
        u8 addressPart1;
        u8 addressPart2;
        u8 addressPart3;
      };
    };

    INLINE operator u32() const { return _address; }
  };

  struct Ipv4AddressWithPort
  {
    Ipv4AddressWithPort() : ipv4( 0 ), port( 0 ) {}
    Ipv4AddressWithPort( Ipv4AddressWithPort const& other ) : ipv4( other.ipv4 ), port( other.port ) {}
    Ipv4AddressWithPort( u32 address, u16 port ) : ipv4( address ), port( port ) {}

    Ipv4Address ipv4;
    u16 port;

    INLINE bool operator ==( Ipv4AddressWithPort const& other ) { return (ipv4 == other.ipv4 && port == other.port); }
    INLINE bool operator !=( Ipv4AddressWithPort const& other ) { return (ipv4 != other.ipv4 && port != other.port); }
  };

  struct Ipv6Address
  {
    Ipv6Address() : addressPart64bit0( 0 ), addressPart64bit1( 0 ) {}
    Ipv6Address( Ipv6Address const& other ) : addressPart64bit0( other.addressPart64bit0 ), addressPart64bit1( other.addressPart64bit1 ) {}
    Ipv6Address( char const* addressString )
    {
      //TODO
      BREAK;
    }
    Ipv6Address( u16 addressParts[] )
      : addressPart0( addressParts[0] )
      , addressPart1( addressParts[1] )
      , addressPart2( addressParts[2] )
      , addressPart3( addressParts[3] )
      , addressPart4( addressParts[4] )
      , addressPart5( addressParts[5] )
      , addressPart6( addressParts[6] )
      , addressPart7( addressParts[7] ) {}


    union
    {
      struct
      {
        u64 addressPart64bit0;
        u64 addressPart64bit1;
      };
      struct
      {
        u16 addressPart0;
        u16 addressPart1;
        u16 addressPart2;
        u16 addressPart3;
        u16 addressPart4;
        u16 addressPart5;
        u16 addressPart6;
        u16 addressPart7;
      };
      u16 addressPart[8];
    };
  };

  struct ResolveHostnameResult
  {
    constexpr static s32 MAX_RESOLVED_IPS = 8;
    ResolveHostnameResult() : ipv4AddressCount( 0 ), ipv6AddressCount( 0 ) {}
    Ipv4Address ipv4Addresses[MAX_RESOLVED_IPS];
    Ipv6Address ipv6Addresses[MAX_RESOLVED_IPS];
    s32 ipv4AddressCount;
    s32 ipv6AddressCount;
  };

  Socket socket_create_tcp();
  Socket socket_create_udp();
  void socket_destroy( Socket socket );
  bool socket_bind( Socket socket, Ipv4AddressWithPort const& ipv4Address );
  bool socket_listen( Socket socket );
  bool socket_accept( Socket socket, Socket* out_socket, Ipv4AddressWithPort* out_remoteAddressOptional );
  bool socket_connect( Socket socket, Ipv4AddressWithPort const& ipv4Address );
  bool socket_connect( Socket socket, char const* hostname, u16 port );
  bool socket_send( Socket socket, char const* data, s32 size );
  bool socket_receive( Socket socket, char* receiveBuffer, s32 receiveBufferSize, s32* out_bytesReceived );
  u32 parse_ipv4( char const* from );

  template<> INLINE s32 string_format_internal<Ipv4Address>( char* destination, s32 capacity, Ipv4Address address );
  template<> INLINE s32 string_format_internal<Ipv4AddressWithPort>( char* destination, s32 capacity, Ipv4AddressWithPort connection );
};



