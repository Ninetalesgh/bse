#pragma once

#include "bse_common.h"
#include <type_traits>

namespace bse
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Function Types for Debug/Development //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ///// I want function pointers in dlls to sustain through          ///////////////////////////////////
  ///// hot reloads, hence this mess.                                ///////////////////////////////////
  ///// Scroll down for register_function() and make_function().     ///////////////////////////////////
  ///// You want to call register_function of your functions         ///////////////////////////////////
  ///// on dll load and keep them in order if you add new ones,      ///////////////////////////////////
  ///// in the release build this is all compiled away.              ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
  struct _StaticFunctionTypeRegister
  {
    //this linked list might not be necessary, but for now it keeps all types together and might make debugging easier
    static _StaticFunctionTypeRegister& get() { static _StaticFunctionTypeRegister instance; return instance; }
    _StaticFunctionTypeRegister* next;
    constexpr static s32 MAX_POINTERS_PER_TYPE = 128;
  };
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Preprocessor defines //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ///// Different compilers complain in different ways, I suppose.   ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if defined (BSE_PLATFORM_WINDOWS)
  # define BSE_STD_IS_INVOKABLE std::_Is_invocable_r
  # define BSE_STD_INVOKE std::invoke
  #elif defined (BSE_PLATFORM_ANDROID)
  # define BSE_STD_IS_INVOKABLE std::is_invocable_r
  # define BSE_STD_INVOKE std::__invoke
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Function Pointers /////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ///// Heavily inspired by std::function, many thanks <3.           ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename ReturnType, typename... Args> struct _FuncClass
  {
    using Signature = ReturnType( * )(Args...);

    #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
    struct _TypeRegister : _StaticFunctionTypeRegister
    {
      _TypeRegister() : ptrCount( 0 )
      {
        auto& base = get();
        this->next = base.next;
        base.next = this;
      }
      s32 ptrCount;
      Signature ptrs[MAX_POINTERS_PER_TYPE];
      Signature get_ptr_by_index( s32 index ) { assert( index < ptrCount );  return ptrs[index]; }
      s32 add_ptr( Signature _ptr ) { assert( ptrCount < MAX_POINTERS_PER_TYPE ); ptrs[ptrCount] = _ptr; return ptrCount++; }
      s32 get_index_for_ptr( Signature _ptr ) { for ( s32 i = 0; i < ptrCount; ++i ) { if ( ptrs[i] == _ptr ) return i; } BREAK; return add_ptr( _ptr ); }
    };
    static _TypeRegister& get_type_register() { static _TypeRegister instance; return instance; }
    #endif

    template <class F, class _Function>
    using _EnableIfCallableType = std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<F>, _Function>>, BSE_STD_IS_INVOKABLE<ReturnType, F, Args...>>, int>;

    INLINE ReturnType operator()( Args... args ) const
    {
      #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
      //TODO get right module via moduleId here
      Signature actualPtr = get_type_register().get_ptr_by_index( hotReloadIndex );
      return BSE_STD_INVOKE( actualPtr, std::forward<Args>( args )... );
      #else
      return BSE_STD_INVOKE( ptr, std::forward<Args>( args )... );
      #endif
    }

    union
    {
      Signature ptr;
      struct { s32 hotReloadIndex; s32 moduleId; };
    };
  };

  template <typename ReturnType, typename T, typename... Args> struct _MemberFuncClass
  {
    using Signature = ReturnType( T::* )(Args...);

    #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
    struct _TypeRegister : _StaticFunctionTypeRegister
    {
      _TypeRegister() : ptrCount( 0 )
      {
        auto& base = get();
        this->next = base.next;
        base.next = this;
      }
      s32 ptrCount;
      Signature ptrs[MAX_POINTERS_PER_TYPE];
      Signature get_ptr_by_index( s32 index ) { assert( index < ptrCount );  return ptrs[index]; }
      s32 add_ptr( Signature _ptr ) { ptrs[ptrCount] = _ptr; return ptrCount++; }
      s32 get_index_for_ptr( Signature _ptr ) { for ( s32 i = 0; i < ptrCount; ++i ) { if ( ptrs[i] == _ptr ) return i; } BREAK; return add_ptr( _ptr ); }
    };
    static _TypeRegister& get_type_register() { static _TypeRegister instance; return instance; }
    #endif

    template <class F, class _Function>
    using _EnableIfCallableType = std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<F>, _Function>>, BSE_STD_IS_INVOKABLE<ReturnType, F, T, Args...>>, int>;
    using _SignatureWithoutClass = ReturnType( Args... );

    INLINE ReturnType operator()( T* obj, Args... args ) const
    {
      #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
      //TODO get right module via moduleId here
      Signature actualPtr = get_type_register().get_ptr_by_index( hotReloadIndex );
      return BSE_STD_INVOKE( actualPtr, std::forward<T*>( obj ), std::forward<Args>( args )... );
      #else
      return BSE_STD_INVOKE( ptr, std::forward<T*>( obj ), std::forward<Args>( args )... );
      #endif
    }

    union
    {
      Signature ptr;
      struct { s32 hotReloadIndex; s32 moduleId; };
    };
  };


  //this calling convention fiddling might be entirely redundant, I'll leave it here for now.
  template <typename T> struct _GetFunc { static_assert(BSE_ALWAYS_FALSE( T ), "Function does not accept non-function types as template arguments."); };

  template <typename R, typename... Args> struct _GetFunc<R BSE_CALLING_CONVENTION_CDECL( Args... )> { using type = _FuncClass<R, Args...>; };
  template <typename R, typename Arg, typename... Args> struct _GetFunc<R( BSE_CALLING_CONVENTION_THISCALL Arg::* )(Args...)> { using type = _MemberFuncClass<R, Arg, Args...>; };
  template <typename R, typename Arg, typename... Args> struct _GetFunc<R( BSE_CALLING_CONVENTION_THISCALL Arg::* )(Args...) const> { using type = _MemberFuncClass<R, Arg, Args...>; };

  #if defined(BSE_ARCHITECTURE_X86)
  //I don't know what calling convention x86 wants for members, the thiscall and fastcall apparently aren't it. Whatever.
  template <typename R, typename Arg, typename... Args> struct _GetFunc<R( Arg::* )(Args...)> { using type = _MemberFuncClass<R, Arg, Args...>; };
  template <typename R, typename... Args> struct _GetFunc<R BSE_CALLING_CONVENTION_STDCALL( Args... )> { using type = _FuncClass<R, Args...>; };
  #endif

  #if defined(BSE_ARCHITECTURE_X64)
  template <typename R, typename... Args> struct _GetFunc<R __vectorcall(Args...)> { using type = _FuncClass<R, Args...>; };
  #endif

  template<typename F> struct Function : public _GetFunc<F>::type
  {
    using Super = typename _GetFunc<F>::type;

    Function() { this->ptr = nullptr; }
    Function( Function<F> const& other ) { this->ptr = other.ptr; }
    Function( Function<F>&& other ) { this->ptr = other.ptr; other.ptr = nullptr; }

    template<typename T, class = typename Super::template _EnableIfCallableType<T&, Function>>
    Function( T func )
    {
      this->ptr = (typename Super::Signature) func;
      #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
      this->hotReloadIndex = Super::get_type_register().get_index_for_ptr( this->ptr );
      //moduleId = TODO
      #endif
    }

    template<typename T, class = typename Super::template _EnableIfCallableType<T&, Function>>
    Function<F>& operator=( T&& func )
    {
      this->ptr = (typename Super::Signature) func;
      #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
      this->hotReloadIndex = Super::get_type_register().get_index_for_ptr( this->ptr );
      //moduleId = TODO
      #endif
      return *this;
    }

    #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
    template<typename T, class = typename Super::template _EnableIfCallableType<T&, Function>>
    static void register_function( T fn )
    {
      Super::get_type_register().add_ptr( fn );
      BREAK;
    }
    #endif
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Make Function Pointers ////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template <bool Enabled, typename F> struct _FunctionTypeReduction {};
  template<typename F> struct _FunctionTypeReduction<true, F> { constexpr static bool isLambda = false; using type = Function<typename std::remove_pointer<F>::type>; };
  template<typename F> struct _FunctionTypeReduction<false, F> { constexpr static bool isLambda = true; using type = Function<typename _GetFunc<decltype(&std::remove_reference<F>::type::operator())>::type::_SignatureWithoutClass>; };
  template<typename F> using _ReducedFunction = _FunctionTypeReduction<std::disjunction_v<std::is_member_function_pointer<F>, std::is_function<typename std::remove_pointer<F>::type>>, F >;

  template<typename F> typename _ReducedFunction<F>::type make_function( F&& functor )
  {
    #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
    //I guess in-time register here, what can you do. there's no way to have in-place lambda's ready across hot reloads
    if ( _ReducedFunction<F>::isLambda ) _ReducedFunction<F>::type::register_function( functor );
    #endif
    return { std::forward<F>( functor ) };
  }

  template<typename F> void register_function( F&& functor )
  {
    #if defined(BSE_BUILD_DEBUG_DEVELOPMENT)
    _ReducedFunction<F>::type::register_function( functor );
    #endif
  }
};