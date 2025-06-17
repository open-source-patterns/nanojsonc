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

/** @file object.c
 *  Parses the json-based object.
 */

#include "nanojsonc/parser.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/**
 * @brief Parse a JSON object and invoke a callback for each key-value pair.
 *
 * This function parses a JSON object and invokes the specified callback function
 * for each key-value pair within the object.
 *
 * @param json The JSON object to parse.
 * @param parentKey The parent key (if applicable).
 * @param object A pointer to the object being populated or processed.
 * @param callback The callback function to be invoked for each key-value pair.
 *
 * The callback function should have the following signature:
 * @code
 * void callback(enum NanoJSONCError error, const char *const key,
 *               const char *const value, const char *const parentKey,
 *               void *object);
 * @endcode
 *
 * @note The provided callback function should handle the processing or population
 *       of the object based on the parsed JSON key-value pairs.
 *
 * @see nanojsonc_parse_array
 */
void nanojsonc_parse_object(const char *const json, const char *const parentKey, void *object, const NanoJSONCCallback callback) {
    if (json == NULL) return;
    const char *start = json, *cursor = NULL, *parent = (parentKey == NULL) ? "" : parentKey;

    for (cursor = start; *cursor != '\0' && *cursor != '{'; cursor++) {} // begin brace
    if (*cursor) cursor++; // proceed to key-value pairs

    while (*cursor) {
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++) {} // whitespace

        // parse key
        for (; *cursor != '\0' && *cursor != '\"'; cursor++); // key: begin quote
        if (*cursor == '\0') return; // empty or no key-value pairs

        start = cursor + 1;
        for (cursor = start; *cursor != '\0' && *cursor != '\"'; cursor++) {} // key: end quote
        long len = cursor - start;

        char key[NANOJSONC_KEY_SIZE];
        memset(key, 0, NANOJSONC_KEY_SIZE);
        strncpy(key, start, len < NANOJSONC_KEY_SIZE ? len : NANOJSONC_KEY_SIZE - 1); // truncate large keys
        if (len >= NANOJSONC_KEY_SIZE) {
            callback(KEY_OVERFLOW, key, NULL, parent, object); // error
            return;
        }
        cursor++;

        // skip whitespace after the key
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++) {}

        if (*cursor) cursor++; // skip colon

        // skip whitespace before the value
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++) {}

        // nested object
        if (*cursor == '{') { // begin brace
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != '}'); cursor++) { // deep nested objects
                if (*cursor == '{') depth++; // causes to skip
                if (*cursor == '}') depth--;
            }
            cursor++; // include brace
            len = cursor - start;

            char subKey[NANOJSONC_KEY_SIZE];
            memset(subKey, 0, NANOJSONC_KEY_SIZE);
            if (snprintf(subKey, NANOJSONC_KEY_SIZE, "%s[%s]", parent, key) >= NANOJSONC_KEY_SIZE) { // parentKey with childKey
                callback(JSON_KEY_OVERFLOW, subKey, NULL, parent, object);
                return;
            }

            char value[NANOJSONC_JSON_SIZE]; // nested JSON
            memset(value, 0, NANOJSONC_JSON_SIZE);
            strncpy(value, start, len < NANOJSONC_JSON_SIZE ? len : NANOJSONC_JSON_SIZE - 1);
            if (len >= NANOJSONC_JSON_SIZE) {
                callback(JSON_VALUE_OVERFLOW, subKey, value, parent, object);
                return;
            }

            nanojsonc_parse_object(value, subKey, object, callback);
        }

        // nested array
        if (*cursor == '[') { // begin bracket
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != ']'); cursor++) { // deep nested arrays
                if (*cursor == '[') depth++;
                if (*cursor == ']') depth--;
            }
            cursor++; // include bracket
            len = cursor - start;

            char subKey[NANOJSONC_KEY_SIZE];
            memset(subKey, 0, NANOJSONC_KEY_SIZE);
            if (snprintf(subKey, NANOJSONC_KEY_SIZE, "%s[%s]", parent, key) >= NANOJSONC_KEY_SIZE) { // parentKey with childKey
                callback(JSON_KEY_OVERFLOW, subKey, NULL, parent, object);
                return;
            }

            char value[NANOJSONC_JSON_SIZE];
            memset(value, 0, NANOJSONC_JSON_SIZE);
            strncpy(value, start, len < NANOJSONC_JSON_SIZE ? len : NANOJSONC_JSON_SIZE - 1);
            if (len >= NANOJSONC_JSON_SIZE) {
                callback(JSON_VALUE_OVERFLOW, subKey, value, parent, object);
                return;
            }

            nanojsonc_parse_array(value, subKey, object, callback);
        }

        // parse values (string, number, boolean, null)

        if (*cursor == '"') { // value: string
            start = cursor + 1;
            for (cursor = start; *cursor != '\0' && *cursor != '\"'; cursor++) {} // end quote
            len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            strncpy(value, start, len < NANOJSONC_VALUE_SIZE ? len : NANOJSONC_VALUE_SIZE - 1); // truncate larger values
            callback(len >= NANOJSONC_VALUE_SIZE ? VALUE_OVERFLOW : NO_ERROR, key, value, parent, object); // error for larger values
            if (len >= NANOJSONC_VALUE_SIZE) return;
            cursor++;
        }

        if (isdigit(*cursor) || *cursor == '-') { // value: number
            start = cursor;
            cursor++;
            for (; *cursor != '\0' && (isdigit(*cursor) || *cursor == '.'); cursor++) {} // end digit (non-whitespace)
            len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            strncpy(value, start, len < NANOJSONC_VALUE_SIZE ? len : NANOJSONC_VALUE_SIZE - 1); // truncate larger values
            callback(len >= NANOJSONC_VALUE_SIZE ? VALUE_OVERFLOW : NO_ERROR, key, value, parent, object); // error for larger values
            if (len >= NANOJSONC_VALUE_SIZE) return;
        }

        if (*cursor == 't' || *cursor == 'f' || *cursor == 'n') { // boolean (true/false) or null
            start = cursor;
            for (; *cursor != '\0' && !isspace(*cursor) && *cursor != ',' && *cursor != '}'; cursor++) {}
            len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            strncpy(value, start, len);
            callback(NO_ERROR, key, value, parent, object);
        }

        // skip whitespace after the value
        for (; *cursor != '\0' && isspace(*cursor); cursor++) {}

        if (*cursor) cursor++; // either EOF or comma
    }
}
