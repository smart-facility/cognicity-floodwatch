/*
 * we need to use a strdup and pebble don't have one, so:
 */

#include <pebble.h>
#include "strdup.h"

char *
strdup(const char *s)
{
    char *p;

    if (s == NULL)
        return NULL;

    if ((p = (char *)malloc(strlen(s)+1)) == NULL)
        return NULL;

    strcpy(p, s);

    return p;
}
