#include "monkeyofficerelay.h"

#include "network/bse_openssl.h"


void fetch_lehmwelt( SSL_CTX* sslContext );
void monkey_office_connect( SSL_CTX* sslContext );

void initialize_platform( bse::PlatformInitParams* initParameters )
{
  //called once before the platform is fully initialized to set init parameters
}

struct UnencryptedConnection
{
  bse::Socket socket;
  bse::Ipv4AddressWithPort peerAddress;
};

bool connect( char const* hostname, u16 port, UnencryptedConnection* out_connection )
{
  using namespace bse;
  bse::Socket socket = socket_create_tcp();
  if ( socket == BSE_SOCKET_ERROR )
  {
    log_warning( "openssl::connect() failed, unable to create socket." );
    return false;
  }

  ResolveHostnameResult hostnameResolution;
  platform->dns_resolve_hostname( hostname, &hostnameResolution );

  if ( !hostnameResolution.ipv4AddressCount )
  {
    log_warning( "openssl::connect() failed, unable to resolve hostname: ", hostname );
    socket_destroy( socket );
    return false;
  }
  Ipv4AddressWithPort connection { hostnameResolution.ipv4Addresses[0], port };
  if ( hostnameResolution.ipv4AddressCount > 1 || hostnameResolution.ipv6AddressCount > 1 )
  {
    BREAK;
  }

  if ( !socket_connect( socket, connection ) )
  {
    log_warning( "openssl::connect() failed, unable to connect to hostname: ", hostname, " at ", connection );
    socket_destroy( socket );
    return false;
  }

  *out_connection = { socket, connection };
  return true;
}


void initialize_app_data()
{
  //called once after the platform is fully initialized for you to initialize your app data
  using namespace bse;

  openssl::init();
  SSL_CTX* context = bse::openssl::create_context_tls();

  //fetch_lehmwelt( context );
  monkey_office_connect( context );

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

void monkey_office_connect( SSL_CTX* sslContext )
{
  using namespace bse;

  char const* hostname = "127.0.0.1";

  UnencryptedConnection connection {};
  if ( !connect( hostname, 8084, &connection ) )
  {
    BREAK;
  }

  char jsonContent[] = "{ \"apiInfoGet\":\"\"}";

  char buffer[BSE_STACK_BUFFER_MEDIUM];
  int written = string_format( buffer, BSE_STACK_BUFFER_MEDIUM
  , "POST /monkeyOfficeConnectJSON HTTP/1.1\r\n"
 // , "mbl-ident: testfirma\r\n"
  , "Authorization: Basic Y2FtaWxsbzpsdWtlc2No\r\n"
  , "Content-Type: application/json\r\n"
  , "Content-Length: ", array_count( jsonContent ), "\r\n\r\n"
  , jsonContent, "\r\n" );

  log_info( buffer );
  log_info( "\r\n\r\n" );
  if ( socket_send( connection.socket, buffer, written ) )
  {
    while ( 1 )
    {
      s32 bytesReceived;
      socket_receive( connection.socket, buffer, array_count( buffer ), &bytesReceived );
      if ( bytesReceived > 0 )
      {
        log_info( buffer );
      }
      else if ( bytesReceived < 0 )
      {
        BREAK;
        break;
      }
      else
      {
        break;
      }

    }
  }

}

void fetch_lehmwelt( SSL_CTX* sslContext )
{
  using namespace bse;

  char const* hostname = "lehmwelt.at";

  openssl::TLSConnection connection {};

  if ( !openssl::connect( sslContext, hostname, 443, &connection ) )
  {
    BREAK;
  }

  //TODO certificates and trust

  char buffer[BSE_STACK_BUFFER_MEDIUM];

  int written = string_format( buffer, BSE_STACK_BUFFER_MEDIUM, "GET /shop/index.php HTTP/1.1\r\n", "HOST: ", hostname, "\r\n", "Connection: close\r\n\r\n" );
  if ( SSL_write( connection.ssl, buffer, written ) != BSE_SOCKET_ERROR )
  {
    while ( 1 )
    {
      int bytesReceived = SSL_read( connection.ssl, buffer, array_count( buffer ) );
      if ( bytesReceived > 0 )
      {
        log_info( buffer );
      }
      else if ( bytesReceived < 0 )
      {
        BREAK;
        break;
      }
      else
      {
        break;
      }

    }
  }

  openssl::disconnect( &connection );
}