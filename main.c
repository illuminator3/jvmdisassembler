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

#include "class.h"
#include "file.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: jvmd <File> <Output>\n");

        return -1;
    }

    char* file = argv[1];
    Tuple bytes = readBytes(file);
    long filelen = (long) bytes.a;
    char* content = (char*) bytes.b;
    ClassFile class = readClassFile(bytesOf(content, filelen));
    char* out = writeClassFile(class);
    char* dst = argv[2];

    write(dst, out);

    return 0;
}