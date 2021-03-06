#ifndef CROSS_SIMPLENETPOINT_H_INCLUDED
#define CROSS_SIMPLENETPOINT_H_INCLUDED 1
// (c) Aug 29, 2010 Oleg N. Peregudov
//	09/19/2010	default callback function
//	11/30/2010	updated netPoint interface
//	12/09/2010	observer for transmission flag

#include <crs/netpoint.h>
#include <crs/callback.h>
#include <crs/security.h>
#include <cstring>
#include <deque>

namespace SimpleNetPoint {

struct CROSS_EXPORT message
{
	unsigned long	size;
	char *		contents;
	
	message ( const unsigned long );
	message ( const message & );
	message ( const std::string & );
	
	~message ();
	
	message & operator = ( const message & );
};

typedef callBackFunction asyncDataCallBackFunction;

class CROSS_EXPORT server : public CrossClass::netPoint
{
protected:
	CrossClass::LockType	_inboxMutex;
	std::deque<message>	_inbox;
	message			_inMessage;
	bool				_inContents;
	char *			_inNextByte;
	unsigned long		_inBytesRest;
	
	asyncDataCallBackFunction	_newMessageCallBack;
	void *			_callBackData;
	CrossClass::LockType	_callBackLock;
	
	CrossClass::LockType	_outboxMutex;
	std::deque<message>	_outbox;
	bool				_outContents,
					_outNeeded;
	char *			_outNextByte;
	unsigned long		_outBytesRest;
	
	virtual void transmit ();
	virtual void receive ();
	
	virtual CrossClass::basicNetPoint * handleNewConnection ( CrossClass::cSocket &, const CrossClass::cSockAddr & );
	server ( CrossClass::cSocket & );
	
public:
	server ();
	virtual ~server ();
	
	virtual bool want2transmit ();
	
	void	sendMessage ( const message & );
	bool	recvMessage ( message & );
	
	void	setAsyncDataCallback ( asyncDataCallBackFunction func, void * pData );
};

} // namespace SimpleNetPoint
#endif // CROSS_SIMPLENETPOINT_H_INCLUDED
