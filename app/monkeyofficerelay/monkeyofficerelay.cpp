#include "monkeyofficerelay.h"

#include "network/bse_openssl.h"


void initialize_platform( bse::PlatformInitParams* initParameters )
{
  //called once before the platform is fully initialized to set init parameters
}

void initialize_app_data()
{
  //called once after the platform is fully initialized for you to initialize your app data
  using namespace bse;


  openssl::init();

  char const* hostname = "lehmwelt.at";
  bse::Ipv4AddressWithPort address( parse_ipv4( hostname ), 443 );

  SSL_CTX* context = bse::openssl::create_context_tls();

  openssl::TLSConnection connection {};

  if ( !openssl::connect( context, hostname, 443, &connection ) )
  {
    BREAK;
  }

  //TODO certificates and trust

  char buffer[BSE_STACK_BUFFER_MEDIUM];

  int written = string_format( buffer, BSE_STACK_BUFFER_MEDIUM, "GET / HTTP/1.1\r\n", "HOST: ", hostname, "\r\n", "Connection: close\r\n\r\n" );
  if ( SSL_write( connection.ssl, buffer, written ) != BSE_SOCKET_ERROR )
  {
    while ( 1 )
    {
      int bytesReceived = SSL_read( connection.ssl, buffer, array_count( buffer ) );
      if ( bytesReceived < 0 )
      {
        BREAK;
        break;
      }

      log_info( buffer );
    }
  }

  openssl::disconnect( &connection );
  openssl::cleanup();
}

void on_reload()
{
  //called once every time you hot reload
}

void tick()
{
  //called once every frame
}

