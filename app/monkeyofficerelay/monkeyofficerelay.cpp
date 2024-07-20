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

  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();

  char const* hostname = "80.77.17.12";
  bse::Ipv4Address address( parse_ipv4( hostname ), 443 );

  SSL_CTX* context = bse::openssl::create_context();

  bse::Socket socket = bse::socket_create_tcp();

  if ( socket_connect( socket, address ) )
  {
    SSL* ssl = SSL_new( context );
    if ( !SSL_set_tlsext_host_name( ssl, hostname ) )
    {
      BREAK;
    }

    SSL_set_fd( ssl, socket );
    if ( SSL_connect( ssl ) == -1 )
    {
      openssl::log_last_errors();
      BREAK;
    }

    // char request[512] = {};
    // string_format( request, array_count( request ), "HTML" );

    // if ( socket_send( socket, request, array_count( request ) ) )
    // {
    //   while ( true )
    //   {
    //     //socket_receive/()
    //   }
    // }
  }

  EVP_cleanup();
}

void on_reload()
{
  //called once every time you hot reload
}

void tick()
{
  //called once every frame
}

