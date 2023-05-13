
#include "json_import.h"

#include "assert.h"
#include "stdio.h"

#ifndef JSON_DEBUG
#define JSON_DEBUG 1
#endif

#ifndef JSON_DEBUG_LOG
#define JSON_DEBUG_LOG 1
#endif

#ifndef JSON_DEBUG_VERBOSE
#define JSON_DEBUG_VERBOSE 1
#endif

#ifndef JSON_DEBUG_LOG_LOAD
#define JSON_DEBUG_LOG_LOAD 0
#endif

#ifndef JSON_DEBUG_LOG_STRINGIFY
#define JSON_DEBUG_LOG_STRINGIFY 1
#endif

#ifndef JSON_NOP
#define JSON_NOP (void)printf
#endif

#if JSON_DEBUG && JSON_DEBUG_LOG
#if JSON_DEBUG_VERBOSE
#define JSON_LOG_DEFINITION(...) (printf("[%s:%i] JSON LOG\n", __FILE__, __LINE__), printf(__VA_ARGS__), printf("\n"))
#define JSON_LOG_VERBOSE(...) (JSON_LOG_DEFINITION(__VA_ARGS__))
#else
#define JSON_LOG_DEFINITION(...) (printf(__VA_ARGS__), printf("\n"))
#endif
#else
#define JSON_LOG_DEFINITION(...) JSON_NOP
#endif

#define JSON_LOG(...) JSON_LOG_DEFINITION(__VA_ARGS__) 

#if JSON_DEBUG && JSON_DEBUG_STACK_COUNT 
#define JSON_STACK_COUNT(T_COUNT, ...) { int JSON_STACK_COUNT_INT = T_COUNT; for (; JSON_STACK_COUNT_INT; --JSON_STACK_COUNT_INT) printf(">"); printf(" " __VA_ARGS__); printf("\n"); }
#else
#define JSON_STACK_COUNT(T_COUNT, ...) JSON_NOP
#endif

#if JSON_DEBUG && JSON_DEBUG_PROPERTY 
#define JSON_PROPERTY_LOG(...) JSON_LOG(__VA_ARGS__)
#else
#define JSON_PROPERTY_LOG(...) JSON_NOP
#endif

#define JSON_LOAD_ERR_MESSAGE() (JSON_LOG("JSON FAILURE"))

#ifndef JSON_LOG_VERBOSE
#define JSON_LOG_VERBOSE(...) JSON_NOP
#endif

#if !JSON_DEBUG_LOG_LOAD
#undef JSON_LOG
#define JSON_LOG(...) JSON_NOP
#else
#undef JSON_LOG
#define JSON_LOG(...) JSON_LOG_DEFINITION(__VA_ARGS__)
#endif

const json_value json_undefined = {0};

int is_whitespace(char t_c)
{
	return t_c == ' ' || t_c == '\t' || t_c == '\n' || t_c == '\r';
}

int json_skip_whitespace(FILE* t_file)
{
	assert(t_file);
	
	char c;
	do
	{
		if (!fread(&c, 1, 1, t_file))
		{
			JSON_LOG("failed to read whitespace");
			return 0;
		}
	}
	while (is_whitespace(c));
	
	return !fseek(t_file, -1, SEEK_CUR);
}

int json_read_string(FILE* t_file, buffer* t_out_string)
{
	assert(t_file);
	assert(t_out_string);
	
	JSON_LOG("begin read string");
	
	char s[1024] = {0};
	
	if (!fread(s, 1, 1, t_file) || s[0] != '"')
	{
		JSON_LOG("failed to read opening quote");
		return 0;
	}
	
	s[0] = '\0';
	char* c = s;
	int is_expecting_escape_character = 0;
	
	if (!buffer_init(t_out_string, 0))
	{
		JSON_LOG("failed to initialize buffer");
		return 0;
	}
	
	goto json_read_c;
	
	while (is_expecting_escape_character || *c != '"')
	{
		if (*c == '\\')
		{
			is_expecting_escape_character = 1;
			goto json_read_c;
		}
		if (is_expecting_escape_character)
		{
			is_expecting_escape_character = 0;
			
			if (*c == 'b')
			{
				*c = '\b';
			}
			else if (*c == 'f')
			{
				*c = '\f';
			}
			else if (*c == 'n')
			{
				*c = '\n';
			}
			else if (*c == 't')
			{
				*c = '\t';
			}
		}
		
		++c;
		if (c == s + sizeof(s))
		{
			if (!buffer_append(t_out_string, s, 1024))
			{
				JSON_LOG("failed to append large string");
				buffer_final(t_out_string);
				return 0;
			}
			memset(s, 0, 1024);
			c = s;
		}
		
json_read_c:
		if (!fread(c, 1, 1, t_file))
		{
			JSON_LOG("failed to read string character");
			buffer_final(t_out_string);
			return 0;
		}
	}
	*c = '\0';
	
	if (!buffer_append(t_out_string, s, (c - s) + 1))
	{
		JSON_LOG("failed to append string");
		buffer_final(t_out_string);
		return 0;
	}
	
	JSON_LOG("succeed read string");
	
	return 1;
}

int json_read_value(FILE* t_file, json_file * t_json, json_value* t_out_value)
{
	assert(t_file);
	
	JSON_LOG("begin read value");
	
	*t_out_value = json_undefined;
	char s[64] = {0};
	
	if (!json_skip_whitespace(t_file) || !fread(s, 1, 1, t_file))
	{
		JSON_LOG("failed to read first character");
		return 0;
	}
	
	if (s[0] == '{')
	{
		/* object */
		
		JSON_LOG("value is object");
		
		t_out_value->type = json_data_type_object;
		t_out_value->value.blob = factory_alloc(&t_json->blob_factory);
		
		if (!t_out_value->value.blob)
		{
			JSON_LOG("failed to acquire blob");
			return 0;
		}
		
		json_object* object = &t_out_value->value.blob->object;
		
		if (!hash_list_init((p_hash_list)object))
		{
			JSON_LOG("failed to initialize object");
			return 0;
		}
		
		int is_expecting_separator = 0;
		
		do
		{
			json_skip_whitespace(t_file);
			if (!fread(s, 1, 1, t_file))
			{
				JSON_LOG("failed to read first character of property");
				return 0;
			}
			
			if (s[0] == '"')
			{
				/* property name */
				
				if (fseek(t_file, -1, SEEK_CUR))
				{
					JSON_LOG("failed to seek for property name");
					goto json_read_value_object_fail;
				}
				
				if (is_expecting_separator)
				{
					JSON_LOG("unexpected string");
					goto json_read_value_object_fail;
				}
				
				buffer name;
				
				if (!json_read_string(t_file, &name))
				{
					JSON_LOG("failed to read property name");
					goto json_read_value_object_fail;
				}
				
				JSON_LOG("property \"%s\" : ", (char*)name.data);
				if (!json_skip_whitespace(t_file) || !fread(s, 1, 1, t_file) || s[0] != ':')
				{
					JSON_LOG("failed to read colon");
					buffer_final(&name);
					goto json_read_value_object_fail;
				}
				
				json_value* value = factory_alloc(&t_json->value_factory);
				
				if (!value)
				{
					JSON_LOG("failed to acquire value from factory");
					buffer_final(&name);
					goto json_read_value_object_fail;
				}
				
				/* I hate recursion, but I want this working, I can unwind this from recursion later */
				
				if (!json_skip_whitespace(t_file) || !json_read_value(t_file, t_json, value))
				{
					JSON_LOG("failed to read value");
					factory_free(&t_json->value_factory, value);
					buffer_final(&name);
					goto json_read_value_object_fail;
				}
				
				if (!hash_list_insert((p_hash_list)object, (char*)name.data, value))
				{
					JSON_LOG("failed to insert property");
					factory_free(&t_json->value_factory, value);
					buffer_final(&name);
					goto json_read_value_object_fail;
				}
				
				is_expecting_separator = 1;
			}
			else if (s[0] == ',')
			{
				/* property separator */
				
				if (!is_expecting_separator)
				{
					JSON_LOG("unexpected separator");
					goto json_read_value_object_fail;
				}
				is_expecting_separator = 0;
			}
			else if (s[0] == '}')
			{
				/* end of object */
			}
			else
			{
				/* unexpected result, fail out */
				
				JSON_LOG("character is unexpected, fail out");
				
json_read_value_object_fail:

				factory_free(&t_json->blob_factory, t_out_value->value.blob);
				t_out_value->type = json_data_type_undefined;
				t_out_value->value.blob = 0;
				return 0;
			}
		
		}
		while (s[0] != '}');
	}
	else if (s[0] == '[')
	{
		/* array */
		
		JSON_LOG("value is array");
		
		t_out_value->type = json_data_type_array;
		t_out_value->value.blob = factory_alloc(&t_json->blob_factory);
		
		if (!t_out_value->value.blob)
		{
			JSON_LOG("failed to acquire blob");
			return 0;
		}
		
		json_array* array = &t_out_value->value.blob->array;
		if (!vector_init((p_vector)array, sizeof(json_value)))
		{
			JSON_LOG("failed to initialize array");
			factory_free(&t_json->blob_factory, t_out_value->value.blob);
			t_out_value->type = json_data_type_undefined;
			t_out_value->value.blob = 0;
			return 0;
		}
		
		int is_expecting_separator = 0;
		
		do
		{
			json_skip_whitespace(t_file);
			if (!fread(s, 1, 1, t_file))
			{
				JSON_LOG("failed to read first character of entry");
				return 0;
			}
			
			if (s[0] == ',')
			{
				/* property separator */
				
				if (!is_expecting_separator)
				{
					JSON_LOG("unexpected separator");
					goto json_read_value_array_fail;
				}
				is_expecting_separator = 0;
			}
			else if (s[0] == ']')
			{
				/* end of array */
			}
			else
			{
				/* property value */
				
				if (fseek(t_file, -1, SEEK_CUR))
				{
					JSON_LOG("failed to seek for array value");
					goto json_read_value_array_fail;
				}
				
				if (is_expecting_separator || !json_skip_whitespace(t_file))
				{
					JSON_LOG("unexpected value");
					goto json_read_value_array_fail;
				}
				
				json_value value;
				
				/* I hate recursion, but I want this working, I can unwind this from recursion later */
				
				if (!json_read_value(t_file, t_json, &value) || !vector_push((p_vector)array, &value))
				{
					JSON_LOG("failed to read and push value");
					
json_read_value_array_fail:
				
					vector_final(array);
					factory_free(&t_json->blob_factory, t_out_value->value.blob);
					t_out_value->type = json_data_type_undefined;
					t_out_value->value.blob = 0;
					return 0;
				}
				
				is_expecting_separator = 1;
			}
		}
		while (s[0] != ']');
	}
	else if (s[0] == '"')
	{
		/* string */
		
		JSON_LOG("value is string");
		
		t_out_value->type = json_data_type_string;
		t_out_value->value.blob = factory_alloc(&t_json->blob_factory);
		
		if (!t_out_value->value.blob)
		{
			JSON_LOG("failed to acquire blob");
			return 0;
		}
		
		if (fseek(t_file, -1, SEEK_CUR) || !json_read_string(t_file, (p_buffer)&t_out_value->value.blob->string))
		{
			JSON_LOG("failed to seek and read string");
			
			factory_free(&t_json->blob_factory, t_out_value->value.blob);
			t_out_value->type = json_data_type_undefined;
			t_out_value->value.blob = 0;
			return 0;
		}
	}
	else if (s[0] == '-' || (s[0] >= '0' && s[0] <= '9'))
	{
		/* number */
		
		JSON_LOG("value is number");
		
		char* c = s;
		int has_exponent_been_used = 0;
		int has_decimal_been_used = 0;
		int has_minus_been_used = 1;
		int skip_check = 1;
		
		while (skip_check || ((*c) >= '0' && (*c) <= '9' && c - s < 64))
		{
			++c;
			skip_check = 0;
			if (!fread(c, 1, 1, t_file))
			{
				JSON_LOG("failed to reach number character");
				return 0;
			}
			if ((*c) == '-')
			{
				if (has_minus_been_used)
				{
					JSON_LOG("unexpected minus");
					/* minus in wrong place, fail out */
					return 0;
				}
				has_decimal_been_used = 1;
				skip_check = 1;
			}
			else if ((*c) == '.')
			{
				if (has_decimal_been_used)
				{
					JSON_LOG("unexpected decimal");
					/* too many decimals, fail out */
					return 0;
				}
				has_decimal_been_used = 1;
				has_minus_been_used = 1;
				skip_check = 1;
			}
			else if ((*c) == 'e' || (*c) == 'E')
			{
				if (has_exponent_been_used)
				{
					JSON_LOG("unexpected exponent");
					/* too many exponents, fail out */
					return 0;
				}
				has_exponent_been_used = 1;
				has_decimal_been_used = 0;
				has_minus_been_used = 0;
				skip_check = 1;
			}
			else
			{
				has_minus_been_used = 1;
			}
		};
		
		*c = '\0';
		if (fseek(t_file, -1, SEEK_CUR))
		{
			JSON_LOG("failed to seek to reset end of number");
			return 0;
		}
		t_out_value->type = json_data_type_number;
		t_out_value->value.number = strtod(s, 0);
	}
	else if (s[0] == 't' || s[0] == 'f')
	{
		/* boolean */
		
		JSON_LOG("value is boolean");
		
		if (s[0] == 't')
		{
			if (fread(s + 1, 1, 3, t_file) != 3)
			{
				JSON_LOG("failed to read true");
				return 0;
			}
			s[4] = '\0';
			if (strcmp(s, "true"))
			{
				JSON_LOG("unknown result, expected true");
				return 0;
			}
			t_out_value->type = json_data_type_boolean;
			t_out_value->value.boolean = 1;
		}
		else if (s[0] == 'f')
		{
			if (fread(s + 1, 1, 4, t_file) != 4)
			{
				JSON_LOG("failed to read false");
				return 0;
			}
			s[5] = '\0';
			if (strcmp(s, "false"))
			{
				JSON_LOG("unknown result, expected false");
				return 0;
			}
			t_out_value->type = json_data_type_boolean;
			t_out_value->value.boolean = 0;
		}
	}
	else if (s[0] == 'n')
	{
		/* null */
		
		JSON_LOG("value is null");
		
		if (fread(s + 1, 1, 3, t_file) != 3)
		{
			JSON_LOG("failed to read null");
			return 0;
		}
		s[4] = '\0';
		if (strcmp(s, "null"))
		{
			JSON_LOG("unknown result, expected null");
			return 0;
		}
		t_out_value->type = json_data_type_null;
		t_out_value->value.null = 0;
	}
	else
	{
		/* unknown */
		
		JSON_LOG("value is unexpected, fail out");
		
		return 0;
	}
	
	JSON_LOG("succeed read value");
	
	return 1;
}

int json_load(json_file* t_json, const char* t_string)
{
	assert(t_json && t_string);
	
	JSON_LOG("begin load");
	
	FILE* file = fopen(t_string, "rb");
	if (!file)
	{
		JSON_LOG("failed to open file");
		return 0;
	}
	
	fseek(file, 0, SEEK_END);
	long int file_length = ftell(file);
	(void)file_length;

	fseek(file, 0, SEEK_SET);
	
	if (!factory_init(&t_json->blob_factory, sizeof(json_blob), 32))
	{
		JSON_LOG("failed to init blob factory");
		fclose(file);
		return 0;
	}
	if (!factory_init(&t_json->value_factory, sizeof(json_value), 256))
	{
		JSON_LOG("failed to init value factory");
		factory_final(&t_json->blob_factory);
		fclose(file);
		return 0;
	}
	
	json_value root;
	
	if (!json_read_value(file, t_json, &root) || root.type != json_data_type_object)
	{
		JSON_LOG("failed to read root object");
		factory_final(&t_json->blob_factory);
		factory_final(&t_json->value_factory);
		fclose(file);
		return 0;
	}
	
	t_json->root = root.value.blob->object;
	
	fclose(file);
	
	JSON_LOG("succeeded load");
	
	return 1;
}

void json_final(json_file* t_json)
{
	assert(t_json);
	
	factory_final(&t_json->blob_factory);
	factory_final(&t_json->value_factory);
}

#if !JSON_DEBUG_LOG_STRINGIFY
#undef JSON_LOG
#define JSON_LOG(...) JSON_NOP
#else
#undef JSON_LOG
#define JSON_LOG(...) JSON_LOG_DEFINITION(__VA_ARGS__)
#endif

int json_push_string(p_vector t_vector, const char* t_string, int t_should_expand_escape_characters)
{
	const char* c = t_string;
	
	for (; *c != '\0'; ++c)
	{
		if (t_should_expand_escape_characters)
		{
			if (*c == '\"')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "\""))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\\')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "\\"))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\b')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "b"))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\f')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "f"))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\n')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "n"))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\r')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "r"))
				{
					return 0;
				}
				continue;
			}
			else if (*c == '\t')
			{
				if (!vector_push(t_vector, "\\") || !vector_push(t_vector, "t"))
				{
					return 0;
				}
				continue;
			}
		}
		
		if (!vector_push(t_vector, c))
		{
			return 0;
		}
	}
	
	return 1;
}

typedef struct
{
	p_link_list list;
	p_vector string;
} json_stringify_property_context, *p_json_stringify_property_context;

void json_for_each_object_property_stringify(p_link t_object, void* t_context)
{
	assert(t_object);
	assert(t_context);
	
	p_json_stringify_property_context context = (p_json_stringify_property_context)t_context;
	hash_pair* pair = (p_hash_pair)t_object->data;
	
	int is_first = context->list->end == t_object->prev;
	
	if (!is_first && !vector_push(context->string, ","))
	{
		JSON_LOG("failed to stringify separator");
		return;
	}
	
	if (!json_push_string(context->string, "\n\"", 0))
	{
		JSON_LOG("failed to stringify property name open quote");
		return;
	}
	
	if (!json_push_string(context->string, pair->key, 1))
	{
		JSON_LOG("failed to stringify property name");
		return;
	}
	
	if (!json_push_string(context->string, "\" : ", 0))
	{
		JSON_LOG("failed to stringify property name end quote");
		return;
	}
	
	if (!json_stringify_value((json_value*)pair->data, context->string))
	{
		JSON_LOG("failed to stringify property value");
		return;
	}
}

void json_for_each_array_property_stringify(unsigned int t_index, void* t_element, void* t_context)
{
	assert(t_element);
	assert(t_context);
	
	p_vector string = (p_vector)t_context;
	json_value* value = (json_value*)t_element;
	
	if (t_index != 0)
	{
		if (!vector_push(string, ","))
		{
			JSON_LOG("failed to stringify separator");
			return;
		}
	}
	
	if (!vector_push(string, "\n"))
	{
		JSON_LOG("failed to stringify property value newline");
		return;
	}
	
	if (!json_stringify_value(value, string))
	{
		JSON_LOG("failed to stringify property value");
		return;
	}
}

int json_stringify_value(json_value* t_value, vector* t_string)
{
	assert(t_value);
	assert(t_string);
	
	if (t_value->type == json_data_type_object)
	{
		if (!vector_push(t_string, "{"))
		{
			JSON_LOG("failed to push object open brace");
			return 0;
		}
		
		json_stringify_property_context context;
		context.list = &((p_hash_list)&t_value->value.blob->object)->pairs;
		context.string = t_string;
		link_list_for_each_with_context(context.list, json_for_each_object_property_stringify, &context);
		
		if (!json_push_string(t_string, "\n}", 0))
		{
			JSON_LOG("failed to push object close brace");
			return 0;
		}
	}
	else if (t_value->type == json_data_type_array)
	{
		if (!vector_push(t_string, "["))
		{
			JSON_LOG("failed to push array open brace");
			return 0;
		}
		
		vector_for_each_with_context((p_vector)&t_value->value.blob->array, json_for_each_array_property_stringify, t_string);
		
		if (!json_push_string(t_string, "\n]", 0))
		{
			JSON_LOG("failed to push array close brace");
			return 0;
		}
	}
	else if (t_value->type == json_data_type_string)
	{
		if (!vector_push(t_string, "\""))
		{
			JSON_LOG("failed to push open quote for string");
			return 0;
		}
		
		if (!json_push_string(t_string, (char*)t_value->value.blob->string.data, 1))
		{
			JSON_LOG("failed to stringify string");
			return 0;
		}
		
		if (!vector_push(t_string, "\""))
		{
			JSON_LOG("failed to push close quote for string");
			return 0;
		}
	}
	else if (t_value->type == json_data_type_boolean)
	{
		if (!json_push_string(t_string, t_value->value.boolean ? "true" : "false", 0))
		{
			JSON_LOG("failed to stringify boolean");
			return 0;
		}
	}
	else if (t_value->type == json_data_type_number)
	{
		char s[64];
		double d = t_value->value.number;
		if (sprintf(s, (d < 0.0001f || d > 10000.0f) ? "%.10e" : "%.10f", t_value->value.number) <= 0)
		{
			JSON_LOG("faled to sprintf a number");
			return 0;
		}
		if (!json_push_string(t_string, s, 0))
		{
			JSON_LOG("failed to stringify number");
			return 0;
		}
	}
	else
	{
		if (!json_push_string(t_string, "null", 0))
		{
			JSON_LOG("failed to stringify null");
			return 0;
		}
	}
	
	return 1;
}

int json_stringify(json_file* t_json, buffer* t_out_string)
{
	assert(t_json);
	assert(t_out_string);
	
	JSON_LOG("begin stringify");
	
	vector string;
	if (!vector_init(&string, sizeof(char)))
	{
		JSON_LOG("failed to initialize string vector");
		return 0;
	}
	
	json_value root = {0};
	root.type = json_data_type_object;
	root.value.blob = (p_json_blob)&t_json->root;
	
	if (!json_stringify_value(&root, &string))
	{
		vector_final(&string);
		JSON_LOG("failed to stringify root value");
		return 0;
	}
	
	*t_out_string = string.buffer;
	
	JSON_LOG("succeeded stringify");
	
	return 1;
}