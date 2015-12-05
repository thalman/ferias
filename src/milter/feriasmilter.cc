#include "feriasmilter.h"
#include <string.h>

std::string FeriasMilter::from () const {
    return _from;
}

void FeriasMilter::from (const char *from) {
    _from = from;
}

void FeriasMilter::rcpt (const char *rcpt) {
    _to.push_back( rcpt );
}

void FeriasMilter::header (const char *header) {
    if (! header) return;
    if (strcasecmp (header, "List-Id") == 0) {
        _mailinglist = true;
    }
    if (strcasecmp (header, "X-Autoreply") == 0) {
        _xautoreply = true;
    }
}

void FeriasMilter::sendAutoreply() {
    if (_xautoreply || _mailinglist || _from.empty() || _to.empty()) return;
    printf ("from: %s\n", _from.c_str ());
}
