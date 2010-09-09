#ifndef CROSS_SC_BASICRS232PORT_H_INCLUDED
#define CROSS_SC_BASICRS232PORT_H_INCLUDED 1
//////////////////////////////////////////////////////////////////////
//
// basic.rs232port.h: bits of the implementation of the rs232port class.
// (c) Aug 31, 2010 Oleg N. Peregudov
//
//////////////////////////////////////////////////////////////////////

#include <crs/security.h>
#include <string>
#include <cstring>

namespace sc {

#if !defined( ASYNCDATACALLBACKFUNCTION )
typedef void ( * asyncDataCallBackFunction ) ( void * pData );
#define ASYNCDATACALLBACKFUNCTION 1
#endif

//
// class basicRS232port (generic interface class)
//
class CROSS_EXPORT basicRS232port
{
public:
	struct errOpen : std::runtime_error {
		errOpen ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
	struct errStatus : std::runtime_error {
		errStatus ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
	struct errClose : std::runtime_error {
		errClose ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
	struct errWrite : std::runtime_error {
		errWrite ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
	struct errRead : std::runtime_error {
		errRead ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
	struct errTimeout : std::runtime_error {
		errTimeout ( const std::string & wh ) : std::runtime_error( wh ) { }
	};
	
protected:
	std::string		m_cComPortName;
	size_t		m_Baud;
	unsigned char	m_DataBits,
				m_StopBits,
				m_Parity;
	bool			m_bConnected;
	size_t		m_dwTimeOut;	// milliseconds
	
	//
	// incoming buffer
	//
	CrossClass::LockType	_inBufLock;
	const size_t		_inBufSize;
	char				*_inBuf,
					*_inBufPtr;
	std::string			_strInBuf;
	asyncDataCallBackFunction _callBackFunc;
	void				*_callBackData;
	CrossClass::LockType	_callBackLock;
	
	virtual void UpdateConnection () = 0;
	virtual size_t compileResponse ( const char * lpBuf, const size_t dwAvailable );
	
	void	incomingData ();
	void	processIncomingBytes ( const size_t dwRead );
	void	inBufAppend ( const char * pData, const size_t nData );
	
public:
	virtual void close () = 0;
	virtual bool synchronize ( const unsigned long dwTimeOut = 0 );
	
	void swrite ( const std::string & str );
	virtual void write ( const char * lpBuf, const size_t dwToWrite ) = 0;
	virtual bool receive ();
	//
	// return false when termination is requested
	// can throw errRead
	//
	virtual void postTerminate () = 0;
	
	void setAsyncDataCallback ( asyncDataCallBackFunction func, void * pData );
	std::string getString ();
	
	basicRS232port ( const size_t inBufSize = 2048 );
	virtual ~basicRS232port ();
};

} // namespace sc
#endif // CROSS_SC_BASICRS232PORT_H_INCLUDED

