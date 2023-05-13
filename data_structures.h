
/**
 * data_structures.h
 */

#ifndef C_UTILS_DATA_STRUCTURES_H
#define C_UTILS_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	size_t size;
	void* data;

} buffer, *p_buffer;

/* initializes a buffer */
int buffer_init(buffer* t_buffer, size_t t_size);

/* finalizes a buffer */
void buffer_final(buffer* t_buffer);

/* resizes a buffer */
int buffer_resize(buffer* t_buffer, size_t t_size);

/* buffer append */
int buffer_append(buffer* t_buffer, void* t_ptr, size_t t_size);

typedef struct
{
	buffer buffer;
	size_t element_size;
	unsigned int element_count;
} vector, *p_vector;

/* initialize a vector */
int vector_init(vector* t_vector, size_t t_element_size);

/* finalize a vector */
void vector_final(vector* t_vector);

/* find the capcity of a vector */
unsigned int vector_find_capacity(p_vector t_vector);

/* resizes a vector */
int vector_resize(p_vector t_vector, unsigned int t_new_element_count);

/* resizes a vector, only if it needs to grow */
int vector_grow(p_vector t_vector, unsigned int t_new_element_count);

/* gets the pointer of an index in a vector */
void* vector_get_index(p_vector t_vector, unsigned int t_index);

/* finds the index of a given pointer */
int vector_find(p_vector t_vector, void* t_ptr);

/* pushes an entry into a vector */
int vector_push(p_vector t_vector, const void* t_data);

/* removes an entry at index from a vector */
void vector_remove(p_vector t_vector, unsigned int t_index);

/* performs a function on each entry of a vector */
void vector_for_each(p_vector t_vector, void (*t_for_each_func)(unsigned int t_index, void* t_element));

/* performs a function on each entry of a vector */
void vector_for_each_with_context(p_vector t_vector, void (*t_for_each_func)(unsigned int t_index, void* t_element, void* t_context), void* t_context);

typedef struct
{
	void* data;
	void* prev;
	void* next;

} link, *p_link;

typedef struct
{
	p_link end;

} link_list, *p_link_list;

/* initialize a link list */
int link_list_init(link_list* t_list);

/* finalize a link list */
void link_list_final(link_list* t_list);

/* insert a link in a link list at a location */
p_link link_list_insert(p_link t_position, void* t_data);

/* move a link in a link list to a new location */
void link_list_move(p_link t_position, p_link t_link);

/* remove the provided link from its link list */
void link_list_remove(p_link t_link);

/* performs a function on each entry of a list */
void link_list_for_each(p_link_list, void (*t_for_each_func)(p_link t_link));

/* performs a function on each entry of a list with context */
void link_list_for_each_with_context(p_link_list t_list, void (*t_for_each_func)(p_link t_link, void* t_context), void* t_context);

typedef struct
{
	const unsigned char hash;
	const char* const key;
	void* data;

} hash_pair, *p_hash_pair;

typedef struct
{
	link_list pairs;
	p_link buckets[256];

} hash_list, *p_hash_list;

/* hash functions */
unsigned char hash_string(const char* t_string);

/* allocates and initialize a hash pair */
p_hash_pair hash_pair_alloc(const char* t_key, void* t_data);

/* finalizes and frees a hash pair */
void hash_pair_free(p_hash_pair t_pair);

/* initialize hash list */
int hash_list_init(hash_list* t_list);

/* finalize hash list */
void hash_list_final(hash_list* t_list);

/* find an entry in a hash_list */
p_link hash_list_find(p_hash_list t_list, const char* t_key);

/* insert an entry into a hash list */
p_link hash_list_insert(p_hash_list t_list, const char* t_key, void* t_data);

/* remove an entry from a hash list */
void hash_list_remove(p_hash_list t_list, p_link t_link);

typedef struct
{
	vector alloc;
	vector free;
	size_t block_size;
	size_t alloc_capacity;
} factory, *p_factory;

/* initializes a memory factory of given element size and allocation capacity */
int factory_init(factory* t_factory, size_t t_block_size, size_t t_alloc_capacity);

/* finalizes a memory factory */
void factory_final(factory* t_factory);

/* allocates an element from the factory, allocating more memory for the factory if necassary */
void* factory_alloc(p_factory t_factory);

/* returns a factory element to the factories list of freed elements */
int factory_free(p_factory t_factory, void* t_memptr);

#endif