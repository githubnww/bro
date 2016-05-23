// See the file "COPYING" in the main distribution directory for copyright.

#ifndef ANALYZER_PROTOCOL_GSSAPI_GSSAPI_H
#define ANALYZER_PROTOCOL_GSSAPI_GSSAPI_H

#include "events.bif.h"
#include "analyzer/protocol/tcp/TCP.h"

#include "gssapi_pac.h"

namespace analyzer { namespace gssapi {

class GSSAPI_Analyzer

: public tcp::TCP_ApplicationAnalyzer {

public:
	GSSAPI_Analyzer(Connection* conn);
	virtual ~GSSAPI_Analyzer();

	// Overriden from Analyzer.
	virtual void Done();

	virtual void DeliverStream(int len, const u_char* data, bool orig);
	virtual void Undelivered(uint64 seq, int len, bool orig);

	// Overriden from tcp::TCP_ApplicationAnalyzer.
	virtual void EndpointEOF(bool is_orig);

	static analyzer::Analyzer* Instantiate(Connection* conn)
		{ return new GSSAPI_Analyzer(conn); }

protected:
	binpac::GSSAPI::GSSAPI_Conn* interp;
};

} } // namespace analyzer::*

#endif
