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
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

u1 u1of(ubyte first) {
    u1 u = { first };

    return u;
}

u2 u2of(ubyte first, ubyte second) {
    u2 u = { u1of(first), u1of(second) };

    return u;
}

u4 u4of(ubyte first, ubyte second, ubyte third, ubyte fourth) {
    u4 u = { u2of(first, second), u2of(third, fourth) };

    return u;
}

ubyte byteOf(u1 u) {
    return u.byte;
}

ushort shortOf(u2 u) {
    return (u.second.byte << 8) | u.first.byte;
}

uint intOf(u4 u) {
    return (u.second.second.byte << 24) + (u.second.first.byte << 16) + (u.first.second.byte << 8) + (u.first.first.byte << 0);
}

Bytes bytesOf(const char* content, uint size) {
    Bytes bytes = { (ubyte*) content, size, 0 };

    return bytes;
}

ubyte readByte(Bytes* bytes) {
    return bytes->actual[bytes->ptr++];
}

u1 readu1(Bytes* bytes) {
    return u1of(readByte(bytes));
}

u2 readu2(Bytes* bytes) {
    return u2of(readByte(bytes), readByte(bytes));
}

u4 readu4(Bytes* bytes) {
    return u4of(readByte(bytes), readByte(bytes), readByte(bytes), readByte(bytes));
}

u1* readu1array_(uint len, Bytes* bytes) {
    u1* r = (u1*) malloc(len * sizeof(u1));

    for (int i = 0; i < len; i++) {
        r[i] = readu1(bytes);
    }

    return r;
}

u1* readu1array(u2 len, Bytes* bytes) {
    ushort actualLen = shortOf(len);
    u1* r = (u1*) malloc(actualLen * sizeof(u1));

    for (int i = 0; i < actualLen; i++) {
        r[i] = readu1(bytes);
    }

    return r;
}

u2* readu2array(u2 len, Bytes* bytes) {
    ushort actualLen = shortOf(len);
    u2* r = (u2*) malloc(actualLen * sizeof(u2));

    for (int i = 0; i < actualLen; i++) {
        r[i] = readu2(bytes);
    }

    return r;
}

cp_info* readConstantPoolItem(Bytes* bytes) {
    u1 tag = readu1(bytes);
    ubyte actual = byteOf(tag);
    cp_info* r;

    switch (actual) {
        case 7: // CONSTANT_Class
            u2 name_index = readu2(bytes);
            CONSTANT_Class_info c_info = { tag, name_index };
            CONSTANT_Class_info* dynamic_c_info = (CONSTANT_Class_info*) malloc(sizeof(CONSTANT_Class_info));

            *dynamic_c_info = c_info;

            r = (cp_info*) dynamic_c_info;

            break;
        case 9: // CONSTANT_Fieldref
            u2 class_index = readu2(bytes);
            u2 name_and_type_index = readu2(bytes);
            CONSTANT_Fieldref_info fr_info = { tag, class_index, name_and_type_index };
            CONSTANT_Fieldref_info* dynamic_fr_info = (CONSTANT_Fieldref_info*) malloc(sizeof(CONSTANT_Fieldref_info));

            *dynamic_fr_info = fr_info;

            r = (cp_info*) dynamic_fr_info;

            break;
        case 10: // CONSTANT_Methodref
            u2 _class_index = readu2(bytes);
            u2 _name_and_type_index = readu2(bytes);
            CONSTANT_Methodref_info mr_info = { tag, _class_index, _name_and_type_index };
            CONSTANT_Methodref_info* dynamic_mr_info = (CONSTANT_Methodref_info*) malloc(sizeof(CONSTANT_Methodref_info));

            *dynamic_mr_info = mr_info;

            r = (cp_info*) dynamic_mr_info;

            break;
        case 11: // CONSTANT_InterfaceMethodref
            u2 _class_index_ = readu2(bytes);
            u2 _name_and_type_index_ = readu2(bytes);
            CONSTANT_InterfaceMethodref_info imr_info = { tag, _class_index_, _name_and_type_index_ };
            CONSTANT_InterfaceMethodref_info* dynamic_imr_info = (CONSTANT_InterfaceMethodref_info*) malloc(sizeof(CONSTANT_InterfaceMethodref_info));

            *dynamic_imr_info = imr_info;

            r = (cp_info*) dynamic_imr_info;

            break;
        case 8: // CONSTANT_String
            u2 string_index = readu2(bytes);
            CONSTANT_String_info s_info = { tag, string_index };
            CONSTANT_String_info* dynamic_s_info = (CONSTANT_String_info*) malloc(sizeof(CONSTANT_String_info));

            *dynamic_s_info = s_info;

            r = (cp_info*) dynamic_s_info;

            break;
        case 3: // CONSTANT_Integer
            u4 bytes_ = readu4(bytes);
            CONSTANT_Integer_info i_info = { tag, bytes_ };
            CONSTANT_Integer_info* dynamic_i_info = (CONSTANT_Integer_info*) malloc(sizeof(CONSTANT_Integer_info));

            *dynamic_i_info = i_info;

            r = (cp_info*) dynamic_i_info;

            break;
        case 4: // CONSTANT_Float
            u4 _bytes_ = readu4(bytes);
            CONSTANT_Float_info f_info = { tag, _bytes_ };
            CONSTANT_Float_info* dynamic_f_info = (CONSTANT_Float_info*) malloc(sizeof(CONSTANT_Float_info));

            *dynamic_f_info = f_info;

            r = (cp_info*) dynamic_f_info;

            break;
        case 5: // CONSTANT_Long
            u4 high_bytes = readu4(bytes);
            u4 low_bytes = readu4(bytes);
            CONSTANT_Long_info l_info = { tag, high_bytes, low_bytes };
            CONSTANT_Long_info* dynamic_l_info = (CONSTANT_Long_info*) malloc(sizeof(CONSTANT_Long_info));

            *dynamic_l_info = l_info;

            r = (cp_info*) dynamic_l_info;

            break;
        case 6: // CONSTANT_Double
            u4 _high_bytes = readu4(bytes);
            u4 _low_bytes = readu4(bytes);
            CONSTANT_Double_info d_info = { tag, _high_bytes, _low_bytes };
            CONSTANT_Double_info* dynamic_d_info = (CONSTANT_Double_info*) malloc(sizeof(CONSTANT_Double_info));

            *dynamic_d_info = d_info;

            r = (cp_info*) dynamic_d_info;

            break;
        case 12: // CONSTANT_NameAndType
            u2 _name_index = readu2(bytes);
            u2 descriptor_index = readu2(bytes);
            CONSTANT_NameAndType_info nat_info = { tag, _name_index, descriptor_index };
            CONSTANT_NameAndType_info* dynamic_nat_info = (CONSTANT_NameAndType_info*) malloc(sizeof(CONSTANT_NameAndType_info));

            *dynamic_nat_info = nat_info;

            r = (cp_info*) dynamic_nat_info;

            break;
        case 1: // CONSTANT_Utf8
            u2 length = readu2(bytes);
            u1* _bytes = readu1array(length, bytes);
            CONSTANT_Utf8_info u8_info = { tag, length, _bytes };
            CONSTANT_Utf8_info* dynamic_u8_info = (CONSTANT_Utf8_info*) malloc(sizeof(CONSTANT_Utf8_info));

            *dynamic_u8_info = u8_info;

            r = (cp_info*) dynamic_u8_info;

            break;
        case 15: // CONSTANT_MethodHandle
            u1 reference_kind = readu1(bytes);
            u2 reference_index = readu2(bytes);
            CONSTANT_MethodHandle_info mh_info = { tag, reference_kind, reference_index };
            CONSTANT_MethodHandle_info* dynamic_mh_info = (CONSTANT_MethodHandle_info*) malloc(sizeof(CONSTANT_MethodHandle_info));

            *dynamic_mh_info = mh_info;

            r = (cp_info*) dynamic_mh_info;

            break;
        case 16: // CONSTANT_MethodType
            u2 _descriptor_index = readu2(bytes);
            CONSTANT_MethodType_info mt_info = { tag, _descriptor_index };
            CONSTANT_MethodType_info* dynamic_mt_info = (CONSTANT_MethodType_info*) malloc(sizeof(CONSTANT_MethodType_info));

            *dynamic_mt_info = mt_info;

            r = (cp_info*) dynamic_mt_info;

            break;
        case 18: // CONSTANT_InvokeDynamic
            u2 bootstrap_method_attr_index = readu2(bytes);
            u2 _name_and_type_index__ = readu2(bytes);
            CONSTANT_InvokeDynamic_info id_info = { tag, bootstrap_method_attr_index, _name_and_type_index__ };
            CONSTANT_InvokeDynamic_info* dynamic_id_info = (CONSTANT_InvokeDynamic_info*) malloc(sizeof(CONSTANT_InvokeDynamic_info));

            *dynamic_id_info = id_info;

            r = (cp_info*) dynamic_id_info;

            break;
        default:
            printf("Unknown constant found in constant pool %d\n", byteOf(tag));
            exit(0);
    }

    return r;
}

cp_info** readConstantPool(u2 count, Bytes* bytes) {
    if (shortOf(count) == 0) {
        return (cp_info**) malloc(0 * sizeof(cp_info*));
    }

    ushort len = shortOf(count) - 1;

    cp_info** r = (cp_info**) malloc(shortOf(count) * sizeof(cp_info*));

    for (int i = 0; i < len; i++) {
        r[i + 1] = readConstantPoolItem(bytes);
    }

    r[0] = NULL;

    return r;
}

String readUTFString(CONSTANT_Utf8_info from) {
    ushort len = shortOf(from.length);
    char* content = (char*) malloc((len + 1) * sizeof(char));

    for (int i = 0; i < len; i++) {
        content[i] = (char) from.bytes[i].byte;
    }

    content[len] = 0;

    return stringOf(content, len);
}

char* readUTF(CONSTANT_Utf8_info from) {
    ushort len = shortOf(from.length);
    char* content = (char*) malloc((len + 1) * sizeof(char));

    for (int i = 0; i < len; i++) {
        content[i] = (char) from.bytes[i].byte;
    }

    content[len] = 0;

    return content;
}

verification_type_info* readVerificationTypeInfo(Bytes* bytes) {
    u1 tag = readu1(bytes);
    verification_type_info* r;

    switch (byteOf(tag)) {
        case 0: // Top_variable_info
            Top_variable_info tv_info = { tag };
            Top_variable_info* dynamic_tv_info = (Top_variable_info*) malloc(sizeof(Top_variable_info));

            *dynamic_tv_info = tv_info;

            r = (verification_type_info*) dynamic_tv_info;
            break;
        case 1: // Integer_variable_info
            Integer_variable_info iv_info = { tag };
            Integer_variable_info* dynamic_iv_info = (Integer_variable_info*) malloc(sizeof(Integer_variable_info));

            *dynamic_iv_info = iv_info;

            r = (verification_type_info*) dynamic_iv_info;
            break;
        case 2: // Float_variable_info
            Float_variable_info fv_info = { tag };
            Float_variable_info* dynamic_fv_info = (Float_variable_info*) malloc(sizeof(Float_variable_info));

            *dynamic_fv_info = fv_info;

            r = (verification_type_info*) dynamic_fv_info;
            break;
        case 3: // Double_variable_info
            Double_variable_info dv_info = { tag };
            Double_variable_info* dynamic_dv_info = (Double_variable_info*) malloc(sizeof(Double_variable_info));

            *dynamic_dv_info = dv_info;

            r = (verification_type_info*) dynamic_dv_info;
            break;
        case 4: // Long_variable_info
            Long_variable_info lv_info = { tag };
            Long_variable_info* dynamic_lv_info = (Long_variable_info*) malloc(sizeof(Long_variable_info));

            *dynamic_lv_info = lv_info;

            r = (verification_type_info*) dynamic_lv_info;
            break;
        case 5: // Null_variable_info
            Null_variable_info nv_info = { tag };
            Null_variable_info* dynamic_nv_info = (Null_variable_info*) malloc(sizeof(Null_variable_info));

            *dynamic_nv_info = nv_info;

            r = (verification_type_info*) dynamic_nv_info;
            break;
        case 6: // UninitalizedThis_variable_info
            UninitializedThis_variable_info utv_info = { tag };
            UninitializedThis_variable_info* dynamic_utv_info = (UninitializedThis_variable_info*) malloc(sizeof(UninitializedThis_variable_info));

            *dynamic_utv_info = utv_info;

            r = (verification_type_info*) dynamic_utv_info;
            break;
        case 7: // Object_variable_info
            u2 cpool_index = readu2(bytes);
            Object_variable_info ov_info = { tag, cpool_index };
            Object_variable_info* dynamic_ov_info = (Object_variable_info*) malloc(sizeof(Object_variable_info));

            *dynamic_ov_info = ov_info;

            r = (verification_type_info*) dynamic_ov_info;
            break;
        case 8: // Uninitialized_variable_info
            u2 offset = readu2(bytes);
            Uninitialized_variable_info uv_info = { tag, offset };
            Uninitialized_variable_info* dynamic_uv_info = (Uninitialized_variable_info*) malloc(sizeof(Uninitialized_variable_info));

            *dynamic_uv_info = uv_info;

            r = (verification_type_info*) dynamic_uv_info;
            break;
        default:
            printf("Unknown verification_type_info found %d\n", byteOf(tag));
            exit(0);
            // ???
    }

    return r;
}

element_value* readElementValue(Bytes* bytes) {
    u1 tag = readu1(bytes);
    element_value* r;

    switch (byteOf(tag)) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 's': // const_value_index
            u2 constant_value_index = readu2(bytes);
            element_value_const_value_index element = { tag, constant_value_index };
            element_value_const_value_index* dynamic_element = (element_value_const_value_index*) malloc(sizeof(element_value_const_value_index));

            *dynamic_element = element;

            r = (element_value*) dynamic_element;

            break;
        case 'e': // enum_const_value
            u2 type_name_index = readu2(bytes);
            u2 const_name_index = readu2(bytes);
            element_value_enum_const_value element_ = { tag, type_name_index, const_name_index };
            element_value_enum_const_value* dynamic_element_ = (element_value_enum_const_value*) malloc(sizeof(element_value_enum_const_value));

            *dynamic_element_ = element_;

            r = (element_value*) dynamic_element_;

            break;
        case 'c': // class_info_index
            u2 class_info_index = readu2(bytes);
            element_value_class_info_index element__ = { tag, class_info_index };
            element_value_class_info_index* dynamic_element__ = (element_value_class_info_index*) malloc(sizeof(element_value_class_info_index));

            *dynamic_element__ = element__;

            r = (element_value*) dynamic_element__;

            break;
        case '@': // annotation_value
            annotation annotation_value = readAnnotation(bytes);
            element_value_annotation_value element___ = { tag, annotation_value };
            element_value_annotation_value* dynamic_element___ = (element_value_annotation_value*) malloc(sizeof(element_value_annotation_value));

            *dynamic_element___ = element___;

            r = (element_value*) dynamic_element___;

            break;
        case '[': // array_value
            u2 num_value = readu2(bytes);
            element_value** values = (element_value**) malloc(shortOf(num_value) * sizeof(element_value*));

            for (int i = 0; i < shortOf(num_value); i++) {
                values[i] = readElementValue(bytes);
            }

            element_value_array_value element____ = { tag, num_value, values };
            element_value_array_value* dynamic_element____ = (element_value_array_value*) malloc(sizeof(element_value_array_value));

            *dynamic_element____ = element____;

            r = (element_value*) dynamic_element____;

            break;
        default:
            printf("Element value has invalid tag %c\n", byteOf(tag));
            exit(0);
    }

    return r;
}

annotation readAnnotation(Bytes* bytes) {
    u2 type_index = readu2(bytes);
    u2 num_element_value_pairs = readu2(bytes);
    element_value_pairs_item* element_value_pairs = (element_value_pairs_item*) malloc(shortOf(num_element_value_pairs) * sizeof(element_value_pairs_item));

    for (int i = 0; i < shortOf(num_element_value_pairs); i++) {
        u2 element_name_index = readu2(bytes);
        element_value* value = readElementValue(bytes);
        element_value_pairs_item element_value_item = { element_name_index, value };

        element_value_pairs[i] = element_value_item;
    }

    annotation ann = { type_index, num_element_value_pairs, element_value_pairs };

    return ann;
}

attribute_info* readAttribute(cp_info** constant_pool, Bytes* bytes) {
    u2 attribute_name_index = readu2(bytes);
    u4 attribute_length = readu4(bytes);
    cp_info* cp = constant_pool[shortOf(attribute_name_index)];

    if (byteOf(cp->tag) != 1) { // CONSTANT_Utf8
        printf("Attribute refers to invalid constant pool info %d\n", byteOf(cp->tag));
        exit(0);
    }

    char* name = readUTF(*(CONSTANT_Utf8_info*) cp);

    attribute_info* r;

    if (!strcmp(name, "ConstantValue")) {
        u2 constantvalue_index = readu2(bytes);
        ConstantValue_attribute cv_attr = { attribute_name_index, attribute_length, constantvalue_index };
        ConstantValue_attribute* dynamic_cv_attr = (ConstantValue_attribute*) malloc(sizeof(ConstantValue_attribute));

        *dynamic_cv_attr = cv_attr;

        r = (attribute_info*) dynamic_cv_attr;
    } else if (!strcmp(name, "Code")) {
        u2 max_stack = readu2(bytes);
        u2 max_locals = readu2(bytes);
        u4 code_length = readu4(bytes);
        u1* code = readu1array_(intOf(code_length), bytes);
        u2 exception_table_length = readu2(bytes);
        exception_table_inner* exception_table = (exception_table_inner*) malloc(shortOf(exception_table_length) * sizeof(exception_table_inner));

        for (int i = 0; i < shortOf(exception_table_length); i++) {
            u2 start_pc = readu2(bytes);
            u2 end_pc = readu2(bytes);
            u2 handler_pc = readu2(bytes);
            u2 catch_type = readu2(bytes);
            exception_table_inner exception_table_inner = { start_pc, end_pc, handler_pc, catch_type };

            exception_table[i] = exception_table_inner;
        }

        u2 attributes_count = readu2(bytes);
        attribute_info** attributes = readAttributes(constant_pool, attributes_count, bytes);
        Code_attribute c_attr = { attribute_name_index, attribute_length, max_stack, max_locals, code_length, code, exception_table_length, exception_table, attributes_count, attributes };
        Code_attribute* dynamic_c_attr = (Code_attribute*) malloc(sizeof(Code_attribute));

        *dynamic_c_attr = c_attr;

        r = (attribute_info*) dynamic_c_attr;
    } else if (!strcmp(name, "StackMapTable")) {
        u2 number_of_entries = readu2(bytes);
        stack_map_frame** entries = (stack_map_frame**) malloc(shortOf(number_of_entries) * sizeof(stack_map_frame*));

        for (int i = 0; i < shortOf(number_of_entries); i++) {
            u1 frame_type = readu1(bytes);
            ushort byte_frame = byteOf(frame_type);
            stack_map_frame* entry;

            if (byte_frame <= 63) {
                same_frame* dynamic_frame = (same_frame*) malloc(sizeof(same_frame));
                same_frame frame = { frame_type };

                *dynamic_frame = frame;

                entry = (stack_map_frame*) dynamic_frame;
            } else if (byte_frame <= 127) {
                verification_type_info* stack = readVerificationTypeInfo(bytes);
                same_locals_1_stack_item_frame frame_ = { frame_type, stack };
                same_locals_1_stack_item_frame* dynamic_frame_ = (same_locals_1_stack_item_frame*) malloc(sizeof(same_locals_1_stack_item_frame));

                *dynamic_frame_ = frame_;

                entry = (stack_map_frame*) dynamic_frame_;
            } else if (byte_frame < 247 | byte_frame > 255) {
                // ???
                printf("Unknown byte_frame found %d\n", byte_frame);
                exit(0);
            } else if (byte_frame == 247) {
                u2 offset_delta = readu2(bytes);
                verification_type_info* stack_ = readVerificationTypeInfo(bytes);
                same_locals_1_stack_item_frame_extended frame__ = { frame_type, offset_delta, stack_ };
                same_locals_1_stack_item_frame_extended* dynamic_frame__ = (same_locals_1_stack_item_frame_extended*) malloc(sizeof(same_locals_1_stack_item_frame_extended));

                *dynamic_frame__ = frame__;

                entry = (stack_map_frame*) dynamic_frame__;
            } else if (byte_frame <= 250) {
                u2 offset_delta_ = readu2(bytes);
                chop_frame frame___ = { frame_type, offset_delta_ };
                chop_frame* dynamic_frame___ = (chop_frame*) malloc(sizeof(chop_frame));

                *dynamic_frame___ = frame___;

                entry = (stack_map_frame*) dynamic_frame___;
            } else if (byte_frame == 251) {
                u2 offset_delta__ = readu2(bytes);
                same_frame_extended frame____ = { frame_type, offset_delta__ };
                same_frame_extended* dynamic_frame____ = (same_frame_extended*) malloc(sizeof(same_frame_extended));

                *dynamic_frame____ = frame____;

                entry = (stack_map_frame*) dynamic_frame____;
            } else if (byte_frame <= 254) {
                u2 offset_delta___ = readu2(bytes);
                verification_type_info** locals = (verification_type_info**) malloc((byteOf(frame_type) - 251) * sizeof(verification_type_info*));

                for (int y = 0; y < byteOf(frame_type) - 251; y++) {
                    locals[y] = readVerificationTypeInfo(bytes);
                }

                append_frame frame_____ = { frame_type, offset_delta___, locals };
                append_frame* dynamic_frame_____ = (append_frame*) malloc(sizeof(append_frame));

                *dynamic_frame_____ = frame_____;

                entry = (stack_map_frame*) dynamic_frame_____;
            } else { // (byte_frame == 255)
                u2 offset_delta____ = readu2(bytes);
                u2 number_of_locals = readu2(bytes);
                verification_type_info** locals_ = (verification_type_info**) malloc((byteOf(frame_type) - 251) * sizeof(verification_type_info*));

                for (int x = 0; x < shortOf(number_of_locals); x++) {
                    locals_[x] = readVerificationTypeInfo(bytes);
                }

                u2 number_of_stack_items = readu2(bytes);
                verification_type_info** stack = (verification_type_info**) malloc((byteOf(frame_type) - 251) * sizeof(verification_type_info*));

                for (int n = 0; n < shortOf(number_of_stack_items); n++) {
                    stack[n] = readVerificationTypeInfo(bytes);
                }

                full_frame frame______ = { frame_type, offset_delta____, number_of_locals, locals_, number_of_stack_items, stack };
                full_frame* dynamic_frame______ = (full_frame*) malloc(sizeof(full_frame));

                *dynamic_frame______ = frame______;

                entry = (stack_map_frame*) dynamic_frame______;
            }

            entries[i] = entry;
        }

        StackMapTable_attribute smt_attr = { attribute_name_index, attribute_length, number_of_entries, entries };
        StackMapTable_attribute* dynamic_smt_attr = (StackMapTable_attribute*) malloc(sizeof(StackMapTable_attribute));

        *dynamic_smt_attr = smt_attr;

        r = (attribute_info*) dynamic_smt_attr;
    } else if (!strcmp(name, "Exceptions")) {
        u2 number_of_exceptions = readu2(bytes);
        u2* exception_index_table = readu2array(number_of_exceptions, bytes);
        Exceptions_attribute e_attr = { attribute_name_index, attribute_length, number_of_exceptions, exception_index_table };
        Exceptions_attribute* dynamic_e_attr = (Exceptions_attribute*) malloc(sizeof(Exceptions_attribute));

        *dynamic_e_attr = e_attr;

        r = (attribute_info*) dynamic_e_attr;
    } else if (!strcmp(name, "InnerClasses")) {
        u2 number_of_classes = readu2(bytes);
        InnerClasses_attribute_inner* classes = (InnerClasses_attribute_inner*) malloc(shortOf(number_of_classes) * sizeof(InnerClasses_attribute_inner));

        for (int i = 0; i < shortOf(number_of_classes); i++) {
            u2 inner_class_info_index = readu2(bytes);
            u2 outer_class_info_index = readu2(bytes);
            u2 inner_name_index = readu2(bytes);
            u2 inner_class_access_flags = readu2(bytes);
            InnerClasses_attribute_inner class = { inner_class_info_index, outer_class_info_index, inner_name_index, inner_class_access_flags };

            classes[i] = class;
        }

        InnerClasses_attribute ic_attr = { attribute_name_index, attribute_length, number_of_classes, classes };
        InnerClasses_attribute* dynamic_ic_attr = (InnerClasses_attribute*) malloc(sizeof(InnerClasses_attribute));

        *dynamic_ic_attr = ic_attr;

        r = (attribute_info*) dynamic_ic_attr;
    } else if (!strcmp(name, "EnclosingMethod")) {
        u2 class_index = readu2(bytes);
        u2 method_index = readu2(bytes);
        EnclosingMethod_attribute em_attr = { attribute_name_index, attribute_length, class_index, method_index };
        EnclosingMethod_attribute* dynamic_em_attr = (EnclosingMethod_attribute*) malloc(sizeof(EnclosingMethod_attribute));

        *dynamic_em_attr = em_attr;

        r = (attribute_info*) dynamic_em_attr;
    } else if (!strcmp(name, "Synthetic")) {
        Synthetic_attribute s_attr = { attribute_name_index, attribute_length };
        Synthetic_attribute* dynamic_s_attr = (Synthetic_attribute*) malloc(sizeof(Synthetic_attribute));

        *dynamic_s_attr = s_attr;

        r = (attribute_info*) dynamic_s_attr;
    } else if (!strcmp(name, "Signature")) {
        u2 signature_index = readu2(bytes);
        Signature_attribute sn_attr = { attribute_name_index, attribute_length, signature_index };
        Signature_attribute* dynamic_sn_attr = (Signature_attribute*) malloc(sizeof(Signature_attribute));

        *dynamic_sn_attr = sn_attr;

        r = (attribute_info*) dynamic_sn_attr;
    } else if (!strcmp(name, "SourceFile")) {
        u2 sourcefile_index = readu2(bytes);
        SourceFile_attribute sf_attr = { attribute_name_index, attribute_length, sourcefile_index };
        SourceFile_attribute* dynamic_sf_attr = (SourceFile_attribute*) malloc(sizeof(SourceFile_attribute));

        *dynamic_sf_attr = sf_attr;

        r = (attribute_info*) dynamic_sf_attr;
    } else if (!strcmp(name, "SourceDebugExtension")) {
        u1* debug_extension = readu1array_(intOf(attribute_length), bytes);
        SourceDebugExtension_attribute sde_attr = { attribute_name_index, attribute_length, debug_extension };
        SourceDebugExtension_attribute* dynamic_sde_attr = (SourceDebugExtension_attribute*) malloc(sizeof(SourceDebugExtension_attribute));

        *dynamic_sde_attr = sde_attr;

        r = (attribute_info*) dynamic_sde_attr;
    } else if (!strcmp(name, "LineNumberTable")) {
        u2 line_number_table_length = readu2(bytes);
        line_number_table_element* line_number_table = (line_number_table_element*) malloc(shortOf(line_number_table_length) * sizeof(line_number_table_element));

        for (int i = 0; i < shortOf(line_number_table_length); i++) {
            u2 start_pc = readu2(bytes);
            u2 line_number = readu2(bytes);
            line_number_table_element element = { start_pc, line_number };

            line_number_table[i] = element;
        }

        LineNumberTable_attribute lnt_attr = { attribute_name_index, attribute_length, line_number_table_length, line_number_table };
        LineNumberTable_attribute* dynamic_lnt_attr = (LineNumberTable_attribute*) malloc(sizeof(LineNumberTable_attribute));

        *dynamic_lnt_attr = lnt_attr;

        r = (attribute_info*) dynamic_lnt_attr;
    } else if (!strcmp(name, "LocalVariableTable")) {
        u2 local_variable_table_length = readu2(bytes);
        local_variable_table_element* local_variable_table = (local_variable_table_element*) malloc(shortOf(local_variable_table_length) * sizeof(local_variable_table_element));

        for (int i = 0; i < shortOf(local_variable_table_length); i++) {
            u2 start_pc = readu2(bytes);
            u2 length = readu2(bytes);
            u2 name_index = readu2(bytes);
            u2 descriptor_index = readu2(bytes);
            u2 index = readu2(bytes);
            local_variable_table_element element = { start_pc, length, name_index, descriptor_index, index };

            local_variable_table[i] = element;
        }

        LocalVariableTable_attribute lvt_attr = { attribute_name_index, attribute_length, local_variable_table_length, local_variable_table };
        LocalVariableTable_attribute* dynamic_lvt_attr = (LocalVariableTable_attribute*) malloc(sizeof(LocalVariableTable_attribute));

        *dynamic_lvt_attr = lvt_attr;

        r = (attribute_info*) dynamic_lvt_attr;
    } else if (!strcmp(name, "LocalVariableTypeTable")) {
        u2 local_variable_type_table_length = readu2(bytes);
        local_variable_type_table_element* local_variable_type_table = (local_variable_type_table_element*) malloc(shortOf(local_variable_type_table_length) * sizeof(local_variable_type_table_element));

        for (int i = 0; i < shortOf(local_variable_type_table_length); i++) {
            u2 start_pc = readu2(bytes);
            u2 length = readu2(bytes);
            u2 name_index = readu2(bytes);
            u2 signature_index = readu2(bytes);
            u2 index = readu2(bytes);
            local_variable_type_table_element element = { start_pc, length, name_index, signature_index, index };

            local_variable_type_table[i] = element;
        }

        LocalVariableTypeTable_attribute lvtt_attr = { attribute_name_index, attribute_length, local_variable_type_table_length };
        LocalVariableTypeTable_attribute* dynamic_lvtt_attr = (LocalVariableTypeTable_attribute*) malloc(sizeof(LocalVariableTypeTable_attribute));

        *dynamic_lvtt_attr = lvtt_attr;

        r = (attribute_info*) dynamic_lvtt_attr;
    } else if (!strcmp(name, "Deprecated")) {
        Deprecated_attribute d_attr = { attribute_name_index, attribute_length };
        Deprecated_attribute* dynamic_d_attr = (Deprecated_attribute*) malloc(sizeof(Deprecated_attribute));

        *dynamic_d_attr = d_attr;

        r = (attribute_info*) dynamic_d_attr;
    } else if (!strcmp(name, "RuntimeVisibleAnnotations")) {
        u2 num_annotations = readu2(bytes);
        annotation* annotations = (annotation*) malloc(shortOf(num_annotations) * sizeof(annotation));

        for (int i = 0; i < shortOf(num_annotations); i++) {
            annotations[i] = readAnnotation(bytes);
        }

        RuntimeVisibleAnnotations_attribute rva_attr = { attribute_name_index, attribute_length, num_annotations, annotations };
        RuntimeVisibleAnnotations_attribute* dynamic_rva_attr = (RuntimeVisibleAnnotations_attribute*) malloc(sizeof(RuntimeVisibleAnnotations_attribute));

        *dynamic_rva_attr = rva_attr;

        r = (attribute_info*) dynamic_rva_attr;
    } else if (!strcmp(name, "RuntimeInvisibleAnnotations")) {
        u2 num_annotations = readu2(bytes);
        annotation* annotations = (annotation*) malloc(shortOf(num_annotations) * sizeof(annotation));

        for (int i = 0; i < shortOf(num_annotations); i++) {
            annotations[i] = readAnnotation(bytes);
        }

        RuntimeInvisibleAnnotations_attribute ria_attr = { attribute_name_index, attribute_length, num_annotations, annotations };
        RuntimeInvisibleAnnotations_attribute* dynamic_ria_attr = (RuntimeInvisibleAnnotations_attribute*) malloc(sizeof(RuntimeInvisibleAnnotations_attribute));

        *dynamic_ria_attr = ria_attr;

        r = (attribute_info*) dynamic_ria_attr;
    } else if (!strcmp(name, "RuntimeVisibleParameterAnnotations")) {
        u1 num_parameters = readu1(bytes);
        parameter_annotations_element* parameter_annotations = (parameter_annotations_element*) malloc(byteOf(num_parameters) * sizeof(parameter_annotations_element));

        for (int i = 0; i < byteOf(num_parameters); i++) {
            u2 num_annotations = readu2(bytes);
            annotation* annotations = (annotation*) malloc(shortOf(num_annotations) * sizeof(annotation));

            for (int y = 0; y < shortOf(num_annotations); y++) {
                annotations[y] = readAnnotation(bytes);
            }

            parameter_annotations_element element = { num_annotations, annotations };

            parameter_annotations[i] = element;
        }

        RuntimeVisibleParameterAnnotations_attribute rvpa_attr = { attribute_name_index, attribute_length, num_parameters, parameter_annotations };
        RuntimeVisibleParameterAnnotations_attribute* dynamic_rvpa_attr = (RuntimeVisibleParameterAnnotations_attribute*) malloc(sizeof(RuntimeVisibleParameterAnnotations_attribute));

        *dynamic_rvpa_attr = rvpa_attr;

        r = (attribute_info*) dynamic_rvpa_attr;
    } else if (!strcmp(name, "RuntimeInvisibleParameterAnnotations")) {
        u1 num_parameters = readu1(bytes);
        parameter_annotations_element* parameter_annotations = (parameter_annotations_element*) malloc(byteOf(num_parameters) * sizeof(parameter_annotations_element));

        for (int i = 0; i < byteOf(num_parameters); i++) {
            u2 num_annotations = readu2(bytes);
            annotation* annotations = (annotation*) malloc(shortOf(num_annotations) * sizeof(annotation));

            for (int y = 0; y < shortOf(num_annotations); y++) {
                annotations[y] = readAnnotation(bytes);
            }

            parameter_annotations_element element = { num_annotations, annotations };

            parameter_annotations[i] = element;
        }

        RuntimeInvisibleParameterAnnotations_attribute ripa_attr = { attribute_name_index, attribute_length, num_parameters, parameter_annotations };
        RuntimeInvisibleParameterAnnotations_attribute* dynamic_ripa_attr = (RuntimeInvisibleParameterAnnotations_attribute*) malloc(sizeof(RuntimeInvisibleParameterAnnotations_attribute));

        *dynamic_ripa_attr = ripa_attr;

        r = (attribute_info*) dynamic_ripa_attr;
    } else if (!strcmp(name, "AnnotationDefault")) {
        element_value* default_value = readElementValue(bytes);
        AnnotationDefault_attribute ad_attr = { attribute_name_index, attribute_length, default_value };
        AnnotationDefault_attribute* dynamic_ad_attr = (AnnotationDefault_attribute*) malloc(sizeof(AnnotationDefault_attribute));

        *dynamic_ad_attr = ad_attr;

        r = (attribute_info*) dynamic_ad_attr;
    } else if (!strcmp(name, "BootstrapMethods")) {
        u2 num_bootstrap_methods = readu2(bytes);
        bootstrap_method* bootstrap_methods = (bootstrap_method*) malloc(shortOf(num_bootstrap_methods) * sizeof(bootstrap_method));

        for (int i = 0; i < shortOf(num_bootstrap_methods); i++) {
            u2 bootstrap_method_ref = readu2(bytes);
            u2 num_bootstrap_arguments = readu2(bytes);
            u2* bootstrap_arguments = readu2array(num_bootstrap_arguments, bytes);
            bootstrap_method method = { bootstrap_method_ref, num_bootstrap_arguments,bootstrap_arguments };

            bootstrap_methods[i] = method;
        }

        BootstrapMethods_attribute bm_attr = { attribute_name_index, attribute_length, num_bootstrap_methods, bootstrap_methods };
        BootstrapMethods_attribute* dynamic_bm_attr = (BootstrapMethods_attribute*) malloc(sizeof(BootstrapMethods_attribute));

        *dynamic_bm_attr = bm_attr;

        r = (attribute_info*) dynamic_bm_attr;
    } else {
        u1* info = (u1*) malloc(intOf(attribute_length) * sizeof(u1));

        for (int i = 0; i < intOf(attribute_length); i++) {
            info[i] = readu1(bytes);
        }

        attribute_info attr_attr = { attribute_name_index, attribute_length, info };
        attribute_info* dynamic_attr_attr = (attribute_info*) malloc(sizeof(attribute_info));

        *dynamic_attr_attr = attr_attr;

        r = (attribute_info*) dynamic_attr_attr;
    }

    return r;
}

attribute_info** readAttributes(cp_info** constant_pool, u2 count, Bytes* bytes) {
    ushort len = shortOf(count);
    attribute_info** r = (attribute_info**) malloc(len * sizeof(attribute_info*));

    for (int i = 0; i < len; i++) {
        r[i] = readAttribute(constant_pool, bytes);
    }

    return r;
}

field_info readField(cp_info** constant_pool, Bytes* bytes) {
    u2 access_flags = readu2(bytes);
    u2 name_index = readu2(bytes);
    u2 descriptor_index = readu2(bytes);
    u2 attributes_count = readu2(bytes);
    attribute_info** attributes = readAttributes(constant_pool, attributes_count, bytes);
    field_info field = { access_flags, name_index, descriptor_index, attributes_count, attributes };

    return field;
}

field_info* readFields(cp_info** constant_pool, u2 count, Bytes* bytes) {
    ushort len = shortOf(count);
    field_info* r = (field_info*) malloc(len * sizeof(field_info));

    for (int i = 0; i < len; i++) {
        r[i] = readField(constant_pool, bytes);
    }

    return r;
}

method_info readMethod(cp_info** constant_pool, Bytes* bytes) {
    u2 access_flags = readu2(bytes);
    u2 name_index = readu2(bytes);
    u2 descriptor_index = readu2(bytes);
    u2 attributes_count = readu2(bytes);
    attribute_info** attributes = readAttributes(constant_pool, attributes_count, bytes);
    method_info method = { access_flags, name_index, descriptor_index, attributes_count, attributes };

    return method;
}

method_info* readMethods(cp_info** constant_pool, u2 count, Bytes* bytes) {
    ushort len = shortOf(count);
    method_info* r = (method_info*) malloc(len * sizeof(method_info));

    for (int i = 0; i < len; i++) {
        r[i] = readMethod(constant_pool, bytes);
    }

    return r;
}

ClassFile readClassFile(Bytes bytes) {
    u4 magic = readu4(&bytes);
    u2 minor_version = readu2(&bytes);
    u2 major_version = readu2(&bytes);
    u2 constant_pool_count = readu2(&bytes);
    cp_info** constant_pool = readConstantPool(constant_pool_count, &bytes);
    u2 access_flags = readu2(&bytes);
    u2 this_class = readu2(&bytes);
    u2 super_class = readu2(&bytes);
    u2 interfaces_count = readu2(&bytes);
    u2* interfaces = readu2array(interfaces_count, &bytes);
    u2 fields_count = readu2(&bytes);
    field_info* fields = readFields(constant_pool, fields_count, &bytes);
    u2 methods_count = readu2(&bytes);
    method_info* methods = readMethods(constant_pool, methods_count, &bytes);
    u2 attributes_count = readu2(&bytes);
    attribute_info** attributes = readAttributes(constant_pool, attributes_count, &bytes);
    ClassFile classFile = {
        magic,
        minor_version,
        major_version,
        constant_pool_count,
        constant_pool,
        access_flags,
        this_class,
        super_class,
        interfaces_count,
        interfaces,
        fields_count,
        fields,
        methods_count,
        methods,
        attributes_count,
        attributes
    };

    return classFile;
}

String writeClassAccessFlags(ushort access_flags, boolean isClass) {
    String output = stringEmpty();

    if ((access_flags & 0x0001) != 0) { // public
        append(&output, stringOf("public ", 7));
    }

    if ((access_flags & 0x0008) != 0) { // static
        append(&output, stringOf("static ", 7));
    }

    if ((access_flags & 0x0100) != 0) { // native
        append(&output, stringOf("native ", 7));
    }

    if ((access_flags & 0x0010) != 0) { // final
        append(&output, stringOf("final ", 6));
    }

    if ((access_flags & 0x0020) != 0) { // super
//        append(&output, stringOf("super ", 6));
        // probably don't print this but I'm not sure
    }

    if ((access_flags & 0x0200) != 0) { // interface
        append(&output, stringOf("interface ", 10));
    } else if ((access_flags & 0x2000) != 0) { // annotation
        append(&output, stringOf("@interface ", 11));
    } else if ((access_flags & 0x4000) != 0) { // enum
        append(&output, stringOf("enum ", 5));
    } else if (isClass) { // class
        append(&output, stringOf("class ", 6));
    }

    if ((access_flags & 0x0400) != 0) { // abstract
        append(&output, stringOf("abstract ", 9));
    }

    if ((access_flags & 0x1000) != 0) { // synthetic
//        append(&output, stringOf("synthetic ", 10));
    }

    return output;
}

void appendUtf8(String* str, CONSTANT_Utf8_info utf8) {
    append(str, stringOf(readUTF(utf8), shortOf(utf8.length)));
}

CONSTANT_Utf8_info UTF8(cp_info** constant_pool, u2 index) {
    return *(CONSTANT_Utf8_info*) constant_pool[shortOf(index)];
}

String readJavaVersion(ushort majorVersion, ushort minorVersion) {
    String result = stringEmpty();

    switch (majorVersion) {
        case 45:
            if (minorVersion == 3) {
                append(&result, stringOf("1.1", 3));
            } else {
                append(&result, stringOf("1.0", 3));
            }

            break;
        case 46:
            append(&result, stringOf("1.2", 3));
            break;
        case 47:
            append(&result, stringOf("1.3", 3));
            break;
        case 48:
            append(&result, stringOf("1.4", 3));
            break;
        case 49:
            append(&result, stringOf("1.5", 3));
            break;
        case 50:
            append(&result, stringOf("1.6", 3));
            break;
        case 51:
            append(&result, stringOf("1.7", 3));
            break;
        case 52:
            append(&result, stringOf("1.8", 3));
            break;
        case 53:
            append(&result, stringOf("9", 1));
            break;
        case 54:
            append(&result, stringOf("10", 2));
            break;
        case 55:
            append(&result, stringOf("11", 2));
            break;
        case 56:
            append(&result, stringOf("12", 2));
            break;
        case 57:
            append(&result, stringOf("13", 2));
            break;
        case 58:
            append(&result, stringOf("14", 2));
            break;
        case 59:
            append(&result, stringOf("15", 2));
            break;
        case 60:
            append(&result, stringOf("16", 2));
            break;
        default:
            printf("16+ not available\n");
            exit(0);
    }

    return result;
}

String readSingleDescriptor(String descriptor, int* read) {
    int ptr = 0, arrayRead = 0;
    String desc = stringEmpty();

    switch (descriptor.content[ptr]) {
        case 'B':
            append(&desc, stringOf("byte", 4));
            break;
        case 'C':
            append(&desc, stringOf("char", 4));
            break;
        case 'D':
            append(&desc, stringOf("double", 6));
            break;
        case 'F':
            append(&desc, stringOf("float", 5));
            break;
        case 'I':
            append(&desc, stringOf("int", 3));
            break;
        case 'J':
            append(&desc, stringOf("long", 4));
            break;
        case 'V':
            append(&desc, stringOf("void", 4));
            break;
        case 'L':
            String cls = stringEmpty();
            char next;

            while ((next = descriptor.content[++ptr]) != ';') {
                appendChar(&cls, next);
            }

            append(&desc, stringOf(replaceString(content(cls), "/", "."), cls.length));
            break;
        case 'S':
            append(&desc, stringOf("short", 5));
            break;
        case 'Z':
            append(&desc, stringOf("boolean", 7));
            break;
        case '[':
            append(&desc, readSingleDescriptor(substring(descriptor, 1, descriptor.length), &arrayRead));
            append(&desc, stringOf("[]", 2));
            break;
    }

    *read = ptr + 1 + arrayRead;

    return desc;
}

Tuple readMethodDescriptor(String descriptor) {
    int ptr = 0;

    ptr++; // (

    String parameters = stringEmpty();

    while (charAt(descriptor, ptr) != ')') {
        int read = 0;

        String desc = readSingleDescriptor(substring(descriptor, ptr, descriptor.length), &read);

        append(&parameters, desc);
        append(&parameters, stringOf(", ", 2));

        ptr += read;
    }

    parameters = substring(parameters, 0, parameters.length - 3);

    int unused = 0; // I know why we need this and I don't like it
    String returnType = readSingleDescriptor(substring(descriptor, ptr + 1, descriptor.length), &unused);
    String* dynamicParameters = (String*) malloc(sizeof(String));
    String* dynamicReturnType = (String*) malloc(sizeof(String));

    *dynamicParameters = parameters;
    *dynamicReturnType = returnType;

    return tupleOf(dynamicParameters, dynamicReturnType);
}

attribute_info* findAttribute(char* find, ushort size, attribute_info** attributes, cp_info** constant_pool) {
    attribute_info* attribute = (attribute_info*) malloc(sizeof(attribute_info));

    for (int i = 0; i < size; i++) {
        attribute_info* cattribute = attributes[i];
        char* name = readUTF(*(CONSTANT_Utf8_info*) constant_pool[shortOf(cattribute->attribute_name_index)]);

        if (!strcmp(name, find)) {
            attribute = cattribute;

            break;
        }
    }

    return attribute;
}

String itos(int i) {
    if (i == 0) {
        return stringOf("0", 1);
    } else if (i < 0) {
        String r = stringEmpty();

        append(&r, stringOf("-", 1));
        append(&r, itos(abs(i)));

        return r;
    }

    int size = ((int) floor(log10(i))) + 1;
    char* str = (char*) malloc(size * sizeof(char));

    sprintf(str, "%d", i);

    return stringOf(str, size);
}

String readByteCode(u1* code, int* ptr) {
    return itos(byteOf(code[++*ptr]));
}

ushort readShortIn(u1* code, int* ptr) {
    u1 first = code[++*ptr];
    u1 second = code[++*ptr];

    return (byteOf(first) << 8) | byteOf(second);
}

String readShortCode(u1* code, int* ptr) {
    return itos(readShortIn(code, ptr));
}

String readIntCode(u1* code, int* ptr) {
    u1 first = code[++*ptr];
    u1 second = code[++*ptr];
    u1 third = code[++*ptr];
    u1 fourth = code[++*ptr];

    return itos((byteOf(first) << 24) | (byteOf(second) << 16) | (byteOf(third) << 8) | byteOf(fourth));
}

void readInvoke(CONSTANT_Methodref_info /* placeholder */ methodRef, cp_info** constant_pool, String* typeInfo) {
    CONSTANT_NameAndType_info nameAndType = *(CONSTANT_NameAndType_info*) constant_pool[shortOf(methodRef.name_and_type_index)];
    String className = readUTFString(*(CONSTANT_Utf8_info*) constant_pool[shortOf((*(CONSTANT_Class_info*) constant_pool[shortOf(methodRef.class_index)]).name_index)]);

    className.content = replaceString(className.content, "/", ".");

    append(typeInfo, className);
    append(typeInfo, stringOf(" => ", 4));

    String s = readUTFString(*(CONSTANT_Utf8_info*) constant_pool[shortOf(nameAndType.descriptor_index)]);
    Tuple desc = readMethodDescriptor(s);

    append(typeInfo, *(String*) desc.b);
    append(typeInfo, stringOf(" ", 1));
    appendUtf8(typeInfo, *(CONSTANT_Utf8_info*) constant_pool[shortOf(nameAndType.name_index)]);
    append(typeInfo, stringOf("(", 1));
    append(typeInfo, *(String*) desc.a);
    append(typeInfo, stringOf(")", 1));
}

String readCode(Code_attribute code, cp_info** constant_pool) {
    String result = stringEmpty();

    for (int i = 0; i < intOf(code.code_length); i++) {
        append(&result, stringOf("\t\t", 2));

        ubyte next = byteOf(code.code[i]);
        String typeInfo = stringEmpty();

        switch (next) {
            case 0x32:
                append(&result, stringOf("aaload", 6));
                break;
            case 0x53:
                append(&result, stringOf("aastore", 7));
                break;
            case 0x1:
                append(&result, stringOf("aconst_null", 11));
                break;
            case 0x19:
                append(&result, stringOf("aload", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x2a:
                append(&result, stringOf("aload_0", 7));
                break;
            case 0x2b:
                append(&result, stringOf("aload_1", 7));
                break;
            case 0x2c:
                append(&result, stringOf("aload_2", 7));
                break;
            case 0x2d:
                append(&result, stringOf("aload_3", 7));
                break;
            case 0xbd:
                append(&result, stringOf("anewarray", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xb0:
                append(&result, stringOf("areturn", 7));
                break;
            case 0xbe:
                append(&result, stringOf("arraylength", 11));
                break;
            case 0x3a:
                append(&result, stringOf("astore", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x4b:
                append(&result, stringOf("astore_0", 8));
                break;
            case 0x4c:
                append(&result, stringOf("astore_1", 8));
                break;
            case 0x4d:
                append(&result, stringOf("astore_2", 8));
                break;
            case 0x4e:
                append(&result, stringOf("astore_3", 8));
                break;
            case 0xbf:
                append(&result, stringOf("athrow", 6));
                break;
            case 0x33:
                append(&result, stringOf("baload", 6));
                break;
            case 0x54:
                append(&result, stringOf("bastore", 7));
                break;
            case 0x10:
                append(&result, stringOf("bipush", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x34:
                append(&result, stringOf("caload", 6));
                break;
            case 0x55:
                append(&result, stringOf("castore", 7));
                break;
            case 0xc0:
                append(&result, stringOf("checkcast", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x90:
                append(&result, stringOf("d2f", 3));
                break;
            case 0x8e:
                append(&result, stringOf("d2i", 3));
                break;
            case 0x8f:
                append(&result, stringOf("d2l", 3));
                break;
            case 0x63:
                append(&result, stringOf("dadd", 4));
                break;
            case 0x31:
                append(&result, stringOf("daload", 6));
                break;
            case 0x52:
                append(&result, stringOf("dastore", 7));
                break;
            case 0x98:
                append(&result, stringOf("dcmpg", 5));
                break;
            case 0x97:
                append(&result, stringOf("dcmpl", 5));
                break;
            case 0xe:
                append(&result, stringOf("dconst_0", 8));
                break;
            case 0xf:
                append(&result, stringOf("dconst_1", 8));
                break;
            case 0x6f:
                append(&result, stringOf("ddiv", 4));
                break;
            case 0x18:
                append(&result, stringOf("dload", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x26:
                append(&result, stringOf("dload_0", 7));
                break;
            case 0x27:
                append(&result, stringOf("dload_1", 7));
                break;
            case 0x28:
                append(&result, stringOf("dload_2", 7));
                break;
            case 0x29:
                append(&result, stringOf("dload_3", 7));
                break;
            case 0x6b:
                append(&result, stringOf("dmul", 4));
                break;
            case 0x77:
                append(&result, stringOf("dneg", 4));
                break;
            case 0x73:
                append(&result, stringOf("drem", 4));
                break;
            case 0xaf:
                append(&result, stringOf("dreturn", 7));
                break;
            case 0x39:
                append(&result, stringOf("dstore", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x47:
                append(&result, stringOf("dstore_0", 8));
                break;
            case 0x48:
                append(&result, stringOf("dstore_1", 8));
                break;
            case 0x49:
                append(&result, stringOf("dstore_2", 8));
                break;
            case 0x4a:
                append(&result, stringOf("dstore_3", 8));
                break;
            case 0x59:
                append(&result, stringOf("dup", 3));
                break;
            case 0x5a:
                append(&result, stringOf("dup_x1", 6));
                break;
            case 0x5b:
                append(&result, stringOf("dup_x2", 6));
                break;
            case 0x5c:
                append(&result, stringOf("dup2", 4));
                break;
            case 0x5d:
                append(&result, stringOf("dup2_x1", 7));
                break;
            case 0x5e:
                append(&result, stringOf("dup2_x2", 7));
                break;
            case 0x8d:
                append(&result, stringOf("f2d", 3));
                break;
            case 0x8b:
                append(&result, stringOf("f2i", 3));
                break;
            case 0x8c:
                append(&result, stringOf("f2l", 3));
                break;
            case 0x62:
                append(&result, stringOf("fadd", 4));
                break;
            case 0x30:
                append(&result, stringOf("faload", 6));
                break;
            case 0x51:
                append(&result, stringOf("fastore", 7));
                break;
            case 0x96:
                append(&result, stringOf("fcmpg", 5));
                break;
            case 0x95:
                append(&result, stringOf("fcmpl", 5));
                break;
            case 0xb:
                append(&result, stringOf("fconst_0", 8));
                break;
            case 0xc:
                append(&result, stringOf("fconst_1", 8));
                break;
            case 0xd:
                append(&result, stringOf("fconst_2", 8));
                break;
            case 0x17:
                append(&result, stringOf("fload", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x22:
                append(&result, stringOf("fload_0", 7));
                break;
            case 0x23:
                append(&result, stringOf("fload_1", 7));
                break;
            case 0x24:
                append(&result, stringOf("fload_2", 7));
                break;
            case 0x25:
                append(&result, stringOf("fload_3", 7));
                break;
            case 0x6a:
                append(&result, stringOf("fmul", 4));
                break;
            case 0x76:
                append(&result, stringOf("fneg", 4));
                break;
            case 0x72:
                append(&result, stringOf("frem", 4));
                break;
            case 0xae:
                append(&result, stringOf("freturn", 7));
                break;
            case 0x38:
                append(&result, stringOf("fstore", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x43:
                append(&result, stringOf("fstore_0", 8));
                break;
            case 0x44:
                append(&result, stringOf("fstore_1", 8));
                break;
            case 0x45:
                append(&result, stringOf("fstore_2", 8));
                break;
            case 0x46:
                append(&result, stringOf("fstore_3", 8));
                break;
            case 0x66:
                append(&result, stringOf("fsub", 4));
                break;
            case 0xb4:
                append(&result, stringOf("getfield", 8));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xb2:
                append(&result, stringOf("getstatic", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa7:
                append(&result, stringOf("goto", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xc8:
                append(&result, stringOf("goto_w", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readIntCode(code.code, &i));
                break;
            case 0x91:
                append(&result, stringOf("i2b", 3));
                break;
            case 0x92:
                append(&result, stringOf("i2c", 3));
                break;
            case 0x87:
                append(&result, stringOf("i2d", 3));
                break;
            case 0x86:
                append(&result, stringOf("i2f", 3));
                break;
            case 0x85:
                append(&result, stringOf("i2l", 3));
                break;
            case 0x93:
                append(&result, stringOf("i2s", 3));
                break;
            case 0x60:
                append(&result, stringOf("iadd", 4));
                break;
            case 0x2e:
                append(&result, stringOf("iaload", 6));
                break;
            case 0x7e:
                append(&result, stringOf("iand", 4));
                break;
            case 0x4f:
                append(&result, stringOf("iastore", 7));
                break;
            case 0x2:
                append(&result, stringOf("iconst_m1", 9));
                break;
            case 0x3:
                append(&result, stringOf("iconst_0", 8));
                break;
            case 0x4:
                append(&result, stringOf("iconst_1", 8));
                break;
            case 0x5:
                append(&result, stringOf("iconst_2", 8));
                break;
            case 0x6:
                append(&result, stringOf("iconst_3", 8));
                break;
            case 0x7:
                append(&result, stringOf("iconst_4", 8));
                break;
            case 0x8:
                append(&result, stringOf("iconst_5", 8));
                break;
            case 0x6c:
                append(&result, stringOf("idiv", 8));
                break;
            case 0xa5:
                append(&result, stringOf("if_acmpeq", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa6:
                append(&result, stringOf("if_acmpne", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9f:
                append(&result, stringOf("if_icmpeq", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa0:
                append(&result, stringOf("if_icmpne", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa1:
                append(&result, stringOf("if_icmplt", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa2:
                append(&result, stringOf("if_icmpge", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa3:
                append(&result, stringOf("if_icmpgt", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa4:
                append(&result, stringOf("if_icmple", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x99:
                append(&result, stringOf("ifeq", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9a:
                append(&result, stringOf("ifne", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9b:
                append(&result, stringOf("iflt", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9c:
                append(&result, stringOf("ifge", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9d:
                append(&result, stringOf("ifgt", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x9e:
                append(&result, stringOf("ifle", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xc7:
                append(&result, stringOf("ifnonnull", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xc6:
                append(&result, stringOf("ifnull", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x84:
                append(&result, stringOf("iinc", 4));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x15:
                append(&result, stringOf("iload", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x1a:
                append(&result, stringOf("iload_0", 7));
                break;
            case 0x1b:
                append(&result, stringOf("iload_1", 7));
                break;
            case 0x1c:
                append(&result, stringOf("iload_2", 7));
                break;
            case 0x1d:
                append(&result, stringOf("iload_3", 7));
                break;
            case 0x68:
                append(&result, stringOf("imul", 4));
                break;
            case 0x74:
                append(&result, stringOf("ineg", 4));
                break;
            case 0xc1:
                append(&result, stringOf("instanceof", 10));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xba:
                append(&result, stringOf("invokedynamic", 13));

                ushort cnst = readShortIn(code.code, &i);

                append(&result, stringOf(" $", 2));
                append(&result, itos(cnst));

                readInvoke(*(CONSTANT_Methodref_info*) constant_pool[cnst], constant_pool, &typeInfo);

                i += 2; // 2x 0

                break;
            case 0xb9:
                append(&result, stringOf("invokeinterface", 15));

                ushort cnst_ = readShortIn(code.code, &i);

                append(&result, stringOf(" $", 2));
                append(&result, itos(cnst_));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));

                readInvoke(*(CONSTANT_Methodref_info*) constant_pool[cnst_], constant_pool, &typeInfo);

                i++; // 1x 0

                break;
            case 0xb7:
                append(&result, stringOf("invokespecial", 13));

                ushort cnst__ = readShortIn(code.code, &i);

                append(&result, stringOf(" $", 2));
                append(&result, itos(cnst__));

                readInvoke(*(CONSTANT_Methodref_info*) constant_pool[cnst__], constant_pool, &typeInfo);

                break;
            case 0xb8:
                append(&result, stringOf("invokestatic", 12));

                ushort cnst___ = readShortIn(code.code, &i);

                append(&result, stringOf(" $", 2));
                append(&result, itos(cnst___));

                readInvoke(*(CONSTANT_Methodref_info*) constant_pool[cnst___], constant_pool, &typeInfo);

                break;
            case 0xb6:
                append(&result, stringOf("invokevirtual", 13));

                ushort cnst____ = readShortIn(code.code, &i);

                append(&result, stringOf(" $", 2));
                append(&result, itos(cnst____));

                readInvoke(*(CONSTANT_Methodref_info*) constant_pool[cnst____], constant_pool, &typeInfo);
                break;
            case 0x80:
                append(&result, stringOf("ior", 3));
                break;
            case 0x70:
                append(&result, stringOf("irem", 4));
                break;
            case 0xac:
                append(&result, stringOf("ireturn", 7));
                break;
            case 0x78:
                append(&result, stringOf("ishl", 4));
                break;
            case 0x7a:
                append(&result, stringOf("ishr", 4));
                break;
            case 0x36:
                append(&result, stringOf("istore", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x3b:
                append(&result, stringOf("istore_0", 8));
                break;
            case 0x3c:
                append(&result, stringOf("istore_1", 8));
                break;
            case 0x3d:
                append(&result, stringOf("istore_2", 8));
                break;
            case 0x3e:
                append(&result, stringOf("istore_3", 8));
                break;
            case 0x64:
                append(&result, stringOf("isub", 4));
                break;
            case 0x7c:
                append(&result, stringOf("iushr", 5));
                break;
            case 0x82:
                append(&result, stringOf("ixor", 4));
                break;
            case 0xa8:
                append(&result, stringOf("jsr", 3));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xc9:
                append(&result, stringOf("jsr_w", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readIntCode(code.code, &i));
                break;
            case 0x8a:
                append(&result, stringOf("l2d", 3));
                break;
            case 0x89:
                append(&result, stringOf("l2f", 3));
                break;
            case 0x88:
                append(&result, stringOf("l2i", 3));
                break;
            case 0x61:
                append(&result, stringOf("ladd", 4));
                break;
            case 0x2f:
                append(&result, stringOf("laload", 6));
                break;
            case 0x7f:
                append(&result, stringOf("land", 4));
                break;
            case 0x50:
                append(&result, stringOf("lastore", 7));
                break;
            case 0x94:
                append(&result, stringOf("lcmp", 4));
                break;
            case 0x9:
                append(&result, stringOf("lconst_0", 8));
                break;
            case 0xa:
                append(&result, stringOf("lconst_1", 8));
                break;
            case 0x12:
                append(&result, stringOf("ldc", 3));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x13:
                append(&result, stringOf("ldc_w", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readIntCode(code.code, &i));
                break;
            case 0x14:
                append(&result, stringOf("ldc2_w", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readIntCode(code.code, &i));
                break;
            case 0x6d:
                append(&result, stringOf("ldiv", 4));
                break;
            case 0x16:
                append(&result, stringOf("lload", 5));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x1e:
                append(&result, stringOf("lload_0", 7));
                break;
            case 0x1f:
                append(&result, stringOf("lload_1", 7));
                break;
            case 0x20:
                append(&result, stringOf("lload_2", 7));
                break;
            case 0x21:
                append(&result, stringOf("lload_3", 7));
                break;
            case 0x69: // nice
                append(&result, stringOf("lmul", 4));
                break;
            case 0x75:
                append(&result, stringOf("lneg", 4));
                break;
            case 0xab:
                append(&result, stringOf("lookupswitch", 12)); // probably done

                i += (i + 1) % 4; // 0-3 byte pad

                ubyte defaultByte1 = byteOf(code.code[++i]);
                ubyte defaultByte2 = byteOf(code.code[++i]);
                ubyte defaultByte3 = byteOf(code.code[++i]);
                ubyte defaultByte4 = byteOf(code.code[++i]);
                int defaultByte = (defaultByte1 << 24) | (defaultByte2 << 16) | (defaultByte3 << 8) | defaultByte4;

                ubyte npairs1 = byteOf(code.code[++i]);
                ubyte npairs2 = byteOf(code.code[++i]);
                ubyte npairs3 = byteOf(code.code[++i]);
                ubyte npairs4 = byteOf(code.code[++i]);
                int npairs = (npairs1 << 24) | (npairs2 << 16) | (npairs3 << 8) | npairs4;

                append(&result, stringOf(" $", 2));
                append(&result, itos(defaultByte));
                append(&result, stringOf(" $", 2));
                append(&result, itos(npairs));

                for (int i2 = 0; i2 < npairs; i2++) {
                    append(&result, readIntCode(code.code, &i));
                    append(&result, readIntCode(code.code, &i));
                }

                break;
            case 0x81:
                append(&result, stringOf("lor", 3));
                break;
            case 0x71:
                append(&result, stringOf("lrem", 4));
                break;
            case 0xad:
                append(&result, stringOf("lreturn", 6));
                break;
            case 0x79:
                append(&result, stringOf("lshl", 4));
                break;
            case 0x7b:
                append(&result, stringOf("lshr", 4));
                break;
            case 0x37:
                append(&result, stringOf("lstore", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0x3f:
                append(&result, stringOf("lstore_0", 8));
                break;
            case 0x40:
                append(&result, stringOf("lstore_1", 8));
                break;
            case 0x41:
                append(&result, stringOf("lstore_2", 8));
                break;
            case 0x42:
                append(&result, stringOf("lstore_3", 8));
                break;
            case 0x65:
                append(&result, stringOf("lsub", 4));
                break;
            case 0x7d:
                append(&result, stringOf("lushr", 5));
                break;
            case 0x83:
                append(&result, stringOf("lxor", 4));
                break;
            case 0xc2:
                append(&result, stringOf("monitorenter", 12));
                break;
            case 0xc3:
                append(&result, stringOf("monitorexit", 11));
                break;
            case 0xc5:
                append(&result, stringOf("multianewarray", 14));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0xbb:
                append(&result, stringOf("new", 3));
                append(&result, stringOf(" $", 2));

                ushort cnst_____ = readShortIn(code.code, &i);

                append(&result, itos(cnst_____));

                String className = readUTFString(*(CONSTANT_Utf8_info*) constant_pool[shortOf((*(CONSTANT_Class_info*) constant_pool[cnst_____]).name_index)]);

                className.content = replaceString(className.content, "/", ".");

                append(&typeInfo, className);

                break;
            case 0xbc:
                append(&result, stringOf("newarray ", 9));

                switch (byteOf(code.code[++i])) {
                    case 4:
                        append(&result, stringOf("T_BOOLEAN", 9));
                        break;
                    case 5:
                        append(&result, stringOf("T_CHAR", 6));
                        break;
                    case 6:
                        append(&result, stringOf("T_FLOAT", 7));
                        break;
                    case 7:
                        append(&result, stringOf("T_DOUBLE", 8));
                        break;
                    case 8:
                        append(&result, stringOf("T_BYTE", 6));
                        break;
                    case 9:
                        append(&result, stringOf("T_SHORT", 7));
                        break;
                    case 10:
                        append(&result, stringOf("T_INT", 5));
                        break;
                    case 11:
                        append(&result, stringOf("T_LONG", 6));
                        break;
                    default:
                        append(&result, stringOf("T_UNKNOWN", 9));
                }
                break;
            case 0x0:
                append(&result, stringOf("nop", 3));
                break;
            case 0x57:
                append(&result, stringOf("pop", 3));
                break;
            case 0x58:
                append(&result, stringOf("pop2", 4));
                break;
            case 0xb5:
                append(&result, stringOf("putfield", 8));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xb3:
                append(&result, stringOf("putstatic", 9));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0xa9:
                append(&result, stringOf("ret", 3));
                append(&result, stringOf(" $", 2));
                append(&result, readByteCode(code.code, &i));
                break;
            case 0xb1:
                append(&result, stringOf("return", 6));
                break;
            case 0x35:
                append(&result, stringOf("saload", 6));
                break;
            case 0x56:
                append(&result, stringOf("sastore", 7));
                break;
            case 0x11:
                append(&result, stringOf("sipush", 6));
                append(&result, stringOf(" $", 2));
                append(&result, readShortCode(code.code, &i));
                break;
            case 0x5f:
                append(&result, stringOf("swap", 4));
                break;
            case 0xaa:
                append(&result, stringOf("tableswitch", 11));

                i += (i + 1) % 4; // 0-3 byte pad

                ubyte defaultByte1_ = byteOf(code.code[++i]);
                ubyte defaultByte2_ = byteOf(code.code[++i]);
                ubyte defaultByte3_ = byteOf(code.code[++i]);
                ubyte defaultByte4_ = byteOf(code.code[++i]);
                int defaultByte_ = (defaultByte1_ << 24) | (defaultByte2_ << 16) | (defaultByte3_ << 8) | defaultByte4_;

                ubyte lowbyte1 = byteOf(code.code[++i]);
                ubyte lowbyte2 = byteOf(code.code[++i]);
                ubyte lowbyte3 = byteOf(code.code[++i]);
                ubyte lowbyte4 = byteOf(code.code[++i]);
                int lowbyte = (lowbyte1 << 24) | (lowbyte2 << 16) | (lowbyte3 << 8) | lowbyte4;

                ubyte highbyte1 = byteOf(code.code[++i]);
                ubyte highbyte2 = byteOf(code.code[++i]);
                ubyte highbyte3 = byteOf(code.code[++i]);
                ubyte highbyte4 = byteOf(code.code[++i]);
                int highbyte = (highbyte1 << 24) | (highbyte2 << 16) | (highbyte3 << 8) | highbyte4;

                append(&result, stringOf(" $", 2));
                append(&result, itos(defaultByte_));
                append(&result, stringOf(" $", 2));
                append(&result, itos(lowbyte));
                append(&result, stringOf(" $", 2));
                append(&result, itos(highbyte));

                for (int i2 = 0; i2 < (highbyte - lowbyte + 1); i2++) {
                    append(&result, readIntCode(code.code, &i));
                }

                break;
            case 0xc4:
                append(&result, stringOf("wide", 4));

                ubyte opcode = byteOf(code.code[++i]);

                append(&result, stringOf(" $", 2));
                append(&result, itos(opcode));
                append(&result, readShortCode(code.code, &i));

                if (opcode == 0x84) { // iinc
                    append(&result, readShortCode(code.code, &i));
                }

                break;
            default:
                append(&result, stringOf("(unknown)", 9));
        }

        append(&result, stringOf(";", 1));

        if (typeInfo.length != 0) {
            append(&result, stringOf(" // ", 4));
            append(&result, typeInfo);
        }

        append(&result, stringOf("\n", 1));
    }

    return result;
}

char* writeClassFile(ClassFile class) {
    String output = stringEmpty();
    char* sourceFileName = "";
    int sourceFileNameLen = 0;

    for (int i = 0; i < shortOf(class.attributes_count); i++) {
        attribute_info* attribute = class.attributes[i];
        char* name = readUTF(*(CONSTANT_Utf8_info*) class.constant_pool[shortOf(attribute->attribute_name_index)]);

        if (!strcmp(name, "SourceFile")) {
            CONSTANT_Utf8_info utf8 = *(CONSTANT_Utf8_info*) class.constant_pool[shortOf((*(SourceFile_attribute*) attribute).sourcefile_index)];

            sourceFileName = readUTF(utf8);
            sourceFileNameLen = shortOf(utf8.length);
        } else if (!strcmp(name, "Signature")) {
            CONSTANT_Utf8_info utf8 = *(CONSTANT_Utf8_info*) class.constant_pool[shortOf((*(Signature_attribute*) attribute).signature_index)];

//            appendUtf8(&output, utf8);
            // TODO
        }
    }

    append(&output, stringOf("// This file was compiled from \"", 32));
    append(&output, stringOf(sourceFileName, sourceFileNameLen));
    append(&output, stringOf("\" in Java ", 10));
    append(&output, readJavaVersion(shortOf(class.major_version), shortOf(class.minor_version)));
    append(&output, stringOf("\n", 1));

    append(&output, writeClassAccessFlags(shortOf(class.access_flags), true));

    CONSTANT_Utf8_info utf8 = UTF8(class.constant_pool, (*(CONSTANT_Class_info*) class.constant_pool[shortOf(class.this_class)]).name_index);
    String className = readUTFString(utf8);

    className.content = replaceString(className.content, "/", ".");

    append(&output, className);

    ushort superClass = shortOf(class.super_class);

    if (superClass != 0) {
        append(&output, stringOf(" extends ", 9));

        CONSTANT_Class_info superCls = *(CONSTANT_Class_info*) class.constant_pool[superClass];
        CONSTANT_Utf8_info super = UTF8(class.constant_pool, superCls.name_index);
        char* superName = readUTF(super);

        superName = replaceString(superName, "/", ".");

        append(&output, stringOf(superName, shortOf(super.length)));
    }

    ushort interfacesCount = shortOf(class.interfaces_count);

    if (interfacesCount != 0) {
        append(&output, stringOf(" implements ", 12));
    }

    for (int i = 0; i < interfacesCount; i++) {
        CONSTANT_Class_info superCls = *(CONSTANT_Class_info*) class.constant_pool[shortOf(class.interfaces[i])];
        CONSTANT_Utf8_info super = UTF8(class.constant_pool, superCls.name_index);
        char* superName = readUTF(super);

        superName = replaceString(superName, "/", ".");

        append(&output, stringOf(superName, shortOf(super.length)));

        if (i != interfacesCount - 1)
            append(&output, stringOf(", ", 2));
    }

    append(&output, stringOf(" {\n", 3));

    String methods = stringEmpty();

    for (int i = 0; i < shortOf(class.methods_count); i++) {
        method_info method = class.methods[i];

        if ((shortOf(method.access_flags) & 0x1000) != 0) { // synthetic
            continue;
        }

        String meth = stringOf("\t", 1);
        CONSTANT_Utf8_info nameRaw = UTF8(class.constant_pool, method.name_index);
        char* name = readUTF(nameRaw);
        Code_attribute code = *(Code_attribute*) findAttribute("Code", shortOf(method.attributes_count), method.attributes, class.constant_pool);

        if (!strcmp(name, "<init>")) { // constructor
            nameRaw = utf8;
            name = className.content;
        } else if (!strcmp(name, "<clinit>")) { // static initalizer
            nameRaw = utf8;

            append(&meth, stringOf("static ", 7));
        }

        append(&meth, writeClassAccessFlags(shortOf(method.access_flags), false));

        if (!strcmp(name, "<clinit>")) {
            append(&meth, stringOf(name, shortOf(nameRaw.length)));
            append(&meth, stringOf("(", 1));
            append(&meth, stringOf(")", 1));
        } else {
            CONSTANT_Utf8_info descriptor = UTF8(class.constant_pool, method.descriptor_index);
            Tuple desc = readMethodDescriptor(stringOf(readUTF(descriptor), shortOf(descriptor.length)));

            if (strcmp(name, "<init>") != 0) {
                append(&meth, *(String*) desc.b);
                append(&meth, stringOf(" ", 1));
            }

            append(&meth, stringOf(name, shortOf(nameRaw.length)));
            append(&meth, stringOf("(", 1));
            append(&meth, *(String*) desc.a);
            append(&meth, stringOf(")", 1));
        }

        if ((shortOf(method.access_flags) & 0x0100) != 0) { // native
            append(&meth, stringOf(";\n", 2));
        } else {
            append(&meth, stringOf(" {\n", 3));
            append(&meth, readCode(code, class.constant_pool));
            append(&meth, stringOf("\t}\n", 3));
        }

        append(&meth, stringOf("\n", 1));
        append(&methods, meth);
    }

    methods = substring(methods, 0, methods.length - 2);

    append(&output, methods);
    append(&output, stringOf("}", 1));

    return content(output);
}