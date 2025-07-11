/**
BSD 3-Clause License

Copyright (c) 2023 Open Source Patterns Inc.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "nanojsonc/parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Hobby {
    char *name;
    struct Hobby *next;
};

static void callback(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error != NO_ERROR) { printf("%s\n", nanojsonc_error_desc(error)); return; } // guard statement

    struct Hobby **hobbies = object;

    struct Hobby *hobby = malloc(sizeof(struct Hobby));
    hobby->name = strdup(value);
    hobby->next = NULL;

    struct Hobby **cursor;
    for (cursor = hobbies; *cursor; cursor = &(*cursor)->next);

    *cursor = hobby;
}

int main(void) {
    char *json = "[\"Reading\", \"Hiking\", \"Cooking\"]";

    struct Hobby *hobbies = NULL;
    nanojsonc_parse_array(json, "hobbies", &hobbies, callback);

    for (struct Hobby **cursor = &hobbies; *cursor; cursor = &(*cursor)->next)
        printf("%s ", (*cursor)->name);

    assert(strcmp(hobbies->name, "Reading") == 0);
    assert(strcmp(hobbies->next->name, "Hiking") == 0);
    assert(strcmp(hobbies->next->next->name, "Cooking") == 0);

    for (struct Hobby **cursor = &hobbies; *cursor;) {
        struct Hobby *node = *cursor;
        *cursor = (*cursor)->next;
        free(node->name);
        free(node);
    }

    return 0;
}