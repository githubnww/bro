// See the file "COPYING" in the main distribution directory for copyright.

#include "bro-config.h"

#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

#include "DCE_RPC.h"

using namespace analyzer::dce_rpc;


DCE_RPC_Analyzer::DCE_RPC_Analyzer(Connection *conn)
: tcp::TCP_ApplicationAnalyzer("DCE_RPC", conn)
	{
	interp = new binpac::DCE_RPC::DCE_RPC_Conn(this);
	}

DCE_RPC_Analyzer::~DCE_RPC_Analyzer()
	{
	delete interp;
	}

void DCE_RPC_Analyzer::Done()
	{
	TCP_ApplicationAnalyzer::Done();

	interp->FlowEOF(true);
	interp->FlowEOF(false);
	}

void DCE_RPC_Analyzer::EndpointEOF(bool is_orig)
	{
	TCP_ApplicationAnalyzer::EndpointEOF(is_orig);
	interp->FlowEOF(is_orig);
	}
	
void DCE_RPC_Analyzer::Undelivered(uint64 seq, int len, bool orig)
	{
	TCP_ApplicationAnalyzer::Undelivered(seq, len, orig);
	interp->NewGap(orig, len);
	}

void DCE_RPC_Analyzer::DeliverStream(int len, const u_char* data, bool orig)
	{
	TCP_ApplicationAnalyzer::DeliverStream(len, data, orig);

	assert(TCP());
	try 
		{
		interp->NewData(orig, data, data + len);
		}
	catch ( const binpac::Exception& e )
		{
		ProtocolViolation(fmt("Binpac exception: %s", e.c_msg()));
		}
	}
