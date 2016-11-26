#include "feriasmilter.h"
#include "httpclient.h"
#include "vsjson.h"
#include "mailer.h"
#include "configuration.h"

#include <string.h>

#define streq(X,Y) (strcmp(X,Y) == 0)

FeriasMilter::FeriasMilter() : _base_url(cfg_base_url ()) { };

std::string FeriasMilter::from () const {
    return _from;
}

void FeriasMilter::from (const char *from) {
    _from = from;
}

void FeriasMilter::rcpt (const char *rcpt) {
    _to.push_back( rcpt );
}

void FeriasMilter::header (const char *header, const char *value) {
    if (! header) return;
    if (strcasecmp (header, "List-Id") == 0) {
        _mailinglist = true;
    }
    if (
        (strcasecmp (header, "X-Autoreply") == 0) ||
        (strcasecmp (header, "X-Autorespond") == 0)
    ) {
        _xautoreply = true;
    }
    if (
        (strcasecmp (header, "Auto-Submitted") == 0) &&
        (strstr (value, "auto-"))
    ) {
        _xautoreply = true;
    }
}


std::string FeriasMilter::b64decode(const std::string &in)
{
    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 

    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0) {
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}

bool FeriasMilter::shouldSendAutoreply (const std::string &to, std::string &subj, std::string &body)
{
    if (to.empty ()) return false;

    bool active = false;
    std::string url = _base_url + "/autoreply/" + to;
    std::string json = http_get (url);
    if (json.empty ()) return false;
    vsjson *vsj = vsjson_new (json.c_str ());
    const char *token = vsjson_first_token (vsj);
    while (token) {
        if (streq (token, "\"active\"")) {
            token = vsjson_next_token (vsj);
            if (!token || !streq (token, ":")) break;
            token = vsjson_next_token (vsj);
            if (token && !streq (token, "1")) {
                active = true;
            } else {
                active = false;
                break;
            }
        }
        else if (strcmp (token, "\"subject\"") == 0) {
            token = vsjson_next_token (vsj);
            if (!token || !streq (token, ":")) {
                active = false;
                break;
            }
            token = vsjson_next_token (vsj);
            if (token && token[0] == '"') {
                subj = token[1];
                subj.pop_back();
            } else {
                active = false;
                break;
            }
        }
        else if (strcmp (token, "\"body\"") == 0) {
            token = vsjson_next_token (vsj);
            if (!token || !streq (token, ":")) {
                active = false;
                break;
            }
            token = vsjson_next_token (vsj);
            if (token && token[0] == '"') {
                body = token[1];
                body.pop_back();
                body = b64decode (body);
            } else {
                active = false;
                break;
            }
        }
        token = vsjson_next_token (vsj);
    }
    vsjson_destroy (&vsj);
    return active;
}

void FeriasMilter::sendAutoreply() {
    if (_xautoreply || _mailinglist || _from.empty() || _to.empty()) return;
    for (const auto to: _to) {
        std::string subj;
        std::string body;
        if (shouldSendAutoreply (to, subj, body)) {
            Mailer m;
            std::string headers =
                "X-Autoreply: auto-replied\r\n"
                "X-Autorespond: auto-replied\r\n"
                "Auto-Submitted: auto-generated\r\n"
                "Content-Type: text/plain; charset=\"utf-8\"\r\n";
            m.send (_from, to, headers + "Subject: " + subj + "\r\n\r\n" + body);
        }
    }
}
