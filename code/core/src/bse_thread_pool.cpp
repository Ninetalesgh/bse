#include "bse_core.h"

#include <type_traits>

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
  ////////// Function Pointers /////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ///// Heavily inspired by std::function, many thanks <3.           ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename ReturnType, typename... Args> struct _FuncClass
  {
    template <class F, class _Function>
    using _EnableIfCallableType = std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<F>, _Function>>, std::_Is_invocable_r<ReturnType, F, Args...>>, int>;
    using Signature = ReturnType( * )(Args...);

    INLINE ReturnType operator()( Args... args ) const
    {
      return std::invoke( ptr, std::forward<Args>( args )... );
    }

    union
    {
      Signature ptr;
      struct { s32 index; s32 padding; };
    };
  };

  template <typename ReturnType, typename T, typename... Args> struct _MemberFuncClass
  {
    template <class F, class _Function>
    using _EnableIfCallableType = std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<F>, _Function>>, std::_Is_invocable_r<ReturnType, F, T, Args...>>, int>;
    using Signature = ReturnType( T::* )(Args...);
    using SignatureWithoutClass = ReturnType( Args... );

    INLINE ReturnType operator()( T* obj, Args... args ) const
    {
      return std::invoke( ptr, std::forward<T*>( obj ), std::forward<Args>( args )... );
    }

    union
    {
      Signature ptr;
      struct { s32 index; s32 padding; };
    };
  };

  template <typename T> struct _GetFunc { static_assert(std::_Always_false<T>, "Function does not accept non-function types as template arguments."); };
  #define _GET_FUNCTION_IMPL(CALL_OPT, X1, X2, X3) template <typename R, typename Arg, typename... Args> struct _GetFunc<R CALL_OPT(Arg, Args...)> { using type = _FuncClass<R, Arg, Args...>; };
  _NON_MEMBER_CALL( _GET_FUNCTION_IMPL, X1, X2, X3 );
  #undef _GET_FUNCTION_IMPL

  template <typename R, typename Arg, typename... Args> struct _GetFunc<R( __thiscall Arg::* )(Args...)> { using type = _MemberFuncClass<R, Arg, Args...>; };
  template <typename R, typename Arg, typename... Args> struct _GetFunc<R( __thiscall Arg::* )(Args...) const> { using type = _MemberFuncClass<R, Arg, Args...>; };

  template<typename F> struct Function : public _GetFunc<F>::type
  {
    using Super = typename _GetFunc<F>::type;
    Function() { ptr = nullptr; }
    Function( Function<F> const& other ) { ptr = other.ptr; }
    Function( Function<F>&& other ) { ptr = other.ptr; other.ptr = nullptr; }

    template<typename T, class = typename Super::_EnableIfCallableType<T&, Function>>
    Function( T func )
    {
      ptr = (Signature) func;
    }

    template<typename T, class = typename Super::_EnableIfCallableType<T&, Function>>
    Function<F>& operator=( T&& func ) { ptr = (Signature) func; return *this; }
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Make Function Pointers ////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template <bool Enabled, typename F> struct _FunctionTypeReduction {};
  template<typename F> struct _FunctionTypeReduction<true, F> { using type = typename Function<typename std::remove_pointer<F>::type>; };
  template<typename F> struct _FunctionTypeReduction<false, F> { using type = typename Function<typename _GetFunc<decltype(&std::remove_reference<F>::type::operator())>::type::SignatureWithoutClass>; };
  template<typename F> using _ReducedFunctionType = typename _FunctionTypeReduction<std::disjunction_v<std::is_member_function_pointer<F>, std::is_function<typename std::remove_pointer<F>::type>>, F >::type;
  template<typename F> _ReducedFunctionType<F> make_function( F&& functor ) { return { std::forward<F>( functor ) }; }





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

    What aaaa;
    auto ga = []( int a ) {return int( 2 );};

    auto fun1 = make_function( &What::doit );
    auto fun2 = make_function( ga );
    auto fun = make_function( &test1 );

    fun1( &aaaa, 3 );
    fun2( 4 );
    fun( 5 );
    int a = 2;

    //make_function( []( int a ) {return int( 2 );} );

    //auto g = make_function( &What::doit );

    //g = &What::doti;




  }

};