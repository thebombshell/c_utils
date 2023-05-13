
/**
 * json_import.h
 */

#ifndef C_UTILS_JSON_IMPORT_H
#define C_UTILS_JSON_IMPORT_H

#include "data_structures.h"

#define json_data_type_undefined 0x00
#define json_data_type_null 0x01
#define json_data_type_array 0x02
#define json_data_type_boolean 0x03
#define json_data_type_number 0x04
#define json_data_type_object 0x05
#define json_data_type_string 0x06

typedef void* json_null;

typedef vector json_array;

typedef unsigned int json_boolean;

typedef double json_number;

typedef hash_list json_object;

typedef buffer json_string;

typedef union {
	
	json_array array;
	json_object object;
	json_string string;
} json_blob, *p_json_blob;

typedef struct {
	
	unsigned int type;
	union {
		
		json_null null;
		json_boolean boolean;
		json_number number;
		json_blob* blob;
	} value;
} json_value, *p_json_value;

typedef struct {
	
	json_object root;
	factory blob_factory;
	factory value_factory;
	
} json_file, *p_json_file;

int json_load(json_file* t_json, const char* t_string);

void json_final(json_file* t_json);

int json_stringify_value(p_json_value t_value, p_vector t_string);

int json_stringify(p_json_file t_json, buffer* t_out_string);

#endif