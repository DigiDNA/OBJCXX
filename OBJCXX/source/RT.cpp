/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @copyright   (c) 2015 - Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * @brief       ...
 */

#include <OBJCXX/RT.hpp>
#include <mutex>

#ifdef _WIN32

#include <Windows.h>
#include <codecvt>

#ifdef GetClassName
#undef GetClassName
#endif

#else

extern "C"
{
    extern Class        objc_getClass            ( const char * );
    extern Class        objc_getMetaClass        ( const char * );
    extern Protocol *   objc_getProtocol         ( const char * );
    extern void         objc_msgSend             ( void );
    #ifndef __arm64__
    extern void         objc_msgSend_fpret       ( void );
    #endif
    extern void         objc_msgSendSuper        ( void );
    extern Class        objc_allocateClassPair   ( Class, const char *, size_t );
    extern void         objc_registerClassPair   ( Class );
    extern void         objc_setAssociatedObject ( id, const void *, id, OBJCXX::RT::Internal::AssociationPolicy );
    extern id           objc_getAssociatedObject ( id, const void * );
    extern SEL          sel_registerName         ( const char * );
    extern const char * sel_getName              ( SEL );
    extern Class        object_getClass          ( id );
    extern IMP          method_getImplementation ( Method );
    extern SEL          method_getName           ( Method );
    extern const char * method_getTypeEncoding   ( Method );
    extern Class        class_getSuperclass      ( Class );
    extern const char * class_getName            ( Class );
    extern Method     * class_copyMethodList     ( Class, unsigned int * );
    extern bool         class_addIvar            ( Class, const char *, size_t, uint8_t, const char * );
    extern bool         class_addMethod          ( Class, SEL, IMP, const char * );
    extern bool         class_addProtocol        ( Class, Protocol * );
    extern Ivar         class_getInstanceVariable( Class, const char * );
    extern ptrdiff_t    ivar_getOffset           ( Ivar );
    extern void         NSLogv                   ( id, va_list );
}

static void init() __attribute__( ( constructor ) );
static void init()
{
    OBJCXX::RT::Init();
}

#endif

namespace OBJCXX
{
    namespace RT
    {
        class MessageBase::IMPL
        {
            public:
                
                IMPL();
                IMPL( id object, const std::string & selector );
                IMPL( const IMPL & o );
                ~IMPL();
                
                id  _object;
                SEL _selector;
        };
        
        Class GetClass( const std::string & name )
        {
            Class ret;
            
            if( name.length() == 0 )
            {
                throw std::runtime_error( "Empty class name" );
            }
            
            ret = Internal::objc_getClass( name.c_str() );
            
            if( ret == nullptr )
            {
                {
                    std::string msg;
                    
                    msg = "No class named '" + name + "'";
                    
                    throw std::runtime_error( msg );
                }
            }
            
            return ret;
        }
        
        Class GetClass( id object )
        {
            return Internal::object_getClass( object );
        }
        
        std::string GetClassName( Class cls )
        {
            const char * s;
            
            if( cls == nullptr )
            {
                return "";
            }
            
            s = Internal::class_getName( cls );
            
            if( s == nullptr )
            {
                return "";
            }
            
            return s;
        }
        
        SEL GetSelector( const std::string & name )
        {
            SEL ret;
            
            if( name.length() == 0 )
            {
                throw std::runtime_error( "Empty selector name" );
            }
            
            ret = Internal::sel_registerName( name.c_str() );
            
            if( ret == nullptr )
            {
                {
                    std::string msg;
                    
                    msg = "No selector named '" + name + "'";
                    
                    throw std::runtime_error( msg );
                }
            }
            
            return ret;
        }
            
        MessageBase::MessageBase( id object, const std::string & selector ):
            impl( std::make_unique< IMPL >( object, selector ) )
        {}
        
        MessageBase::MessageBase( Class cls, const std::string & selector ):
            impl( std::make_unique< IMPL >( reinterpret_cast< id >( cls ), selector ) )
        {}
        
        MessageBase::MessageBase( const std::string & cls, const std::string & selector ):
            MessageBase( GetClass( cls ), selector )
        {}
        
        MessageBase::MessageBase( const MessageBase & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        MessageBase::MessageBase( MessageBase && o ) noexcept:
            impl( std::move( o.impl ) )
        {}
        
        MessageBase::~MessageBase()
        {}
        
        MessageBase & MessageBase::operator =( MessageBase o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        void swap( MessageBase & o1, MessageBase & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        id MessageBase::object()
        {
            return this->impl->_object;
        }
        
        SEL MessageBase::selector()
        {
            return this->impl->_selector;
        }
        
        Class        ( * Internal::objc_getClass             )( const char * )                                        = nullptr;
        Class        ( * Internal::objc_getMetaClass         )( const char * )                                        = nullptr;
        Protocol *   ( * Internal::objc_getProtocol          )( const char * )                                        = nullptr;
        void         ( * Internal::objc_msgSend              )()                                                      = nullptr;
        #ifndef __arm64__
        void         ( * Internal::objc_msgSend_fpret        )()                                                      = nullptr;
        #endif
        void         ( * Internal::objc_msgSendSuper         )()                                                      = nullptr;
        Class        ( * Internal::objc_allocateClassPair    )( Class, const char *, size_t )                         = nullptr;
        void         ( * Internal::objc_registerClassPair    )( Class )                                               = nullptr;
        void         ( * Internal::objc_setAssociatedObject  )( id, const void *, id, AssociationPolicy )             = nullptr;
        id           ( * Internal::objc_getAssociatedObject  )( id, const void * )                                    = nullptr;
        SEL          ( * Internal::sel_registerName          )( const char * )                                        = nullptr;
        const char * ( * Internal::sel_getName               )( SEL )                                                 = nullptr;
        Class        ( * Internal::object_getClass           )( id )                                                  = nullptr;
        IMP          ( * Internal::method_getImplementation  )( Method )                                              = nullptr;
        SEL          ( * Internal::method_getName            )( Method )                                              = nullptr;
        const char * ( * Internal::method_getTypeEncoding    )( Method )                                              = nullptr;
        Class        ( * Internal::class_getSuperclass       )( Class )                                               = nullptr;
        const char * ( * Internal::class_getName             )( Class )                                               = nullptr;
        Method     * ( * Internal::class_copyMethodList      )( Class, unsigned int * )                               = nullptr;
        bool         ( * Internal::class_addIvar             )( Class, const char *, size_t, uint8_t, const char * )  = nullptr;
        bool         ( * Internal::class_addMethod           )( Class, SEL, IMP, const char * )                       = nullptr;
        bool         ( * Internal::class_addProtocol         )( Class, Protocol * )                                   = nullptr;
        Ivar         ( * Internal::class_getInstanceVariable )( Class, const char * )                                 = nullptr;
        ptrdiff_t    ( * Internal::ivar_getOffset            )( Ivar )                                                = nullptr;
        void         ( * Internal::NSLogv                    )( id, va_list )                                         = nullptr;
        
        void Init()
        {
            static std::once_flag once;
            
            std::call_once
            (
                once,
                []
                {
                    #ifdef _WIN32
                    
                    char       * common;
                    std::string  apple;
                    HMODULE      objc;
                    HMODULE      foundation;
                    
                    objc       = LoadLibraryA( "objc.dll" );
                    foundation = LoadLibraryA( "Foundation.dll" );
                    
                    if( objc == nullptr || foundation == nullptr )
                    {
                        #ifdef _WIN64
                        common = nullptr;
                        #else
                        common = getenv( "COMMONPROGRAMFILES(x86)" );
                        #endif

                        if( common == nullptr )
                        {
                            common = getenv( "COMMONPROGRAMFILES" );
                        }
                        
                        apple = std::string( common ) + "\\Apple\\Apple Application Support";

                        SetDllDirectoryA( apple.c_str() );

                        objc       = LoadLibraryA( ( apple + "\\objc.dll" ).c_str() );
                        foundation = LoadLibraryA( ( apple + "\\Foundation.dll" ).c_str() );
                    }
                    
                    if( objc == nullptr || foundation == nullptr )
                    {
                        return;
                    }
                    
                    Internal::objc_getClass             = ( Class        ( * )( const char * )                                       )GetProcAddress( objc, "objc_getClass" );
                    Internal::objc_getMetaClass         = ( Class        ( * )( const char * )                                       )GetProcAddress( objc, "objc_getMetaClass" );
                    Internal::objc_getProtocol          = ( Protocol *   ( * )( const char * )                                       )GetProcAddress( objc, "objc_getProtocol" );
                    Internal::objc_msgSend              = ( void         ( * )()                                                     )GetProcAddress( objc, "objc_msgSend" );
                    #ifndef __arm64__
                    Internal::objc_msgSend_fpret        = ( void         ( * )()                                                     )GetProcAddress( objc, "objc_msgSend_fpret" );
                    #endif
                    Internal::objc_msgSendSuper         = ( void         ( * )()                                                     )GetProcAddress( objc, "objc_msgSendSuper" );
                    Internal::objc_allocateClassPair    = ( Class        ( * )( Class, const char *, size_t )                        )GetProcAddress( objc, "objc_allocateClassPair" );
                    Internal::objc_registerClassPair    = ( void         ( * )( Class )                                              )GetProcAddress( objc, "objc_registerClassPair" );
                    Internal::objc_setAssociatedObject  = ( void         ( * )( id, const void *, id, Internal::AssociationPolicy )  )GetProcAddress( objc, "objc_setAssociatedObject" );
                    Internal::objc_getAssociatedObject  = ( id           ( * )( id, const void * )                                   )GetProcAddress( objc, "objc_getAssociatedObject" );
                    Internal::sel_registerName          = ( SEL          ( * )( const char * )                                       )GetProcAddress( objc, "sel_registerName" );
                    Internal::sel_getName               = ( const char * ( * )( SEL )                                                )GetProcAddress( objc, "sel_getName" );
                    Internal::object_getClass           = ( Class        ( * )( id )                                                 )GetProcAddress( objc, "object_getClass" );
                    Internal::method_getImplementation  = ( IMP          ( * )( Method )                                             )GetProcAddress( objc, "method_getImplementation" );
                    Internal::method_getName            = ( SEL          ( * )( Method )                                             )GetProcAddress( objc, "method_getName" );
                    Internal::method_getTypeEncoding    = ( const char * ( * )( Method )                                             )GetProcAddress( objc, "method_getTypeEncoding" );
                    Internal::class_getSuperclass       = ( Class        ( * )( Class )                                              )GetProcAddress( objc, "class_getSuperclass" );
                    Internal::class_getName             = ( const char * ( * )( Class )                                              )GetProcAddress( objc, "class_getName" );
                    Internal::class_copyMethodList      = ( Method     * ( * )( Class, unsigned int * )                              )GetProcAddress( objc, "class_copyMethodList" );
                    Internal::class_addIvar             = ( bool         ( * )( Class, const char *, size_t, uint8_t, const char * ) )GetProcAddress( objc, "class_addIvar" );
                    Internal::class_addMethod           = ( bool         ( * )( Class, SEL, IMP, const char * )                      )GetProcAddress( objc, "class_addMethod" );
                    Internal::class_addProtocol         = ( bool         ( * )( Class, Protocol * )                                  )GetProcAddress( objc, "class_addProtocol" );
                    Internal::class_getInstanceVariable = ( Ivar         ( * )( Class, const char * )                                )GetProcAddress( objc, "class_getInstanceVariable" );
                    Internal::ivar_getOffset            = ( ptrdiff_t    ( * )( Ivar )                                               )GetProcAddress( objc, "ivar_getOffset" );
                    Internal::NSLogv                    = ( void         ( * )( id, va_list )                                        )GetProcAddress( foundation, "NSLogv" );

                    #else
                    
                    Internal::objc_getClass             = ::objc_getClass;
                    Internal::objc_getMetaClass         = ::objc_getMetaClass;
                    Internal::objc_getProtocol          = ::objc_getProtocol;
                    Internal::objc_msgSend              = ::objc_msgSend;
                    #ifndef __arm64__
                    Internal::objc_msgSend_fpret        = ::objc_msgSend_fpret;
                    #endif
                    Internal::objc_msgSendSuper         = ::objc_msgSendSuper;
                    Internal::objc_allocateClassPair    = ::objc_allocateClassPair;
                    Internal::objc_registerClassPair    = ::objc_registerClassPair;
                    Internal::objc_setAssociatedObject  = ::objc_setAssociatedObject;
                    Internal::objc_getAssociatedObject  = ::objc_getAssociatedObject;
                    Internal::sel_registerName          = ::sel_registerName;
                    Internal::sel_getName               = ::sel_getName;
                    Internal::object_getClass           = ::object_getClass;
                    Internal::method_getImplementation  = ::method_getImplementation;
                    Internal::method_getName            = ::method_getName;
                    Internal::method_getTypeEncoding    = ::method_getTypeEncoding;
                    Internal::class_getSuperclass       = ::class_getSuperclass;
                    Internal::class_getName             = ::class_getName;
                    Internal::class_copyMethodList      = ::class_copyMethodList;
                    Internal::class_addIvar             = ::class_addIvar;
                    Internal::class_addMethod           = ::class_addMethod;
                    Internal::class_addProtocol         = ::class_addProtocol;
                    Internal::class_getInstanceVariable = ::class_getInstanceVariable;
                    Internal::ivar_getOffset            = ::ivar_getOffset;
                    Internal::NSLogv                    = ::NSLogv;
                    
                    #endif
                }
            );
        }

        MessageBase::IMPL::IMPL():
            _object( nullptr ),
            _selector( nullptr )
        {}

        MessageBase::IMPL::IMPL( id object, const std::string & selector ):
            _object( object ),
            _selector( OBJCXX::RT::GetSelector( selector ) )
        {}

        MessageBase::IMPL::IMPL( const IMPL & o ):
            _object( o._object ),
            _selector( o._selector )
        {}

        MessageBase::IMPL::~IMPL()
        {}
    }
}
