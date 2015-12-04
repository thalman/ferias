#include <libmilter/mfapi.h>
#include "milterprivate.h"

/**
 * milter documentation http://www.elandsys.com/resources/sendmail/libmilter/api.html
 */

sfsistat ferias_envfrom (SMFICTX *ctx, char **argv) {
    MilterPrivateData *milter = (MilterPrivateData *)smfi_getpriv (ctx);
    if (milter) delete milter;
    milter = new MilterPrivateData();
    smfi_setpriv (ctx, milter);
    return SMFIS_CONTINUE;
}

sfsistat ferias_envrcpt (SMFICTX *ctx, char **argv) {
    return SMFIS_CONTINUE;
}

sfsistat ferias_header (SMFICTX *ctx, char *headerf, char *headerv) {
    return SMFIS_CONTINUE;
}

sfsistat ferias_abort (SMFICTX *ctx) {
    MilterPrivateData *milter = (MilterPrivateData *)smfi_getpriv (ctx);
    if (milter) {
        delete milter;
        smfi_setpriv (ctx, NULL);
    }
    return SMFIS_CONTINUE;
}

sfsistat ferias_eom (SMFICTX *ctx) {
    MilterPrivateData *milter = (MilterPrivateData *)smfi_getpriv (ctx);
    if (milter) {
        delete milter;
        // send message
        smfi_setpriv (ctx, NULL);
    }
    return SMFIS_CONTINUE;
}



struct smfiDesc feriasMilter =
{
        (char*)"FERIAS", // filter name
        SMFI_VERSION,    // version code
        SMFIF_NONE,      // flags
        NULL,            // connection info filter
        NULL,            // SMTP HELO command filter
        ferias_envfrom,  // envelope sender filter
        ferias_envrcpt,  // envelope recipient filter
        ferias_header,   // header filter
        NULL,            // end of header
        NULL,            // body block filter
        ferias_eom,      // end of message
        ferias_abort,    // message aborted
        NULL,            // connection cleanup
};

int main (int argc, char *argv[])
{
    if (smfi_register(feriasMilter) == MI_FAILURE) {
        fprintf(stderr, "smfi_register failed\n");
        return 1;
    }
    return 0;
}

