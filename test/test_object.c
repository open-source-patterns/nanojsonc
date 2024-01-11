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
#include <string.h>

int counter = 0;

static void callback(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    counter++;
}

static void testEmptyObject() {
    nanojsonc_parse_object("", NULL, NULL, callback);
    assert(counter == 0);
    nanojsonc_parse_object("{}", NULL, NULL, callback);
    assert(counter == 0);
    nanojsonc_parse_object(NULL, "empty", NULL, callback);
    assert(counter == 0);
    nanojsonc_parse_object(NULL, NULL, NULL, callback);
    assert(counter == 0);
}

static void callback1(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "phone") == 0) { assert(strcmp(value, "") == 0); counter++; }
}

static void testStringProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"phone\": \"\"}";
    nanojsonc_parse_object(json, NULL, NULL, callback1);
    assert(counter == 3);
    counter = 0;
}

static void callback2(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "age") == 0) { assert(strcmp(value, "15") == 0); counter++; }
}

static void testNumberProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"age\": 15}";
    nanojsonc_parse_object(json, NULL, NULL, callback2);
    assert(counter == 3);
    counter = 0;
}

static void callback3(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "isMarried") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    if (strcmp(key, "isEmployed") == 0) { assert(strcmp(value, "true") == 0); counter++; }
}

static void testBooleanProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"isMarried\": false, \"isEmployed\": true}";
    nanojsonc_parse_object(json, NULL, NULL, callback3);
    assert(counter == 4);
    counter = 0;
}

static void callback4(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "phone") == 0) { assert(strcmp(value, "null") == 0); counter++; }
}

static void testNullValue() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"phone\": null}";
    nanojsonc_parse_object(json, NULL, NULL, callback4);
    assert(counter == 3);
    counter = 0;
}

static void callback5(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "") == 0) {
        if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
        if (strcmp(key, "age") == 0) { assert(strcmp(value, "15") == 0); counter++; }
    }
    if (strcmp(parentKey, "[hobbies]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "Hiking") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "Cooking") == 0); counter++; }
    }
}

static void testArrayProperty() {
    char *json = "{\"first\": \"John\", \"hobbies\": [\"Reading\", \"Hiking\", \"Cooking\"], \"age\": 15}";
    nanojsonc_parse_object(json, NULL, NULL, callback5);
    assert(counter == 5);
    counter = 0;
}

static void callback6(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "") == 0) {
        if (strcmp(key, "foo") == 0) { assert(strcmp(value, "bar") == 0); counter++; }
    }
    if (strcmp(parentKey, "[level1]") == 0) {
        assert(strcmp(key, "property1") == 0);
        assert(strcmp(value, "value1") == 0);
        counter++;
    } else if (strcmp(parentKey, "[level1][level2]") == 0) {
        assert(strcmp(key, "property2") == 0);
        assert(strcmp(value, "value2") == 0);
        counter++;
    } else if (strcmp(parentKey, "[level1][level2][level3]") == 0) {
        assert(strcmp(key, "property3") == 0);
        assert(strcmp(value, "value3") == 0);
        counter++;
    }
}

static void testDeepNested() {
    char *json = "{\"foo\": \"bar\", \"level1\": {\"property1\": \"value1\", \"level2\": {\"property2\": \"value2\", \"level3\": {\"property3\": \"value3\"}}}}";
    nanojsonc_parse_object(json, NULL, NULL, callback6);
    assert(counter == 4);
    counter = 0;
}

static void callback7(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        assert(strcmp(key, "xOuewxHu7Q1dlsvFSzcqiJH0JzOC558vb0Ql8ctq2ciOT8xKwZuHjK2cWQ0ncg95ELr7wcBaCL4O43U6pgHF0RS9mZDTlYpW2gB3KYevGa479k1cs3ugWpHn0bsfbMJ") == 0);
        counter++;
    } else {
        counter += 10;
    }
}

static void testKeyOverflow() { // key of length 128
    char *json = "{\"xOuewxHu7Q1dlsvFSzcqiJH0JzOC558vb0Ql8ctq2ciOT8xKwZuHjK2cWQ0ncg95ELr7wcBaCL4O43U6pgHF0RS9mZDTlYpW2gB3KYevGa479k1cs3ugWpHn0bsfbMJl\": \"John\"}";
    nanojsonc_parse_object(json, NULL, NULL, callback7);
    assert(counter == 1);
    counter = 0;
}

static void callback8(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_VALUE_SIZE - 1); // truncated value
        assert(strcmp(value, "XpO8OfDY8OM9hZUJbnZGOxQEGLl2mbuypPMluL0Ymi992TRZ3MhDWv9V8be3RLGtThDusozR0wLgHi5NrUnv4t21gi8iNAZMcmStuamW3DpMxMl31xfoULeTepZDH4ifJLGCvnh9PEBy03u3SG0YB56seVH1iwfGDB7s3aguU0Qt9df3FXPm3ORffJdaBBiVtfwfbP8uJu9A3IXV3cJlQd2647jIL2F5woSqsGcnii0bKEIMMvXmMPxZfmPZo29UZ2PGmx57KLsklMXhW4CUVwu7vpgyNrxugGYl4T8CLPqVGBHGtcdoiDRLJqVzE4qnLF5gw6YF561LGYJLUUPjWXecuK7KT2Wq1oh9Hg09pvbUsD1VdP5JXCtLkFvNsiHfREqAGJPs6PwymomWA1ex7F8w26AxeHfuanoL9FP33e6QGeTey1qPD0XvA64jtWfslZjPmXRfXIKrcSfcOgI1jazLdgLiYW53Enre4jRk632SOkKhxzqKpR5nHzPvdS2W9YcOyzqjWUFqtBb2lDRocDPDFookRhs0MzmROFZhqECQ3ZIPXT9iVSPLebI5ZORToPfchN899wAXX3HcrUSf7IHW1Db9688Ck58ZL9eT20iWbiLvWoAvTPpgCAjSGRB9ixDf4JYWnSefshQz5pLc2Yra76vPChMktxZh5uIpKjLqK4LK54Kj0erQ0s3uboEEP7MbRVrkewf3nu0HEWKYT267uLTAugWaw3za1bHYgBkgaejM9HjQ1IKGMA14PCUKSvFR07octhfzgpeMHQ8BQARPo6tkooVCO13czblnyTjOmdxVeeG3bNpdI3AiY0qvsh9fO6xTRO3UAj83cmDe5VTO0BKEtqZ49xGVHaiyxeqcAYJKXdfdjbIpHyh1cotyZg7iBH3GIgHE5xsiffUvMVeN6v70H64n7FyfA8XkPxPpBYzcJRSHCd5fZlSwrsaGQhFd1FXMxw0LfTa8tdYjW2QfNXP7CGWMZhNNypQRXvPUogAu0DHb2hd46z0tdFN") == 0);
        counter++;
    } else {
        counter += 10;
    }
}

static void testStringValueOverflow() { // string of length 1024
    char *json = "{\"lengthy\": \"XpO8OfDY8OM9hZUJbnZGOxQEGLl2mbuypPMluL0Ymi992TRZ3MhDWv9V8be3RLGtThDusozR0wLgHi5NrUnv4t21gi8iNAZMcmStuamW3DpMxMl31xfoULeTepZDH4ifJLGCvnh9PEBy03u3SG0YB56seVH1iwfGDB7s3aguU0Qt9df3FXPm3ORffJdaBBiVtfwfbP8uJu9A3IXV3cJlQd2647jIL2F5woSqsGcnii0bKEIMMvXmMPxZfmPZo29UZ2PGmx57KLsklMXhW4CUVwu7vpgyNrxugGYl4T8CLPqVGBHGtcdoiDRLJqVzE4qnLF5gw6YF561LGYJLUUPjWXecuK7KT2Wq1oh9Hg09pvbUsD1VdP5JXCtLkFvNsiHfREqAGJPs6PwymomWA1ex7F8w26AxeHfuanoL9FP33e6QGeTey1qPD0XvA64jtWfslZjPmXRfXIKrcSfcOgI1jazLdgLiYW53Enre4jRk632SOkKhxzqKpR5nHzPvdS2W9YcOyzqjWUFqtBb2lDRocDPDFookRhs0MzmROFZhqECQ3ZIPXT9iVSPLebI5ZORToPfchN899wAXX3HcrUSf7IHW1Db9688Ck58ZL9eT20iWbiLvWoAvTPpgCAjSGRB9ixDf4JYWnSefshQz5pLc2Yra76vPChMktxZh5uIpKjLqK4LK54Kj0erQ0s3uboEEP7MbRVrkewf3nu0HEWKYT267uLTAugWaw3za1bHYgBkgaejM9HjQ1IKGMA14PCUKSvFR07octhfzgpeMHQ8BQARPo6tkooVCO13czblnyTjOmdxVeeG3bNpdI3AiY0qvsh9fO6xTRO3UAj83cmDe5VTO0BKEtqZ49xGVHaiyxeqcAYJKXdfdjbIpHyh1cotyZg7iBH3GIgHE5xsiffUvMVeN6v70H64n7FyfA8XkPxPpBYzcJRSHCd5fZlSwrsaGQhFd1FXMxw0LfTa8tdYjW2QfNXP7CGWMZhNNypQRXvPUogAu0DHb2hd46z0tdFNi\"}";
    nanojsonc_parse_object(json, NULL, NULL, callback8);
    assert(counter == 1);
    counter = 0;
}

static void callback9(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_VALUE_SIZE - 1); // truncated value
        assert(strcmp(value, "600350014924766923141829347897213230039028424830241885306086693770126428754222555402466151664138374572759056276600991393726345407013290355904892193979981067352848221858084970022391498819309274637884744438118438534591697973435925391115630720133604052122389845534090581978707722204610451393635754888603042096828621510754932178532777789494096050356383451038057472532772642814771144176857513801178260522834536903448468581924776770677355577967289844549002597753879098393313565643330019310323470604067479672851987117770726192026599374844956505896131757485828698057699167354565231930164493282562343313613317127778965692767577988179683702811279862610692883989206743623571265019216585744285450380875082619258963814123433409945946491202131656416062887068226261496346703916585101359953363005595957596013483337620709170621419238321149538945548111322793236824033958240637112515666829077059392761639778916227123856966199361721173138452373118464956221171764069692902186935622359098944280421824218680640091931759775415287758295323067367621") == 0);
        counter++;
    } else {
        counter += 10;
    }
}

static void testNumberValueOverflow() { // number of length 1024
    char *json = "{\"lengthy\": 6003500149247669231418293478972132300390284248302418853060866937701264287542225554024661516641383745727590562766009913937263454070132903559048921939799810673528482218580849700223914988193092746378847444381184385345916979734359253911156307201336040521223898455340905819787077222046104513936357548886030420968286215107549321785327777894940960503563834510380574725327726428147711441768575138011782605228345369034484685819247767706773555779672898445490025977538790983933135656433300193103234706040674796728519871177707261920265993748449565058961317574858286980576991673545652319301644932825623433136133171277789656927675779881796837028112798626106928839892067436235712650192165857442854503808750826192589638141234334099459464912021316564160628870682262614963467039165851013599533630055959575960134833376207091706214192383211495389455481113227932368240339582406371125156668290770593927616397789162271238569661993617211731384523731184649562211717640696929021869356223590989442804218242186806400919317597754152877582953230673676218}";
    nanojsonc_parse_object(json, NULL, NULL, callback9);
    assert(counter == 1);
    counter = 0;
}
static void callback10(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_JSON_SIZE - 1); // truncated json
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedObjectValueOverflow() {
    char *json = "{\"nested\": {\"lengthy\": \"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\"}}";
    nanojsonc_parse_object(json, NULL, NULL, callback10);
    assert(counter == 1);
    counter = 0;
}

static void callback11(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedObjectKeyOverflow() { // 127 character key length to cause issue at formatting key nesting
    char *json = "{\"NhMtsqB7SvQACbnJNrlkqEDDzb2xLVn1NuPIaebzoXjIbJddEAt5akc0sNDQ4Z13YEqvxldAIV6SbklMAFtIxhtFcPgHtjMDDuta5oKjGlwF649SxzMLO1X9WEyCAdh\": {\"name\": \"John Doe\"}}";
    nanojsonc_parse_object(json, NULL, NULL, callback11);
    assert(counter == 1);
    counter = 0;
}

static void callback12(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_VALUE_OVERFLOW) {
        assert(strlen(value) == NANOJSONC_JSON_SIZE - 1); // truncated json
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedArrayValueOverflow() {
    char *json = "{\"nested\": [\"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\"]}";
    nanojsonc_parse_object(json, NULL, NULL, callback12);
    assert(counter == 1);
    counter = 0;
}

static void callback13(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (error == JSON_KEY_OVERFLOW) {
        assert(strlen(key) == NANOJSONC_KEY_SIZE - 1); // truncated key
        counter++;
    } else {
        counter += 10;
    }
}

static void testNestedArrayKeyOverflow() { // 127 key length to cause fail at formatting
    char *json = "{\"pFi9rjleBpCLzUQkFYD9vwlbQ28CvPOVL8I7M7tYZeIKM8lQnJT7NiKo1JUWpcvR7YJOtquGl1niB8AylrvBlfVHpcVHDC3fVgclVyUBwsCnMemjXGaynrCChgBFm9w\": [\"abc\"]}";
    nanojsonc_parse_object(json, NULL, NULL, callback13);
    assert(counter == 1);
    counter = 0;
}

int main() {
    testEmptyObject();
    testStringProperty();
    testNumberProperty();
    testBooleanProperty();
    testNullValue();
    testArrayProperty();

    testDeepNested();

    testKeyOverflow();
    testStringValueOverflow();
    testNumberValueOverflow();

    testNestedObjectValueOverflow();
    testNestedObjectKeyOverflow();

    testNestedArrayValueOverflow();
    testNestedArrayKeyOverflow();

    return 0;
}
