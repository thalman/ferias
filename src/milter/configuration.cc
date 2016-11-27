#include "configuration.h"
#include <iostream>
#include <limits.h>
#include <string.h>
#include <getopt.h>

#define URL_MAX 2000
#define DEFAULT_SOCKET "unix:./ferias.sock"
#define DEFAULT_URL "http://localhost/ferias"

typedef struct {
   char url[PATH_MAX + 1];
   char socket[URL_MAX + 1];
} Config;

static Config cfg;

const char *cfg_socket() { return cfg.socket; }

const char *cfg_base_url() { return cfg.socket; }

void usage() {
    std::cerr << "usage: ferias [options]" << std::endl
              << "options:" << std::endl
              << "  -s|--socket   milter socket address [" << DEFAULT_SOCKET << "]" << std::endl
              << "  -u|--url      URL address of ferias restapi server [" << DEFAULT_URL << "]" << std::endl;
}

bool cfg_parse_commandline(int argc, char *argv[]) {
    // set defaults
    memset (&cfg, 0, sizeof (cfg));
    strncpy (cfg.url, DEFAULT_URL, URL_MAX);
    strncpy (cfg.socket, DEFAULT_SOCKET, PATH_MAX);
    
    static struct option long_options[] = {
        {"socket", required_argument, NULL, 's'},
        {"url", required_argument, NULL, 'u'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    
    char ch;
    while ((ch = getopt_long(argc, argv, "hs:u:", long_options, NULL)) != -1) {
        switch (ch) {
        case 'h':
            usage ();
            return false;
        case 's':
            strncpy (cfg.socket, optarg, PATH_MAX);
            break;
        case 'u':
            strncpy (cfg.url, optarg, URL_MAX);
            break;
        default:
            usage ();
            return false;
        }
    }
    return true;
}
