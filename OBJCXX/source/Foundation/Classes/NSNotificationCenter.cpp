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

#include <OBJCXX/Foundation/Classes/NSNotificationCenter.hpp>

namespace NS
{
    NotificationCenter NotificationCenter::defaultCenter()
    {
        id o;
        
        o = OBJCXX::RT::Message< id >( "NSNotificationCenter", "defaultCenter" ).send();
        
        return NotificationCenter( o );
    }
    
    NotificationCenter::NotificationCenter(): Object( "NSNotificationCenter" )
    {}
    
    void NotificationCenter::addObserver( id notificationObserver, SEL notificationSelector, const String & notificationName, id notificationSender )
    {
        this->message< void >( "addObserver:selector:name:object:" ).send< id, SEL, id, id >( notificationObserver, notificationSelector, notificationName, notificationSender );
    }
    
    void NotificationCenter::removeObserver( id notificationObserver )
    {
        this->message< void >( "removeObserver:" ).send< id >( notificationObserver );
    }
    
    void NotificationCenter::removeObserver( id notificationObserver, const String & notificationName, id notificationSender )
    {
        this->message< void >( "removeObserver:name:object:" ).send< id, id, id >( notificationObserver, notificationName, notificationSender );
    }
    
    void NotificationCenter::postNotification( const Notification & notification )
    {
        this->message< void >( "postNotification:" ).send< id >( notification );
    }
    
    void NotificationCenter::postNotificationName( const String & notificationName, id notificationSender )
    {
        this->message< void >( "postNotificationName:object:" ).send< id, id >( notificationName, notificationSender );
    }
    
    void NotificationCenter::postNotificationName( const String & notificationName, id notificationSender, const Dictionary & userInfo )
    {
        this->message< void >( "postNotificationName:object:userInfo:" ).send< id, id, id >( notificationName, notificationSender, userInfo );
    }
}
