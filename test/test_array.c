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
#include <stdlib.h>
#include <string.h>

int counter = 0;

static void callback1(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "hobbies") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "Hiking") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "Cooking") == 0); counter++; }
    }
}

static void testStringArray() {
    char *json = "[\"Reading\", \"Hiking\", \"Cooking\"]";
    nanojsonc_parse_array(json, "hobbies", NULL, callback1);
    assert(counter == 3);
    counter = 0;
}

static void callback2(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "numbers") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "22") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "333") == 0); counter++; }
        if (strcmp(key, "[3]") == 0) { assert(strcmp(value, "4444") == 0); counter++; }
        if (strcmp(key, "[4]") == 0) { assert(strcmp(value, "55555") == 0); counter++; }
        if (strcmp(key, "[5]") == 0) { assert(strcmp(value, "-10") == 0); counter++; }
        if (strcmp(key, "[6]") == 0) { assert(strcmp(value, "-500") == 0); counter++; }
        if (strcmp(key, "[7]") == 0) { assert(strcmp(value, "-1000") == 0); counter++; }
        if (strcmp(key, "[8]") == 0) { assert(strcmp(value, "40.5") == 0); counter++; }
    }
}

static void testNumberArray() {
    char *json = "[1, 22, 333, 4444, 55555, -10, -500, -1000, 40.5]";
    nanojsonc_parse_array(json, "numbers", NULL, callback2);
    assert(counter == 9);
    counter = 0;
}

static void callback3(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    counter++;
}

static void testEmptyArray() {
    nanojsonc_parse_array("", "empty", NULL, callback3);
    assert(counter == 0);
    nanojsonc_parse_array("[]", "empty", NULL, callback3);
    assert(counter == 0);
    nanojsonc_parse_array(NULL, "empty", NULL, callback3);
    assert(counter == 0);
    nanojsonc_parse_array(NULL, NULL, NULL, callback3);
    assert(counter == 0);
}

static void callback4(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "single") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "2023") == 0); counter++; }
    }
}

static void testSingleItemArray() {
    char *json = "[2023]";
    nanojsonc_parse_array(json, "single", NULL, callback4);
    assert(counter == 1);
    counter = 0;
}

static void callback5(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "boolean") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if(strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if(strcmp(key, "[2]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
    }
}

static void testBooleanArray() {
    char *json = "[true, false, true]";
    nanojsonc_parse_array(json, "boolean", NULL, callback5);
    assert(counter == 3);
    counter = 0;
}

static void callback6(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "Null") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if(strcmp(key, "[1]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if(strcmp(key, "[2]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
    }
}

static void testNullArray() {
    char *json = "[null, null, null]";
    nanojsonc_parse_array(json, "Null", NULL, callback6);
    assert(counter == 3);
    counter = 0;
}

static void callback7(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "single[0]") == 0) {
        if(strcmp(key, "i") == 0) { assert(strcmp(value, "4") == 0); counter++; }
    }
}

static void testSingleObjectArray() {
    char *json = "[{\"i\": 4 }]";
    nanojsonc_parse_array(json, "single", NULL, callback7);
    assert(counter == 1);
    counter = 0;
}

static void callback8(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "multiple[0]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "2") == 0); counter++; }
    }
    if (strcmp(parentKey, "multiple[1]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "3") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "4") == 0); counter++; }
    }
}

static void testMultipleObjectsArray() {
    char *json = "[{\"a\":1,\"b\":2}, {\"a\":3,\"b\":4}]";
    nanojsonc_parse_array(json, "multiple", NULL, callback8);
    assert(counter == 4);
    counter = 0;
}

static void callback9(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "boolean[0]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
    }
    if (strcmp(parentKey, "boolean[1]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    }
}

static void testNestedArray() { // has to be parsed list, not whole values
    char *json = "[[true, false, true], [false, false]]";
    nanojsonc_parse_array(json, "boolean", NULL, callback9);
    assert(counter == 5);
    counter = 0;
}

static void callback10(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "mixed") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "2020") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if (strcmp(key, "[3]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if (strcmp(key, "[4]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    } else if (strcmp(parentKey, "mixed[5]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "2") == 0); counter++; }
    }
}

static void testMixedArray() {
    char *json = "[\"Reading\", 2020, null, true, false, {\"a\": 1, \"b\": 2}]";
    nanojsonc_parse_array(json, "mixed", NULL, callback10);
    assert(counter == 7);
    counter = 0;
}

static void callback11(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "data[0][name]") == 0) {
        if (strcmp(key, "first") == 0) { assert(strcmp(value, "Janie") == 0); counter++; }
        if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    }
    if (strcmp(parentKey, "data[0]") == 0) {
        if (strcmp(key, "age") == 0) { assert(atoi(value) == 13); counter++; }
    }
}

static void testNestedObject() {
    char *json = "[{\"name\": {\"first\": \"Janie\",\"last\": \"Doe\"},\"age\": 13}]";
    nanojsonc_parse_array(json, "data", NULL, callback11);
    assert(counter == 3);
    counter = 0;
}

static void callback12(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "data") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "k") == 0); counter++; }
    } else if (strcmp(parentKey, "data[1]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "r") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "h") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "c") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "a") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "b") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2][2]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "x") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "y") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "z") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2][2][3]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "j") == 0); counter++; }
    }
}

static void testDeepNestedArray() {
    char *json = "[\"k\", [\"r\", \"h\", \"c\"], [\"a\", \"b\", [\"x\", \"y\", \"z\", [\"j\"]]]]]";
    nanojsonc_parse_array(json, "data", NULL, callback12);
    assert(counter == 10);
    counter = 0;
}

static void callback13(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testKeyOverflow() { // // 127 key length to cause fail at formatting
    char *json = "[\"abc\"]";
    nanojsonc_parse_array(json, "a7gPE3QAWDj5XlXaZQ2MH0fcUWttbTwvuDQiw1dDI1j3Ge6qjaFxthfrZOxTGA09ufgcDDlBqt0MHVzlRwKg0TMhq1bcDFD00IERLzgpVQAMkHmgeyyHDiBwb93EEwJ", NULL, callback13);
    assert(counter == 10); // indexes are integers, overflow not expected
    counter = 0;
}

static void callback14(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_VALUE_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testStringValueOverflow() {
    char *json = "[\"ZuhiccKTmfXqWP9H5F8aJeTwKKnJTk33Xl6KG6mbCk62aaL6DpS3TnOurRJAy6rqMzQXkq87oGWM2tNf549NBVdbN36LARMkZg1KiMXf5Odpx1sKyhFjLJUNJw8RIo04jwnPSD9I6XKKGYzuJpSujjWmUksElh29cMiQO0ZXVKbkAIzefw3U4BurWzFr8SAJNoGaCgWxwvkD0BYBPzEBOS9sPB8LTSfwJN6LvR5xxAkIopmxg5sXFYnoZ63BcJaZXUiCdFs2tDmGYbwrLaeiYlfBA9avQrbq20IYLrLfhO46HnGnW64p01huEgGU5vTBzELtSHybFCUNOd0kahCl7hlUVhigYksBmkZkmP1CTzs0Cyt4g10ZKY7REEUGG3NdegwH8lB88TPyL15aApLxGGEbdBIpJHwsDK7raUNAxiihR2lqgwIqrKTY85YwRMPOAgomAiFp1F5SMTGEkRiTDZlI1kUdll9gPDP3XuhpQmGX1g68mmcCtC8YgKyRLFuHzCIjZu6g5YGJdMBeM7smIDYDzQHEI8Ye6zh2sQGy7VoioUfZb09Ej75LAQVpDknmYAe158uZjyqix6OQQJt0P6V1ewchfYyc71OXPy0kwHBbxrKY2ZjtQTzckPiKZFWO8Enblb8AkGGuIwSA6BxQ3ztrPx98JBO6GXARrIaxEttvQpJOKCZoSWps4gqsLUh7CzxKl0kCgHcWWJ6YFJ2ZUt8U15Qw2DzWqwzdiiGxROGS3toyWSpKmwkK7f9ckHhP0FHlxds4fhMuRWZe8inSqdg7VZVSzJdnHK4SbP26P2aXVYjFEMXwrKMBaG05IOnByJLHVcNANDKAbGq73hQ55ym2wbbwJYGAbQPCEFbcvHeBn1FCEtH6kI7HAP18ob0pL3PIroDyIldrGCG4CLLn9FNilebtJDOe5Ui4vGQfjMMOjN77qUs0Y9d8mgAEsloWF0i9GtEZBR8y4SQUzxvZPWmJNpeqd7Ib2T2shOZCrJFLbba7QYR8dZalB2b0Or15\"]";
    nanojsonc_parse_array(json, NULL, NULL, callback14);
    assert(counter == 1);
    counter = 0;
}

static void callback15(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_VALUE_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testNumberValueOverflow() {
    char *json = "[6003500149247669231418293478972132300390284248302418853060866937701264287542225554024661516641383745727590562766009913937263454070132903559048921939799810673528482218580849700223914988193092746378847444381184385345916979734359253911156307201336040521223898455340905819787077222046104513936357548886030420968286215107549321785327777894940960503563834510380574725327726428147711441768575138011782605228345369034484685819247767706773555779672898445490025977538790983933135656433300193103234706040674796728519871177707261920265993748449565058961317574858286980576991673545652319301644932825623433136133171277789656927675779881796837028112798626106928839892067436235712650192165857442854503808750826192589638141234334099459464912021316564160628870682262614963467039165851013599533630055959575960134833376207091706214192383211495389455481113227932368240339582406371125156668290770593927616397789162271238569661993617211731384523731184649562211717640696929021869356223590989442804218242186806400919317597754152877582953230673676218]";
    nanojsonc_parse_array(json, NULL, NULL, callback15);
    assert(counter == 1);
    counter = 0;
}

static void callback16(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_JSON_SIZE - 1); // truncated value
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedObjectValueOverflow() {
    char *json = "[{\"nested\": \"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\"}]";
    nanojsonc_parse_array(json, NULL, NULL, callback16);
    assert(counter == 1);
    counter = 0;
}

static void callback17(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedObjectKeyOverflow() { // 127 key length to cause fail at formatting
    char *json = "[{\"name\": \"John Doe\"}]";
    nanojsonc_parse_array(json, "NhMtsqB7SvQACbnJNrlkqEDDzb2xLVn1NuPIaebzoXjIbJddEAt5akc0sNDQ4Z13YEqvxldAIV6SbklMAFtIxhtFcPgHtjMDDuta5oKjGlwF649SxzMLO1X9WEyCA", NULL, callback17);
    assert(counter == 1);
    counter = 0;
}

static void callback18(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_JSON_SIZE - 1); // truncated value
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedArrayValueOverflow() { // 5120 character length to cause overflow
    char *json = "[[\"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\"]]";
    nanojsonc_parse_array(json, NULL, NULL, callback18);
    assert(counter == 1);
    counter = 0;
}

static void callback19(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedArrayKeyOverflow() {  // 127 key length to cause fail at formatting
    char *json = "[[\"abc\"]]";
    nanojsonc_parse_array(json, "h2RLjTVX4IFFZgEnV3U90dEhvqs6Ukym5GBhRYwXx7HmGoghN9aqdWtSe45i50rE0VPAL4deX1P3plxKyDpLQcYRE29cngTDUFyCLZ5jC25R52IXjNsdUuUtQjrKEWu", NULL, callback19);
    assert(counter == 1);
    counter = 0;
}

int main() {
    // flat arrays
    testStringArray();
    testNumberArray();
    testEmptyArray();
    testSingleItemArray();
    testBooleanArray();
    testNullArray();

    // nested arrays
    testSingleObjectArray();
    testMultipleObjectsArray();
    testNestedArray();
    testMixedArray();
    testNestedObject();
    testDeepNestedArray();

    testKeyOverflow();
    testStringValueOverflow();
    testNumberValueOverflow();

    testNestedObjectValueOverflow();
    testNestedObjectKeyOverflow();

    testNestedArrayValueOverflow();
    testNestedArrayKeyOverflow();

    return 0;
}
