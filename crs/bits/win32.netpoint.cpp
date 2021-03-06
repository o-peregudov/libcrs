/*
 *  crs/bits/win32.netpoint.cpp
 *  Copyright (c) 2009-2012 Oleg N. Peregudov <o.peregudov@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if defined (HAVE_CONFIG_H)
#	include "config.h"
#endif
#include <crs/bits/win32.netpoint.h>
#include <cstdio>

namespace CrossClass {

//
// members of class win32NetPoint
//
win32NetPoint::win32NetPoint ()
	: basicNetPoint( )
	, readset( )
	, writeset( )
	, exceptset( )
	, highsock( )
	, evntTerminate( CreateEvent( NULL, FALSE, FALSE, NULL ) )
	, selectTimeOut( )
{
}

win32NetPoint::win32NetPoint ( cSocket & clientSocket )
	: basicNetPoint( clientSocket )
	, readset( )
	, writeset( )
	, exceptset( )
	, highsock( )
	, evntTerminate( CreateEvent( NULL, FALSE, FALSE, NULL ) )
	, selectTimeOut( )
{
	setNonBlock();
}

win32NetPoint::~win32NetPoint ()
{
	//
	// disconnect all clients
	//
	disconnectAll();
	
	//
	// shutdown socket
	//
	shutdown( _socket, SD_BOTH );
	
	//
	// release system handle
	//
	CloseHandle( evntTerminate );
}

basicNetPoint * win32NetPoint::handleNewConnection ( cSocket & s, const cSockAddr & sa )
{
	return new win32NetPoint( s );
}

void win32NetPoint::buildSelectList ()
{
	FD_ZERO( &readset );
	FD_ZERO( &writeset );
	FD_ZERO( &exceptset );
	
	highsock = _socket;
	FD_SET( _socket, &readset );
	FD_SET( _socket, &exceptset );
	
	// fill in clients
	for( size_t i = 0; i < _clientList.size(); ++i )
	{
		while( ( i < _clientList.size() ) && ( _clientList[ i ] == 0 ) )
		{
			_clientList[ i ] = _clientList.back();
			_clientList.pop_back();
		}
		if( i < _clientList.size() )
		{
			FD_SET( _clientList[ i ]->getSocket(), &readset );
			if( _clientList[ i ]->want2transmit() )
				FD_SET( _clientList[ i ]->getSocket(), &writeset );
			FD_SET( _clientList[ i ]->getSocket(), &exceptset );
			if( highsock < _clientList[ i ]->getSocket() )
				highsock = _clientList[ i ]->getSocket();
		}
	}
}

bool win32NetPoint::checkTerminate ()
{
	return ( WaitForSingleObject( evntTerminate, 0 ) == WAIT_OBJECT_0 );
}

void win32NetPoint::postTerminate ()
{
	SetEvent( evntTerminate );
}

bool win32NetPoint::clientSendRecv ()
{
	if( checkTerminate() )
		return false;
	else
	{
		FD_ZERO( &readset );
		FD_ZERO( &writeset );
		FD_ZERO( &exceptset );
		
		highsock = _socket;
		FD_SET( _socket, &readset );
		if( want2transmit() )
			FD_SET( _socket, &writeset );
		FD_SET( _socket, &exceptset );
		
		selectTimeOut.microseconds( 100 );
		switch( select( highsock+1, &readset, &writeset, &exceptset, &selectTimeOut ) )
		{
		case	SOCKET_ERROR:
			{
				char msgText [ 64 ];
				sprintf( msgText, "code: %d", WSAGetLastError() );
				throw socket_select_error( msgText );
			}
		
		case	0:	// timeout
			break;
		
		default:
			if( FD_ISSET( _socket, &readset ) )
				receive();
			
			if( FD_ISSET( _socket, &writeset ) )
				transmit();
		}
		
		return true;
	}
}

bool win32NetPoint::serverSendRecv ()
{
	if( checkTerminate() )
		return false;
	else
	{
		buildSelectList();
		selectTimeOut.microseconds( 100 );
		switch( select( highsock+1, &readset, &writeset, &exceptset, &selectTimeOut ) )
		{
		case	SOCKET_ERROR:
			{
				char msgText [ 64 ];
				sprintf( msgText, "code: %d", WSAGetLastError() );
				throw socket_select_error( msgText );
			}
		
		case	0:	// timeout
			break;
		
		default:	// something was selected
			if( FD_ISSET( _socket, &readset ) )			// do we have a new connection ?
			{
				cSocket newPeer;
				cSockAddr newPeerAddr;
				if( _socket.accept( newPeer, newPeerAddr ) )
					addClient( handleNewConnection( newPeer, newPeerAddr ) );
			}
			
			#pragma omp parallel for if ( _clientList.size() > 100 )
			for( int i = 0; i < _clientList.size(); ++i )
			{
				try
				{
					if( FD_ISSET( _clientList[ i ]->getSocket(), &writeset ) )
						clientTransmit( i );
					
					if( FD_ISSET( _clientList[ i ]->getSocket(), &readset ) )
						clientReceive( i );
					
					if( FD_ISSET( _clientList[ i ]->getSocket(), &exceptset ) )
						throw basicNetPoint::end_of_file( "exceptset" );
				}
				catch( ... )
				{
					removeClient( i );
				}
			}
		}
		
		return true;
	}
}

} /* namespace CrossClass	*/
