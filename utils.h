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

#ifndef JVMDISASSEMBLER_UTILS_H
#define JVMDISASSEMBLER_UTILS_H

#define generic void*

typedef struct {
    generic a;
    generic b;
} Tuple;

typedef enum {
    false = 0,
    true  = 1
} boolean;

Tuple tupleOf(void* a, void* b);

typedef struct {
    char* content;
    int length;
} String;

String stringEmpty();
String stringBy(char* content);
String stringOf(char* content, int length);

void prepend(String* to, String src);
void prependChar(String* to, char c);
void append(String* to, String src);
void appendChar(String* to, char c);
char charAt(String str, int pos);

char* content(String str);
String substring(String str, int from, int to);

char* replaceString(char* string, const char* substr, const char* replacement);

#endif