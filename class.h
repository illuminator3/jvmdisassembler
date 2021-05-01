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

#ifndef JVMDISASSEMBLER_CLASS_H
#define JVMDISASSEMBLER_CLASS_H

typedef struct OneByte {
    unsigned char byte;
} u1;

typedef struct TwoBytes {
    u1 first;
    u1 second;
} u2;

typedef struct FourBytes {
    u2 first;
    u2 second;
} u4;

u1 u1of(unsigned char first);
u2 u2of(unsigned char first, unsigned char second);
u4 u4of(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth);

unsigned char byteOf(u1 u);
unsigned short shortOf(u2 u);
unsigned int intOf(u4 u);

typedef struct {
    u1 tag;
    u1* info;
} cp_info;

typedef struct {
    u1 tag;
    u2 name_index;
} CONSTANT_Class_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_InterfaceMethodref_info;

typedef struct {
    u1 tag;
    u2 string_index;
} CONSTANT_String_info;

typedef struct {
    u1 tag;
    u4 bytes;
} CONSTANT_Integer_info;

typedef struct {
    u1 tag;
    u4 bytes;
} CONSTANT_Float_info;

typedef struct {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Long_info;

typedef struct {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Double_info;

typedef struct {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct {
    u1 tag;
    u2 length;
    u1* bytes;
} CONSTANT_Utf8_info;

char* readUTF(CONSTANT_Utf8_info from);

typedef struct {
    u1 tag;
    u1 reference_kind;
    u2 reference_index;
} CONSTANT_MethodHandle_info;

typedef struct {
    u1 tag;
    u2 descriptor_index;
} CONSTANT_MethodType_info;

typedef struct {
    u1 tag;
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} CONSTANT_InvokeDynamic_info;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1* info;
} attribute_info;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constantvalue_index;
} ConstantValue_attribute;

typedef struct {
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} exception_table_inner;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1* code;
    u2 exception_table_length;
    exception_table_inner* exception_table;
    u2 attributes_count;
    attribute_info** attributes;
} Code_attribute;

typedef struct {
    u1 tag;
    u1* data;
} verification_type_info;

typedef struct {
    u1 tag; // 0
} Top_variable_info;

typedef struct {
    u1 tag; // 1
} Integer_variable_info;

typedef struct {
    u1 tag; // 2
} Float_variable_info;

typedef struct {
    u1 tag; // 3
} Double_variable_info;

typedef struct {
    u1 tag; // 4
} Long_variable_info;

typedef struct {
    u1 tag; // 5
} Null_variable_info;

typedef struct {
    u1 tag; // 6
} UninitializedThis_variable_info;

typedef struct {
    u1 tag; // 7
    u2 cpool_index;
} Object_variable_info;

typedef struct {
    u1 tag; // 8
    u2 offset;
} Uninitialized_variable_info;

typedef struct {
    u1 frame_type;
    u1* data;
} stack_map_frame;

typedef struct {
    u1 frame_type; // 0-63
} same_frame;

typedef struct {
    u1 frame_type; // 64-127
    verification_type_info* stack;
} same_locals_1_stack_item_frame;

typedef struct {
    u1 frame_type; // 247
    u2 offset_delta;
    verification_type_info* stack;
} same_locals_1_stack_item_frame_extended;

typedef struct {
    u1 frame_type; // 248-250
    u2 offset_delta;
} chop_frame;

typedef struct {
    u1 frame_type; // 251
    u2 offset_delta;
} same_frame_extended;

typedef struct {
    u1 frame_type; // 252-254
    u2 offset_delta;
    verification_type_info** locals; // [frame_type - 251]
} append_frame;

typedef struct {
    u1 frame_type; // 255
    u2 offset_delta;
    u2 number_of_locals;
    verification_type_info** locals;
    u2 number_of_stack_items;
    verification_type_info** stack;
} full_frame;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_entries;
    stack_map_frame** entries;
} StackMapTable_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_exceptions;
    u2* exception_index_table;
} Exceptions_attribute;

typedef struct {
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
} InnerClasses_attribute_inner;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_classes;
    InnerClasses_attribute_inner* classes;
} InnerClasses_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 class_index;
    u2 method_index;
} EnclosingMethod_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
} Synthetic_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 signature_index;
} Signature_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 sourcefile_index;
} SourceFile_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1* debug_extension;
} SourceDebugExtension_attribute;

typedef struct {
    u2 start_pc;
    u2 line_number;
} line_number_table_element;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    line_number_table_element* line_number_table;
} LineNumberTable_attribute;

typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
} local_variable_table_element;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_table_length;
    local_variable_table_element* local_variable_table;
} LocalVariableTable_attribute;

typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;
} local_variable_type_table_element;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_type_table_length;
    local_variable_type_table_element* local_variable_type_table;
} LocalVariableTypeTable_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
} Deprecated_attribute;

typedef struct {
    u1 tag;
    u1* data;
} element_value;

typedef struct {
    u2 element_name_index;
    element_value* value;
} element_value_pairs_item;

typedef struct {
    u2 type_index;
    u2 num_element_value_pairs;
    element_value_pairs_item* element_value_pairs;
} annotation;

typedef struct {
    u1 tag;
    u2 num_value;
    element_value** values;
} element_value_array_value;

typedef struct {
    u1 tag;
    annotation annotation_value;
} element_value_annotation_value;

typedef struct {
    u1 tag;
    u2 class_info_index;
} element_value_class_info_index;

typedef struct {
    u1 tag;
    u2 type_name_index;
    u2 const_name_index;
} element_value_enum_const_value;

typedef struct {
    u1 tag;
    u2 constant_value_index;
} element_value_const_value_index;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation* annotations;
} RuntimeVisibleAnnotations_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation* annotations;
} RuntimeInvisibleAnnotations_attribute;

typedef struct {
    u2 num_annotations;
    annotation* annotations;
} parameter_annotations_element;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations_element* parameter_annotations;
} RuntimeVisibleParameterAnnotations_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations_element* parameter_annotations;
} RuntimeInvisibleParameterAnnotations_attribute;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    element_value* default_value;
} AnnotationDefault_attribute;

typedef struct {
    u2 bootstrap_method_ref;
    u2 num_bootstrap_arguments;
    u2* bootstrap_arguments;
} bootstrap_method;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_bootstrap_methods;
    bootstrap_method* bootstrap_methods;
} BootstrapMethods_attribute;

typedef struct {
    u2                  access_flags;
    u2                  name_index;
    u2                  descriptor_index;
    u2                  attributes_count;
    attribute_info**    attributes;
} field_info;

typedef struct {
    u2                  access_flags;
    u2                  name_index;
    u2                  descriptor_index;
    u2                  attributes_count;
    attribute_info**    attributes;
} method_info;

typedef struct {
    u4                  magic;
    u2                  minor_version;
    u2                  major_version;
    u2                  constant_pool_count;
    cp_info**           constant_pool; // [](cp_info*)
    u2                  access_flags;
    u2                  this_class;
    u2                  super_class;
    u2                  interfaces_count;
    u2*                 interfaces;
    u2                  fields_count;
    field_info*         fields;
    u2                  methods_count;
    method_info*        methods;
    u2                  attributes_count;
    attribute_info**    attributes;
} ClassFile;

typedef struct {
    unsigned char* actual;
    unsigned int size;
    long ptr;
} Bytes;

Bytes bytesOf(const char* content, unsigned int size);

unsigned char readByte(Bytes* bytes);

u1 readu1(Bytes* bytes);
u2 readu2(Bytes* bytes);
u4 readu4(Bytes* bytes);

u1* readu1array(u2 len, Bytes* bytes);
u2* readu2array(u2 len, Bytes* bytes);

// this is so stupid
annotation readAnnotation(Bytes* bytes);

cp_info** readConstantPool(u2 count, Bytes* bytes);
field_info* readFields(cp_info** constant_pool, u2 count, Bytes* bytes);
method_info* readMethods(cp_info** constant_pool, u2 count, Bytes* bytes);
attribute_info** readAttributes(cp_info** constant_pool, u2 count, Bytes* bytes);
ClassFile readClassFile(Bytes bytes);

char* writeClassFile(ClassFile class);

#endif