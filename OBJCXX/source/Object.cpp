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

#include <OBJCXX/Object.hpp>
#include <OBJCXX/Foundation/Classes/NSString.hpp>
#include <iostream>

namespace OBJCXX
{
    class Object::IMPL
    {
        public:
            
            IMPL();
            IMPL( id o );
            IMPL( const IMPL & o );
            ~IMPL();
            
            std::string _className;
            Class       _class;
            id          _object;
    };
    
    Object::Object( const std::string & className ):
        Object
        (
            className,
            [ = ]
            {
                return this->message< id >( "init" ).send();
            }
        )
    {}
    
    Object::Object( const std::string & className, std::function< id() > init ):
        NS::Protocols::Object(),
        impl( std::make_unique< IMPL >() )
    {
        this->impl->_className = className;
        this->impl->_class     = OBJCXX::RT::GetClass( this->impl->_className );
        this->impl->_object    = OBJCXX::RT::Message< id >( this->impl->_class, "alloc" ).send();
        this->impl->_object    = init();
    }
    
    Object::Object( id object ):
        NS::Protocols::Object(),
        impl( std::make_unique< IMPL >( object ) )
    {}
    
    Object::Object( const Object & o ):
        NS::Protocols::Object(),
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    Object::Object( Object && o ):
        NS::Protocols::Object(),
        impl( std::move( o.impl ) )
    {}
    
    Object::~Object()
    {}
    
    Object & Object::operator =( Object o )
    {
        using std::swap;
        
        if( o != nullptr && this->impl->_class != nullptr && o.message< bool >( "isKindOfClass:" ).send< Class >( this->impl->_class ) == false )
        {
            std::string msg;
            
            msg = "Cannot assign object of incompatible class '"
                + o.impl->_className
                + "' to object of class '"
                + this->impl->_className
                + "'";
            
            throw std::runtime_error( msg );
        }
        
        swap( this->impl->_object, o.impl->_object );
        
        return *( this );
    }
    
    bool Object::operator ==( const Object & o ) const
    {
        return this->message< bool >( "isEqual:" ).send< id >( o );
    }
    
    bool Object::operator !=( const Object & o ) const
    {
        return !operator ==( o );
    }
    
    bool Object::operator ==( id object ) const
    {
        return this->message< bool >( "isEqual:" ).send< id >( object );
    }
    
    bool Object::operator !=( id object ) const
    {
        return !operator ==( object );
    }
    
    bool Object::operator ==( std::nullptr_t n ) const
    {
        ( void )n;
        
        return static_cast< id >( *( this ) ) == nullptr;
    }
    
    bool Object::operator !=( std::nullptr_t n ) const
    {
        return !operator ==( n );
    }
    
    Object::operator id() const
    {
        return this->impl->_object;
    }
         
    Class Object::getClass() const
    {
        return this->message< Class >( "class" ).send();
    }
    
    Class Object::superclass() const
    {
        return this->message< Class >( "superclass" ).send();
    }
    
    bool Object::isEqual( const Object & o ) const
    {
        return this->message< bool >( "isEqual:" ).send< id >( o );
    }
    
    NS::UInteger Object::hash() const
    {
        return this->message< NS::UInteger >( "hash" ).send();
    }
    
    id Object::self() const
    {
        return this->message< id >( "self" ).send();
    }
    
    bool Object::isKindOfClass( Class cls ) const
    {
        return this->message< bool >( "isKindOfClass:" ).send< Class >( cls );
    }
    
    bool Object::isMemberOfClass( Class cls ) const
    {
        return this->message< bool >( "isMemberOfClass:" ).send< Class >( cls );
    }
    
    bool Object::respondsToSelector( SEL sel ) const
    {
        return this->message< bool >( "respondsToSelector:" ).send< SEL >( sel );
    }
    
    bool Object::conformsToProtocol( void * protocol ) const
    {
        return this->message< bool >( "conformsToProtocol:" ).send< void * >( protocol );
    }
    
    std::string Object::description() const
    {
        const char * cp;
        NS::String   s( this->message< id >( "description" ).send() );
        
        cp = s.UTF8String();
        
        return ( cp ) ? cp : "";
    }
    
    std::string Object::debugDescription() const
    {
        const char * cp;
        NS::String   s( this->message< id >( "debugDescription" ).send() );
        
        cp = s.UTF8String();
        
        return ( cp ) ? cp : "";
    }
    
    id Object::performSelector( SEL sel )
    {
        return this->message< id >( "performSelector:" ).send< SEL >( sel );
    }
    
    id Object::performSelector( SEL sel, id o1 )
    {
        return this->message< id >( "performSelector:withObject:" ).send< SEL, id >( sel, o1 );
    }
    
    id Object::performSelector( SEL sel, id o1, id o2 )
    {
        return this->message< id >( "performSelector:withObject:" ).send< SEL, id, id >( sel, o1, o2 );
    }
    
    bool Object::isProxy() const
    {
        return this->message< bool >( "isProxy" ).send();
    }
    
    id Object::retain()
    {
        return this->message< id >( "retain" ).send();
    }
    
    void Object::release()
    {
        this->message< void >( "release" ).send();
    }
    
    id Object::autorelease() const
    {
        return this->message< id >( "autorelease" ).send();
    }
    
    NS::UInteger Object::retainCount() const
    {
        return this->message< NS::UInteger >( "retainCount" ).send();
    }
    
    void * Object::zone() const
    {
        return this->message< void * >( "zone" ).send();
    }
    
    std::ostream & operator << ( std::ostream & os, const Object & o )
    {
        os << o.description();
        
        return os;
    }

    Object::IMPL::IMPL():
        _className( "" ),
        _class( nullptr ),
        _object( nullptr )
    {}
        
    Object::IMPL::IMPL( id o ): IMPL()
    {
        if( o )
        {
            this->_className = ( o ) ? OBJCXX::RT::GetClassName( OBJCXX::RT::GetClass( o ) ) : "";
            this->_class     = ( o ) ? OBJCXX::RT::GetClass( this->_className ): nullptr;
            this->_object    = ( o ) ? o : nullptr;
            this->_object    = ( o ) ? OBJCXX::RT::Message< id >( this->_object, "retain" ).send() : nullptr;
        }
    }

    Object::IMPL::IMPL( const IMPL & o ):
        _className( o._className ),
        _class( o._class ),
        _object( o._object )
    {
        this->_object = OBJCXX::RT::Message< id >( this->_object, "retain" ).send();
    }

    Object::IMPL::~IMPL()
    {
        OBJCXX::RT::Message< void >( this->_object, "release" ).send();
    }
}
