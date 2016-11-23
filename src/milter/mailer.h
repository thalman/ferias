#ifndef __SRC_MILTER_MAILER__
#define __SRC_MILTER_MAILER__

#include <string>

class Mailer {
 public:
    bool send (const std::string& from, const std::string& to, const std::string &mail);
    ~Mailer () { disconnect (); }
 private:
    int _sock = -1;
    char _buf[BUFSIZ+1];

    bool connected () { return _sock != -1; };
    void write (const std::string& s);
    std::string read ();
    bool connect ();
    void disconnect ();
};

#endif // __SRC_MILTER_MAILER__

