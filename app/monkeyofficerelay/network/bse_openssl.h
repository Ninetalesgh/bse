#pragma once

#include "bse_core.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

namespace bse
{
  namespace openssl
  {
    bool init();
    void cleanup();

    [[nodiscard]] SSL_CTX* create_context_tls();

    void log_last_errors();

    struct TLSConnection
    {
      bse::Socket socket;
      SSL* ssl;
      bse::Ipv4AddressWithPort peer;
    };

    //SSL* connect( SSL_CTX* context, bse::Ipv4AddressWithPort const& remoteAddress );
    bool connect( SSL_CTX* context, char const* hostname, u16 port, TLSConnection* out_connection );
    void disconnect( TLSConnection* connection );
  };
};

namespace bse
{
  namespace openssl
  {
    int _error_callback( char const* string, size_t length, void* u )
    {
      log_warning( string );
      return 0;
    }
    void log_last_errors()
    {
      ERR_print_errors_cb( _error_callback, nullptr );
    }

    void* _alloc_callback( size_t size, const char* file, int line )
    {
      return memory::allocate_thread_safe_dont_remember_size( size );
    }
    void _free_callback( void* ptr, const char* file, int line )
    {
      if ( ptr )
      {
        memory::free_thread_safe_dont_remember_size( ptr );
      }
    }
    void* _realloc_callback( void* existing, size_t size, const char* file, int line )
    {
      return memory::reallocate_thread_safe_dont_remember_size( existing, size );
    }

    bool init()
    {
      if ( !CRYPTO_set_mem_functions( _alloc_callback, _realloc_callback, _free_callback ) )
      {
        log_error( "openssl::init() failed, make sure this is the first openssl call in your program." );
        return false;
      }

      SSL_library_init();
      SSL_load_error_strings();
      OpenSSL_add_ssl_algorithms();

      return true;
    }

    void cleanup()
    {
      OPENSSL_cleanup();
    }

    SSL_CTX* create_context_tls()
    {
      SSL_CTX* result = SSL_CTX_new( TLS_client_method() );
      if ( !result )
      {
        openssl::log_last_errors();
      }

      return result;
    }

    bool connect( SSL_CTX* context, char const* hostname, u16 port, TLSConnection* out_connection )
    {
      bse::Socket socket = socket_create_tcp();
      if ( socket == BSE_NETWORK_ERROR )
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

      SSL* ssl = SSL_new( context );

      if ( !ssl )
      {
        openssl::log_last_errors();
        socket_destroy( socket );
      }

      if ( !SSL_set_fd( ssl, socket ) || SSL_connect( ssl ) == BSE_NETWORK_ERROR )
      {
        openssl::log_last_errors();
        SSL_free( ssl );
        socket_destroy( socket );
      }

      log_info( "openssl::connect() succeeded, connected to '", hostname, "' at address '", connection.ipv4, "' with cipher ", SSL_get_cipher( ssl ) );
      *out_connection = { socket, ssl, connection };
      return true;
    }

    void disconnect( TLSConnection* connection )
    {
      if ( connection->ssl )
      {
        SSL_shutdown( connection->ssl );
        SSL_free( connection->ssl );
      }
      socket_destroy( connection->socket );
    }

  };
};