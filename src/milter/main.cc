#include <libmilter/mfapi.h>
#include <string.h>

#include "feriasmilter.h"

/**
 * milter documentation http://www.elandsys.com/resources/sendmail/libmilter/api.html
 */

sfsistat ferias_envfrom (SMFICTX *ctx, char **argv) {
    FeriasMilter *milter = (FeriasMilter *)smfi_getpriv (ctx);
    if (milter) delete milter;
    milter = new FeriasMilter();
    milter->from(argv[0]);
    smfi_setpriv (ctx, milter);
    return SMFIS_CONTINUE;
}

sfsistat ferias_envrcpt (SMFICTX *ctx, char **argv) {
    FeriasMilter *milter = (FeriasMilter *)smfi_getpriv (ctx);
    if (milter) milter->rcpt (argv[0]);    
    return SMFIS_CONTINUE;
}

sfsistat ferias_header (SMFICTX *ctx, char *headerf, char *headerv) {
    FeriasMilter *milter = (FeriasMilter *)smfi_getpriv (ctx);
    if (milter) milter->header(headerf);
    return SMFIS_CONTINUE;
}

sfsistat ferias_abort (SMFICTX *ctx) {
    FeriasMilter *milter = (FeriasMilter *)smfi_getpriv (ctx);
    if (milter) {
        delete milter;
        smfi_setpriv (ctx, NULL);
    }
    return SMFIS_CONTINUE;
}

sfsistat ferias_eom (SMFICTX *ctx) {
    FeriasMilter *milter = (FeriasMilter *)smfi_getpriv (ctx);
    if (milter) {
        milter->sendAutoreply();
        smfi_setpriv (ctx, NULL);
        delete milter;
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
    smfi_setconn ((char *)"unix:./ferias.sock");
    if (smfi_register(feriasMilter) == MI_FAILURE) {
        fprintf (stderr, "smfi_register failed\n");
        return 1;
    }
    return smfi_main();
}

