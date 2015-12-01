#include <libmilter/mfapi.h>

struct smfiDesc feriasMilter =
{
        (char*)"FERIAS", // filter name
        SMFI_VERSION,    // version code
        SMFIF_NONE,      // flags
        ferias_connect,  // connection info filter
        NULL,            // SMTP HELO command filter
        ferias_envfrom,  // envelope sender filter
        ferias_envrcpt,  // envelope recipient filter
        NULL,            // header filter
        ferias_header,   // end of header
        NULL,            // body block filter
        ferias_eom,      // end of message
        ferias_abort,    // message aborted
        ferias_close,    // connection cleanup
};

int main (int argc, char *argv[])
{
    if (smfi_register(feriasMilter) == MI_FAILURE) {
        fprintf(stderr, "smfi_register failed\n");
        exit(EX_UNAVAILABLE);
    }
}

