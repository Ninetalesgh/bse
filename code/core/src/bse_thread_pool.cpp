#include "bse_core.h"


namespace bse
{
  int test3( int b )
  {
    int a = 4;
    a = b + a;
    return a;
  }

  int test2( int a )
  {
    return a + 2;
  }

  int test1( int a )
  {
    return a + 1;
  }

  struct StaticFunctionRegistry
  {
    StaticFunctionRegistry* nextType;
  };

  static StaticFunctionRegistry sfr;

  template<typename ReturnType, typename... Args> struct StaticFunction : StaticFunctionRegistry
  {
    using signature = ReturnType( Args... );

    StaticFunction()
    {
      nextType = sfr.nextType;
      sfr.nextType = this;
    }

    static StaticFunction<ReturnType, Args...>& get_static_instance() { static StaticFunction<ReturnType, Args...> staticInstance;  return staticInstance; }
    Array<signature*> fns;

    struct Pointer
    {
      Pointer( s32 i ) : index( i ) {}
      ReturnType operator()( Args... args )
      {
        return get_static_instance().fns[index]( args... );
      }
      s32 index;
      s32 something;
    };

    static void register_function( signature* ptr )
    {
      signature* converted = (signature*) ((void*) ptr);
      get_static_instance().fns.push( converted );
    }

    static Pointer make( signature* fn )
    {
      auto& fns = get_static_instance().fns;
      s32 i = 0;
      for ( ; i < fns.count; ++i )
      {
        if ( (void*) (fn) == (void*) (fns[i]) )
        {
          return Pointer( i );
        }
      }

      log_error( "function was not previously registered, registering it now, but that might have unintended consequences, please register it explicitly." );
      register_function( fn );

      return Pointer( i );
    }
  };

  using IntInt = StaticFunction<int, int>;




  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Static Registry ///////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////




  struct What
  {
    void doit( int b )
    {
      a = b;
    }
    int a;
    void doti( int oo )
    {
      a = 2 * oo;
    }

    void operator()( int p ) { return doit( p ); }
  };

  void threadtest()
  {

    register_function( &What::doit );
    register_function( &What::doti );

    What aaaa;
    auto ga = []( int a ) {return int( 2 );};

    auto fun1 = make_function( &What::doit );
    auto fun2 = make_function( ga );
    auto fun = make_function( &test1 );

    fun1( &aaaa, 3 );
    fun2( 4 );
    // fun( 5 );
    // int a = 2;

    //make_function( []( int a ) {return int( 2 );} );

    //auto g = make_function( &What::doit );

    //g = &What::doti;




  }

};