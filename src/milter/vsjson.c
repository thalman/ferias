#include "vsjson.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

enum JsonState {
    NONE,
    DICT,
    ARRAY
};

struct vsjson {
    int state;
    const char *cursor;
    char *text;
    char *token;
    int tokensize;
};

vsjson *vsjson_new (const char *json)
{
    if (!json) return NULL;
    vsjson *self = (vsjson *)malloc (sizeof (vsjson));
    if (!self) return NULL;
    
    memset(self, 0, sizeof(vsjson));
    self->text = strdup (json);
    return self;
}

const char *_vsjson_set_token (vsjson *self, const char *ptr, size_t len)
{
    if (!ptr || !self) return NULL;
    
    if (!len) len = strlen (ptr);
    if (self->tokensize > len + 1) {
        // fits in
        strncpy (self->token, ptr, len);
        self->token[len] = 0;
        return self->token;
    }
    if (self->token) {
        free (self->token);
        self->token = NULL;
        self->tokensize = 0;
    }
    self->token = (char *) malloc (len + 1);
    if (!self->token) return NULL;
    strncpy (self->token, ptr, len);
    self->token[len] = 0;
    self->tokensize = len+1;
    return self->token;
}

const char* _vsjson_seek_to_next_token(vsjson *self)
{
    if (!self) return NULL;
    
    while (true) {
        if (self->cursor == NULL) return NULL;
        if (! isspace (self->cursor[0])) return self->cursor;
        self->cursor++;
    }
}

const char* _vsjson_find_next_token(vsjson *self, const char *start)
{
    if (!self) return NULL;
    
    const char *p = start;
    if (!start) p = self->text;
    while (true) {
        if (*p == 0) return NULL;
        if (!isspace(*p)) return p;
        p++;
    }
}

const char* _vsjson_find_string_end(vsjson *self, const char *start)
{
    if (!self || !start) return NULL;

    const char *p = start;
    if (*p != '"') return NULL;
    ++p;
    while (true) {
        switch(*p) {
        case 0:
            return NULL;
        case '\\':
            ++p;
            if (*p == 0) return NULL;
            break;
        case '"':
                return ++p;
        }
        ++p;
    }
}

const char* _vsjson_find_number_end(vsjson *self, const char *start)
{
    if (!self || !start) return NULL;

    const char *p = start;
    if (!(isdigit (*p) || *p == '-' || *p  == '+')) return NULL;
    ++p;
    while (true) {
        if (*p == 0) return NULL;
        if(isdigit (*p) || *p == '.' || *p == 'e' || *p == 'E' || *p == '-' || *p == '+') {
            ++p;
        } else {
            return p;
        }
    }
}

const char* _vsjson_find_keyword_end(vsjson *self, const char *start)
{
    if (!self || !start) return NULL;

    const char *p = start;
    if (!isalpha (*p)) return NULL;
    ++p;
    while (true) {
        if (*p == 0) return NULL;
        if(isalpha (*p)) {
            ++p;
        } else {
            return p;
        }
    }
}

const char* _vsjson_find_token_end(vsjson *self, const char *start)
{
    if (!self || !start) return NULL;

    const char *p = start;
    if (strchr ("{}[]:,",*p)) {
        return ++p;
    }
    if (*p == '"') {
        return _vsjson_find_string_end (self, p);
    }
    if (strchr ("+-0123456789", *p)) {
        return _vsjson_find_number_end (self, p);
    }
    if (isalpha (*p)) {
        return _vsjson_find_keyword_end (self, p);
    }
    return NULL;
}

const char* vsjson_first_token (vsjson *self)
{
    if (!self) return NULL;
    self->cursor = _vsjson_find_next_token (self, NULL);
    if (!self->cursor) return NULL;
    const char *p = _vsjson_find_token_end (self, self->cursor);
    if (p) {
        _vsjson_set_token (self, self->cursor, p - self->cursor);
        self->cursor = p;
        return self->token;
    }
    return NULL;
}

const char* vsjson_next_token (vsjson *self)
{
    if (!self) return NULL;
    self->cursor = _vsjson_find_next_token (self, self->cursor);
    if (!self->cursor) return NULL;
    const char *p = _vsjson_find_token_end (self, self->cursor);
    if (p) {
        _vsjson_set_token (self, self->cursor, p - self->cursor);
        self->cursor = p;
        return self->token;
    }
    return NULL;
}

void vsjson_destroy (vsjson **self_p)
{
    if (!self_p) return;
    if (!*self_p) return;
    vsjson *self = *self_p;
    if (self->text) free (self->text);
    if (self->token) free (self->token);
    free (self);
    *self_p = NULL;
}

void test_vsjson() {
    vsjson *v = vsjson_new ("{ \"a\":\"avalue\"\n,\n\"b\": +31.4e-1, 3 : null, \"array\": [1,2,3]}");
    assert(v);
    const char *t = vsjson_first_token (v);
    while (t) {
        printf("%s\n", t);
        t = vsjson_next_token (v);
    }
    vsjson_destroy (&v);

    // empty dict
    v = vsjson_new ("{}  ");
    t = vsjson_first_token (v);
    assert (!strcmp (t, "{"));
    t = vsjson_next_token (v);
    assert (!strcmp (t, "}"));
    t = vsjson_next_token (v);
    assert (t == NULL);
    vsjson_destroy (&v);

    // empty string
    v = vsjson_new ("");
    t = vsjson_first_token (v);
    assert (t == NULL);
    vsjson_destroy (&v);

    // broken json
    v = vsjson_new ("[1");
    t = vsjson_first_token (v);
    t = vsjson_next_token (v);
    assert (t == NULL);
    vsjson_destroy (&v);

    v = vsjson_new ("{ \"key:1");
    t = vsjson_first_token (v);
    t = vsjson_next_token (v);
    assert (t == NULL);
    vsjson_destroy (&v);

    return;
}

/*
int main() {
    test_vsjson();
}
*/
