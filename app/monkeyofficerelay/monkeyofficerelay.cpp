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

