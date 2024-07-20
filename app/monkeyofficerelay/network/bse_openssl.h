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
    [[nodiscard]] SSL_CTX* create_context();

    void log_last_errors();
  };
};



namespace bse
{
  namespace openssl
  {
    int _error_callback( char const* string, size_t length, void* u )
    {
      log_error( string );
      return 0;
    }
    void log_last_errors()
    {
      ERR_print_errors_cb( _error_callback, nullptr );
    }

    SSL_CTX* create_context()
    {
      SSL_CTX* result = SSL_CTX_new( TLS_client_method() );
      if ( !result )
      {
        ERR_print_errors_cb( _error_callback, nullptr );
      }

      return result;
    }
  };
};