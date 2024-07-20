#pragma once


namespace bse
{
  enum class SocketType : s32
  {
    TCP = 1,
    UDP = 2
  };

  constexpr u32 IPv4_ADDRESS_ANY = 0;
  constexpr u32 IPv4_ADDRESS_INVALID = U32_MAX;
  constexpr u16 PORT_ANY = 0;
  constexpr u16 PORT_INVALID = U16_MAX;
  constexpr s32 BSE_SOCKET_ERROR = -1;
  using Socket = s32;
  struct Ipv4Address
  {
    Ipv4Address() : address( 0 ), port( 0 ) {}
    Ipv4Address( Ipv4Address const& other ) : address( other.address ), port( other.port ) {}
    Ipv4Address( u32 address, u16 port ) : address( address ), port( port ) {}
    union
    {
      u32 address;
      struct
      {
        u8 addressPart0;
        u8 addressPart1;
        u8 addressPart2;
        u8 addressPart3;
      };
    };
    u16 port;

    INLINE u32 operator ==( Ipv4Address const& other ) { return (address == other.address && port == other.port); }
    INLINE u32 operator !=( Ipv4Address const& other ) { return (address != other.address && port != other.port); }
  };

  Socket socket_create_tcp();
  Socket socket_create_udp();
  void socket_destroy( Socket socket );
  bool socket_bind( Socket socket, Ipv4Address const& ipv4Address );
  bool socket_listen( Socket socket );
  bool socket_accept( Socket socket, Socket* out_socket, Ipv4Address* out_remoteAddressOptional );
  bool socket_connect( Socket socket, Ipv4Address const& ipv4Address );
  bool socket_send( Socket socket, char const* data, s32 size );
  bool socket_receive( Socket socket, char* receiveBuffer, s32 receiveBufferSize, s32* out_bytesReceived );


  u32 parse_ipv4( char const* from );
  template<> INLINE s32 string_format_internal<Ipv4Address>( char* destination, s32 capacity, Ipv4Address connection );
};



