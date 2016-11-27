#ifndef __SRC_MILTER_CONFIGURATION__
#define __SRC_MILTER_CONFIGURATION__

const char *cfg_socket();
const char *cfg_base_url();
bool cfg_parse_commandline(int argc, char *argv[]);
void cfg_free();

#endif // __SRC_MILTER_CONFIGURATION__
