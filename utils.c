/*
   Copyright 2021 illuminator3

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Tuple tupleOf(void* a, void* b) {
    Tuple tuple = { a, b };

    return tuple;
}

String stringEmpty() {
    return stringOf("", 0);
}

String stringBy(char* content) {
    return stringOf(content, strlen(content));
}

String stringOf(char* content, int length) {
    String string = { content, length };

    return string;
}

void prepend(String* to, String src) {
    char* buffer = malloc(to->length + src.length);

    for (int i = src.length; i < to->length; i++) {
        buffer[i] = to->content[i];
    }

    for (int i = 0; i < src.length; i++) {
        buffer[i] = src.content[i];
    }

    to->length += src.length;
    to->content = buffer;
}

void prependChar(String* to, char c) {
    char* buffer = malloc(to->length + sizeof(char));

    for (int i = 1; i < to->length + 1; i++) {
        buffer[i] = to->content[i];
    }

    buffer[0] = c;

    to->length += 1;
    to->content = buffer;
}

void append(String* to, String src) {
    char* buffer = malloc(to->length + src.length);

    for (int i = 0; i < to->length; i++) {
        buffer[i] = to->content[i];
    }

    for (int i = 0; i < src.length; i++) {
        buffer[i + to->length] = src.content[i];
    }

    to->length += src.length;
    to->content = buffer;
}

void appendChar(String* to, char c) {
    char* buffer = malloc(to->length + sizeof(char));

    for (int i = 0; i < to->length; i++) {
        buffer[i] = to->content[i];
    }

    buffer[to->length] = c;

    to->length += 1;
    to->content = buffer;
}

char charAt(String str, int pos) {
    return str.content[pos];
}

char* content(String str) {
    char* buffer = malloc(str.length + sizeof(char));

    for (int i = 0; i < str.length; i++) {
        buffer[i] = str.content[i];
    }

    buffer[str.length] = '\0';

    return buffer;
}

String substring(String str, int from, int to) {
    String result = stringEmpty();

    for (int i = from; i <= to; i++) {
        appendChar(&result, charAt(str, i));
    }

    return result;
}

char* replaceString(char* string, const char* substr, const char* replacement) {
    char* tok = NULL;
    char* newstr = NULL;
    char* oldstr = NULL;
    int   oldstrLen;
    int   substrLen;
    int   replacementLen;

    newstr = strdup(string);
    substrLen = strlen(substr);
    replacementLen = strlen(replacement);

    if (substr == NULL || replacement == NULL) {
        return newstr;
    }

    while ((tok = strstr(newstr, substr))) {
        oldstr = newstr;
        oldstrLen = strlen(oldstr);
        newstr = (char*)malloc(sizeof(char) * (oldstrLen - substrLen + replacementLen + 1));

        if (newstr == NULL) {
            free(oldstr);
            return NULL;
        }

        memcpy(newstr, oldstr, tok - oldstr);
        memcpy(newstr + (tok - oldstr), replacement, replacementLen);
        memcpy(newstr + (tok - oldstr) + replacementLen, tok + substrLen, oldstrLen - substrLen - (tok - oldstr));
        memset(newstr + oldstrLen - substrLen + replacementLen, 0, 1);

        free(oldstr);
    }

    free(string);

    return newstr;
}