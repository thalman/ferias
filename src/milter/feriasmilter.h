#ifndef __SRC_MILTER_FERIASMILTER__
#define __SRC_MILTER_FERIASMILTER__

#include <string>
#include <vector>

class FeriasMilter {
 public:
    FeriasMilter();
    std::string from () const;
    void from (const char *from);
    void rcpt (const char *rcpt);
    void header (const char *header);
    void sendAutoreply();
 private:
    std::string _base_url;
    std::string _from;
    std::vector <std::string> _to;
    bool _xautoreply = false;
    bool _mailinglist = false;

    std::string b64decode(const std::string &in);
    bool shouldSendAutoreply (const std::string &to, std::string &subj, std::string &body);
    //void sendMail();
};

#endif // __SRC_MILTER_FERIASMILTER__
