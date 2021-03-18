
/**
 * data_structures.c
 */

#include "data.h"
#include <assert.h>

void buffer_init(buffer* t_buffer, size_t t_size)
{
	assert(t_buffer && t_size);

	t_buffer->data = malloc(t_size);
	t_buffer->size = t_size;
}

void buffer_resize(buffer* t_buffer, size_t t_size)
{
	assert(t_buffer && t_size);

	t_buffer->data = realloc(t_buffer->data, t_size);
	t_buffer->size = t_size;
}

void buffer_final(buffer* t_buffer)
{
	assert(t_buffer);

	free(t_buffer->data);
	t_buffer->data = 0;
	t_buffer->size = 0;
}

void vector_init(vector* t_vector, size_t t_element_size)
{
	assert(t_vector && t_element_size);
	
	buffer_init(&t_vector->buffer, t_element_size);
	t_vector->element_size = t_element_size;
	t_vector->element_count = 0;
}

unsigned int vector_find_capacity(vector* t_vector)
{
	return t_vector->buffer.size / t_vector->element_size;
}

void vector_resize(vector* t_vector, unsigned int t_new_element_count)
{
	assert(t_vector && t_new_element_count);
	
	buffer_resize(&t_vector->buffer, t_new_element_count * t_vector->element_size);
}

void vector_grow(vector* t_vector, unsigned int t_new_element_count)
{
	assert(t_vector && t_new_element_count);

	if (vector_find_capacity(t_vector) < t_new_element_count)
	{
		vector_resize(t_vector, t_new_element_count);
	}
}

void vector_final(vector* t_vector)
{
	assert(t_vector);

	buffer_final(&t_vector->buffer);
	t_vector->element_size = 0;
	t_vector->element_count = 0;
}

void* vector_get_index(vector* t_vector, unsigned int t_index)
{
	assert(t_vector && t_index && vector_find_capacity(t_vector) > t_index);

	return ((char*)t_vector->buffer.data) + (t_index * t_vector->element_size);
}

void vector_push(vector* t_vector, const void* t_data)
{
	assert(t_vector && t_data);

	vector_grow(t_vector, t_vector->element_count + 1);
	memcpy(vector_get_index(t_vector, t_vector->element_count), t_data, t_vector->element_count);
	--t_vector->element_size;
}

void vector_remove(vector* t_vector, unsigned int t_index)
{
	assert(t_vector && vector_find_capacity(t_vector) > t_index);

	--t_vector->element_count;
	if (t_index != t_vector->element_count - 1)
	{
		char* dest = (char*)vector_get_index(t_vector, t_index);
		char* source = dest + t_vector->element_size;
		memmove(dest, source, (t_vector->element_count - t_index) * t_vector->element_size);
	}
}

void link_list_init(link_list* t_list)
{
	assert(t_list);

	t_list->end.data = 0;
	t_list->end.next = &t_list->end;
	t_list->end.prev = &t_list->end;
}

p_link link_list_insert(p_link t_position, void* t_data)
{
	assert(t_position);

	p_link link = (p_link)malloc(sizeof(link));

	link->data = t_data;
	link->prev = t_position;
	link->next = t_position->next;
	((p_link)t_position->next)->prev = link;
	t_position->next = link;

	return link;
}

void link_list_move(p_link t_position, p_link t_link)
{
	assert(t_position && t_link);
	
	((p_link)t_link->next)->prev = t_link->prev;
	((p_link)t_link->prev)->next = t_link->next;
	t_link->prev = t_position;
	t_link->next = t_position->next;
	((p_link)t_position->next)->prev = t_link;
	t_position->next = t_link;
}

void link_list_remove(p_link t_link)
{
	assert(t_link);

	((p_link)t_link->next)->prev = t_link->prev;
	((p_link)t_link->prev)->next = t_link->next;

	free(t_link);
}

void link_list_final(link_list* t_list)
{
	assert(t_list);

	p_link link = (p_link)t_list->end.next;
	for (; link != &t_list->end; link = (p_link)t_list->end.prev)
	{
		link_list_remove(link);
	}
}

unsigned char hash_string(const char* t_string)
{
	assert(t_string);

	unsigned char hash = 7;
	const char* c = t_string;
	for (; *c != '\0'; ++c)
	{
		hash = ((hash * 31) + *c) & 0xff;
	}
	return hash;
}

p_hash_pair hash_pair_alloc(const char* t_key, void* t_data)
{
	assert(t_key);

	p_hash_pair pair = (p_hash_pair)malloc(sizeof(hash_pair) + strlen(t_key));
	char* key = (char*)(pair + 1);
	unsigned char hash = hash_string(t_key);
	strcpy(key, t_key);
	hash_pair temp = {hash, key, t_data};
	memcpy(&temp, pair, sizeof(hash_pair));

	return pair;
}

void hash_pair_free(p_hash_pair t_pair)
{
	assert(t_pair);

	free(t_pair);
}

void hash_list_init(hash_list* t_list)
{
	assert(t_list);

	link_list_init(&t_list->pairs);
	
	int c = 0;
	for (; c < 256; ++c)
	{
		t_list->buckets[c] = &t_list->pairs.end;
	}
}

void hash_list_final(hash_list* t_list)
{
	assert(t_list);

	link_list_final(&t_list->pairs);

	int c = 0;
	for (; c < 256; ++c)
	{
		t_list->buckets[c] = 0;
	}
}

p_link hash_list_find(hash_list* t_list, const char* t_key)
{
	assert(t_list && t_key);

	unsigned char hash = hash_string(t_key);
	p_link link = t_list->buckets[hash];
	p_hash_pair pair = (p_hash_pair)link->data;

	for (; link != &t_list->pairs.end && pair->hash == hash; link = (p_link)link->next)
	{
		pair = (p_hash_pair)link->data;
		if (strcmp(t_key, pair->key) == 0)
		{
			return link;
		}
	}

	return 0;
}

p_link hash_list_insert(hash_list* t_list, const char* t_key, void* t_data)
{
	assert(t_list && t_key && !hash_list_find(t_list, t_key));

	p_hash_pair pair = hash_pair_alloc(t_key, t_data);
	unsigned char hash = hash_string(t_key);

	p_link prev = t_list->buckets[hash];
	p_link link = link_list_insert(prev, pair);
	if (prev == &t_list->pairs.end)
	{
		t_list->buckets[hash] = link;
	}

	return link;
}

void hash_list_remove(hash_list* t_list, p_link t_link)
{
	assert(t_link);

	p_hash_pair pair = (p_hash_pair)t_link->data;
	if (t_link == t_list->buckets[pair->hash])
	{
		p_link next = (p_link)t_link->next;
		p_hash_pair next_pair = (p_hash_pair)next->data;
		t_list->buckets[pair->hash] = (next_pair->hash == pair->hash) ? next : &t_list->pairs.end;
	}
	
	hash_pair_free(pair);

	link_list_remove(t_link);
}

void factory_init(factory* t_factory, size_t t_block_size, size_t t_alloc_capacity)
{
	assert(t_factory && t_block_size && t_alloc_capacity);
	
	link_list_init(&t_factory->alloc);
	vector_init(&t_factory->free, sizeof(void*));
	t_factory->block_size = t_block_size;
	t_factory->alloc_capacity = t_alloc_capacity;
}

void factory_final(factory* t_factory)
{
	assert(t_factory);
	
	p_link link = &t_factory->alloc.end;
	do
	{
		link = (p_link)link->next;
		free(link->data);
	}
	while (link != &t_factory->alloc.end);
}

void* factory_alloc(factory* t_factory)
{
	assert(t_factory);
	
	void* output = 0;
	unsigned int i = 0;
	if (t_factory->free.element_count > 0)
	{
		i = t_factory->free.element_count - 1;
		output = vector_get_index(&t_factory->free, i);
		vector_remove(&t_factory->free, i);
		return output;
	}
	output = malloc(t_factory->block_size * t_factory->alloc_capacity);
	char* chunk = (char*)output;
	assert(output);
	
	link_list_insert(&t_factory->alloc.end, output);
	i = 1;
	for (; i < t_factory->alloc_capacity; ++i)
	{
		vector_push(&t_factory->free, chunk + (i * t_factory->block_size));
	}
	return output;
}

void factory_free(factory* t_factory, void* t_memptr) 
{
	assert(t_factory && t_memptr);
	
	vector_push(&t_factory->free, t_memptr);
}
