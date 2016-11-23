#ifndef __SRC_MILTER_CONFIGURATION__
#define __SRC_MILTER_CONFIGURATION__

#define SOCKET "unix:./ferias.sock"
#define BASE_URL "http://localhost/ferias"

const char *cfg_socket();
const char *cfg_base_url();

#endif // __SRC_MILTER_CONFIGURATION__
