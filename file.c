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

#include <stdio.h>
#include <stdlib.h>
#include "file.h"

// Tuple[<long>filelen, <char*>buffer]
Tuple readBytes(char* file) {
    FILE* fileptr = fopen(file, "rb");

    fseek(fileptr, 0, SEEK_END);

    long filelen = ftell(fileptr);

    rewind(fileptr);

    char* buffer = (char*) malloc(filelen * sizeof(char));

    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    return tupleOf((void*) filelen, (void*) buffer);
}

void write(char* file, char* data) {
    FILE* fileptr = fopen(file, "w+");

    if (fileptr!= NULL) {
        fputs(data, fileptr);
        fclose(fileptr);
    }
}