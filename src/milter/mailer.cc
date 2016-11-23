#include "mailer.h"

// #include<iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define HELO "HELO ferias\r\n"
#define DATA "DATA\r\n"
#define QUIT "QUIT\r\n"

void Mailer::write(const std::string& s)
{
    if (! connected ()) return;
    ::write(_sock,s.c_str (), s.size());
}

std::string Mailer::read()
{
    if (! connected ()) return "";
    memset(_buf, 0, BUFSIZ);
    size_t len = ::read(_sock, _buf, BUFSIZ);
    if (len) return _buf;
    return "";
}

bool Mailer::connect()
{
    struct addrinfo ai, *servinfo;
    if (connected ()) disconnect ();
    
    int rv;
    memset(&ai, 0, sizeof (ai));
    ai.ai_family = AF_INET;
    ai.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo("127.0.0.1", "25", &ai, &servinfo)) != 0) return connected ();

    for(addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        _sock = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_sock == -1) {
            continue;
        }
        if (::connect (_sock, p->ai_addr, p->ai_addrlen) == -1) {
            close (_sock);
            _sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo (servinfo);
    return connected ();
}

void Mailer::disconnect()
{
    if (connected ()) {
        close (_sock);
        _sock = -1;
    }
}

bool Mailer::send (const std::string& from, const std::string& to, const std::string &mail)
{
    connect ();
    if (! connected ()) return false;
    read (); // greeting
    write (HELO);
    read (); // OK
    write ("MAIL FROM: "); 
    write (from);
    write ("\r\n");
    read (); // sender OK?
    write ("RCPT TO: "); // Mail to
    write (to);
    write ("\r\n");
    read (); // Recipient OK
    write (DATA);
    read (); // end data with .\r\n
    write (mail.c_str ());
    write ("\r\n.\r\n");
    read (); 
    write (QUIT); // quit
    read (); // log off
    disconnect ();
    return true;
}
