//
// pools.cpp: general mass spectrum representation data base - pool classes
// (c) Jan 8, 2008 Oleg N. Peregudov
//	09/07/2010	conforming libcrs v1.0.x
//	09/28/2010	non standard temporary file names in Win32
//	12/07/2010	included into libcrs
//
#if defined( _MSC_VER )
#	pragma warning( disable : 4251 )
#	pragma warning( disable : 4351 )
#endif
#include <crs/msdc/pools.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace msdc {

//
// members of class basic_pool
//

void basic_pool::clear ()
{
	nCount = 0;
}

//
// members of class mem_pool
//

mem_pool::mem_pool ( const size_t nData, const size_t nPoolSize )
	: basic_pool( nData, nPoolSize )
	, pBigStor( new char [ nPoolSize * nByteRecordSize ] )
	, vContainer( new cIntensityPoint [ nPoolSize ] )
{
	memset( pBigStor, 0, nPoolSize * nByteRecordSize );
	for( size_t nRec = 0; nRec < nPoolSize; ++nRec )
		vContainer[ nRec ].allocate( nRecordSize, ( pBigStor + nRec * nByteRecordSize ) );
}

mem_pool::~mem_pool ()
{
	delete [] vContainer;
	delete [] pBigStor;
}

bool	mem_pool::push ( const record & r )
{
	vContainer[ nCount ] = r;
	return ( ++nCount == nFullSize );
}

void	mem_pool::get ( const size_t i, record & r ) const
{
	r = vContainer[ i ];
}

//
// members of class file_pool
//

void	file_pool::_create_file ()
{
	char tempFileName [ L_tmpnam ];
	for( size_t i = 0; i < 5; ++i )
	{
#if defined( USE_WIN32_API )
		sprintf( tempFileName, "~page%04X.tmp", rand() );
#else
		tmpnam( tempFileName );
#endif
		_stream.open( tempFileName, std::ios_base::in|std::ios_base::out|std::ios_base::trunc|std::ios_base::binary );
		if( _stream.good() )
		{
			_path = tempFileName;
			_stream.exceptions( ALL_IOS_EXCEPTIONS );
			return;
		}
		else
			_stream.clear();
	}
	throw std::runtime_error( "can't create file_pool" );
}

file_pool::file_pool ( const size_t nData, const size_t nPoolSize, const std::string & path )
	: basic_pool( nData, nPoolSize )
	, _mutex( )
	, _stream( )
	, _path( path )
{
	_create_file();
}

file_pool::file_pool ( const mem_pool & mp, const std::string & path )
	: basic_pool( mp )
	, _mutex( )
	, _stream( )
	, _path( path )
{
	_create_file();
	assign( mp );
}

file_pool::~file_pool ()
{
	_stream.close();
	remove( _path.c_str() );
}

void	file_pool::assign ( const mem_pool & mp )
{
	CrossClass::_LockIt lockFileMutex ( _mutex );
	_stream.seekp( 0 );
	_stream.write( mp.pBigStor, nFullSize * nByteRecordSize );
	nCount = mp.nCount;
}

bool	file_pool::push ( const record & r )
{
	CrossClass::_LockIt lockFileMutex ( _mutex );
	_stream.seekp( nCount * nByteRecordSize );
	_stream.write( reinterpret_cast<const char *>( r.operator->() ), nByteRecordSize );
	return ( ++nCount == nFullSize );
}

void	file_pool::get ( const size_t i, record & r ) const
{
	CrossClass::_LockIt lockFileMutex ( _mutex );
	_stream.seekg( i * nByteRecordSize );
	_stream.read( reinterpret_cast<char *>( r.operator->() ), nByteRecordSize );
}

} // namespace msdc
