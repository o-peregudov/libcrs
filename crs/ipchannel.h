#ifndef CROSS_IPCHANNEL_H_INCLUDED
#define CROSS_IPCHANNEL_H_INCLUDED 1
/*
 *  crs/ipchannel.h - event loop based inter process communication channel (pipe)
 *  Copyright (c) 2012 Oleg N. Peregudov <o.peregudov@gmail.com>
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

/*
 *	2012/09/21	first version
 */

#include <crs/libexport.h>
#if USE_WIN32_API
#	include <crs/bits/win32.ipchannel.h>
namespace CrossClass
{
	typedef win32_ip_channel	ip_channel;
}
#elif USE_POSIX_API
#	include <crs/bits/posix.ipchannel.h>
namespace CrossClass
{
	typedef posix_ip_channel	ip_channel;
}
#endif
#endif/* CROSS_IPCHANNEL_H_INCLUDED	*/
