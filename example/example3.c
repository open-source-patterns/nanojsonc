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

#include "parser.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    struct Name {
        char *first;
        char *last;
    } *name;
    int age;
    char *phone;
    bool isStudent;
    bool isEmployed;
    struct Hobby {
        char *name;
        struct Hobby *next;
    } *hobbies;
    struct Address {
        char *street;
        char *city;
        char *state;
        char *zip;
    } *address;
    struct Child {
        struct Name *name;
        int age;
        struct Child *next;
    } *children;
};

static void callback(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error != NO_ERROR) { printf("%s\n", nanojsonc_error_desc(error)); return; } // guard statement

    struct Person **person = object;

    if (*person == NULL) {
        *person = malloc(sizeof(struct Person));
        **person = (struct Person){0}; // Initialize all members to 0
    }

    if (strcmp(parentKey, "data[name]") == 0) {
        if ((*person)->name == NULL) {
            (*person)->name = malloc(sizeof(struct Name));
            *(*person)->name = (struct Name){0};
        }
        if (strcmp(key, "first") == 0) (*person)->name->first = strdup(value);
        if (strcmp(key, "last") == 0) (*person)->name->last = strdup(value);
    }

    if (strcmp(parentKey, "data") == 0) {
        if (strcmp(key, "age") == 0)(*person)->age = atoi(value);
        if (strcmp(key, "phone") == 0) (*person)->phone = strdup(value);
        if (strcmp(key, "isStudent") == 0)
            (*person)->isStudent = (strcmp(value, "true") == 0) ? true : false;
        if (strcmp(key, "isEmployed") == 0)
            (*person)->isEmployed = (strcmp(value, "true") == 0) ? true : false;
    }

    // hobbies
    if (strcmp(parentKey, "data[hobbies]") == 0) {
        struct Hobby *hobby = malloc(sizeof(struct Hobby));
        *hobby = (struct Hobby){0};
        hobby->name = strdup(value);
        hobby->next = NULL;

        struct Hobby **cursor;
        for (cursor = &(*person)->hobbies; *cursor; cursor = &(*cursor)->next);
        *cursor = hobby;
    }

    // address
    if (strcmp(parentKey, "data[address]") == 0) {
        if ((*person)->address == NULL) {
            (*person)->address = malloc(sizeof(struct Address));
            *(*person)->address = (struct Address){0};
        }
        if (strcmp(key, "street") == 0) (*person)->address->street = strdup(value);
        if (strcmp(key, "city") == 0) (*person)->address->city = strdup(value);
        if (strcmp(key, "state") == 0) (*person)->address->state = strdup(value);
        if (strcmp(key, "zip") == 0) (*person)->address->zip = strdup(value);
    }

    // children
    if (strncmp("data[children]", parentKey, strlen("data[children]")) == 0) {
        if (strcmp(key, "first") == 0) {
            struct Child *child = malloc(sizeof(struct Child));
            *child = (struct Child){0};

            child->name = malloc(sizeof(struct Name));
            *child->name = (struct Name){0};

            child->next = NULL;
            child->name->first = strdup(value);

            struct Child **cursor;
            for (cursor = &(*person)->children; *cursor; cursor = &(*cursor)->next);
            *cursor = child;
        }
        if (strcmp(key, "last") == 0) {
            struct Child **cursor;
            for (cursor = &(*person)->children; *cursor != NULL && (*cursor)->next != NULL; cursor = &(*cursor)->next);
            (*cursor)->name->last = strdup(value);
        }
        if (strcmp(key, "age") == 0) {
            struct Child **cursor;
            for (cursor = &(*person)->children; *cursor != NULL && (*cursor)->next != NULL; cursor = &(*cursor)->next);
            (*cursor)->age = atoi(value);
        }
    }
}

void verify(struct Person *person) {
    assert(person);
    assert(strcmp(person->name->first, "John") == 0);
    assert(strcmp(person->name->last, "Doe") == 0);
    assert(person->age == 30);
    assert(strcmp(person->phone, "null") == 0);
    assert(person->isStudent == false);
    assert(person->isEmployed == true);

    // hobbies
    assert(strcmp(person->hobbies->name, "Reading") == 0);
    assert(strcmp(person->hobbies->next->name, "Hiking") == 0);
    assert(strcmp(person->hobbies->next->next->name, "Cooking") == 0);

    // address
    assert(strcmp(person->address->street, "123 Main St") == 0);
    assert(strcmp(person->address->city, "Asgard") == 0);
    assert(strcmp(person->address->state, "CA") == 0);
    assert(strcmp(person->address->zip, "12345") == 0);

    // children
    assert(strcmp(person->children->name->first, "Janie") == 0);
    assert(strcmp(person->children->name->last, "Doe") == 0);
    assert(person->children->age == 13);
    assert(strcmp(person->children->next->name->first, "Johnny") == 0);
    assert(strcmp(person->children->next->name->last, "Doe") == 0);
    assert(person->children->next->age == 14);
}

void person_free(struct Person **person) {
    free((*person)->name->first);
    free((*person)->name->last);
    free((*person)->name);
    (*person)->name = NULL;

    free((*person)->phone);

    // hobbies
    for (struct Hobby **cursor = &(*person)->hobbies; *cursor;) {
        struct Hobby *node = *cursor;
        *cursor = (*cursor)->next;
        free(node->name);
        free(node);
    }
    (*person)->hobbies = NULL;

    // address
    free((*person)->address->street);
    free((*person)->address->city);
    free((*person)->address->state);
    free((*person)->address->zip);
    free((*person)->address);
    (*person)->address = NULL;

    // children
    for (struct Child **cursor = &(*person)->children; *cursor;) {
        struct Child *node = *cursor;
        *cursor = (*cursor)->next;
        free(node->name->first);
        free(node->name->last);
        free(node->name);
        free(node);
    }
    (*person)->children = NULL;

    free(*person);
    *person = NULL;
}

int main() {
    FILE *file = fopen("example3.json", "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: %d: %s\n", errno, strerror(errno));
        return errno;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking file");
        fclose(file);
        return errno;
    }
    long len = ftell(file);
    rewind(file);

    char *buffer = (char *) malloc(len + 1);
    if (buffer == NULL) {
        fclose(file);
        perror("Error allocating memory");
        return ENOMEM;
    }

    size_t bytes_read = fread(buffer, 1, len, file);
    if (bytes_read != (size_t) len) {
        if (feof(file)) {
            perror("Error: end of file reached before reading expected bytes.");
            fclose(file);
            free(buffer);
            return EIO;
        } else if (ferror(file)) {
            perror("Error reading the file");
            fclose(file);
            free(buffer);
            return errno;
        }
    }
    buffer[len] = '\0';
    fclose(file);

    struct Person *person = NULL;
    nanojsonc_parse_object(buffer, "data", &person, callback);
    verify(person);

    person_free(&person);
    free(buffer);

    return 0;
}
