#ifndef __VSJSON_H
#define __VSJSON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vsjson vsjson;

// minimalized json parser class
// returns new parser object
// parameter is json string
// call vsjson_destroy to free the parser
vsjson *vsjson_new (const char *json);

// destructor of json parser
void vsjson_destroy (vsjson **self_p);

// get first json token, usually "[" or "{"
// tokens are [ ] { } , : string (quote included)
// number or keyword like null
const char* vsjson_first_token (vsjson *self);

// get next json token
// walk trough json like this:
//     vsjson *parser = vsjson_new (jsonString);
//     const char *token = vsjson_first_token (parser);
//     while (token) {
//        printf ("%s ", token);
//        token = vsjson_next_token (parser);
//     }
//     printf ("\n");
//     vsjson_destroy (&parser);
const char* vsjson_next_token (vsjson *self);

#ifdef __cplusplus
}
#endif

#endif // __VSJSON_H
