
/**
 * data_structures.c
 */

#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

int buffer_init(buffer* t_buffer, size_t t_size)
{
	assert(t_buffer);
	
	t_size = t_size;
	
	t_buffer->data = t_size ? malloc(t_size) : 0;
	t_buffer->size = t_size;
	
	return t_size ? (t_buffer->data ? 1 : 0) : 1;
}

void buffer_final(buffer* t_buffer)
{
	assert(t_buffer);

	free(t_buffer->data);
	t_buffer->data = 0;
	t_buffer->size = 0;
}

int buffer_resize(buffer* t_buffer, size_t t_size)
{
	assert(t_buffer);
	
	if (t_size == 0)
	{
		free(t_buffer->data);
		t_buffer->data = 0;
	}
	else
	{
		t_buffer->data = realloc(t_buffer->data, t_size);
	}
	t_buffer->size = t_size;
	
	return t_buffer->data ? 1 : 0;
}

int buffer_append(buffer* t_buffer, void* t_ptr, size_t t_size)
{
	assert(t_buffer);
	
	size_t offset = t_buffer->size;
	if (!buffer_resize(t_buffer, t_buffer->size + t_size))
	{
		return 0;
	}
	memcpy(((char*)t_buffer->data) + offset, t_ptr, t_size);
	
	return 1;
}

int vector_init(vector* t_vector, size_t t_element_size)
{
	assert(t_vector);
	assert(t_element_size);
	
	int result = buffer_init(&t_vector->buffer, t_element_size);
	t_vector->element_size = t_element_size;
	t_vector->element_count = 0;
	
	return result;
}

unsigned int vector_find_capacity(vector* t_vector)
{
	return t_vector->buffer.size / t_vector->element_size;
}

int vector_resize(vector* t_vector, unsigned int t_new_element_count)
{
	assert(t_vector);
	assert(t_new_element_count);
	
	int result = buffer_resize(&t_vector->buffer, t_new_element_count * t_vector->element_size);
	
	return result;
}

int vector_grow(vector* t_vector, unsigned int t_new_element_count)
{
	assert(t_vector);
	assert(t_new_element_count);
	
	int result = 1;
	unsigned int capacity = vector_find_capacity(t_vector);
	if (capacity < t_new_element_count)
	{
		result = vector_resize(t_vector, capacity * 2);
	}
	
	return result;
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
	assert(t_vector);
	assert(vector_find_capacity(t_vector) > t_index);

	return ((char*)t_vector->buffer.data) + (t_index * t_vector->element_size);
}

int vector_find(vector* t_vector, void* t_ptr)
{
	assert(t_vector);
	
	if (t_ptr < t_vector->buffer.data)
	{
		return -1;
	}
	int offset = (int)(((char*)t_ptr) - ((char*)t_vector->buffer.data));
	if (offset >= t_vector->buffer.size)
	{
		return -1;
	}
	return offset / t_vector->element_size;
}

int vector_push(vector* t_vector, const void* t_data)
{
	assert(t_vector);
	
	int result = vector_grow(t_vector, t_vector->element_count + 1);
	if (!result)
	{
		return result;
	}
	if (t_data)
	{
		memcpy(vector_get_index(t_vector, t_vector->element_count), t_data, t_vector->element_size);
	}
	else
	{
		memset(vector_get_index(t_vector, t_vector->element_count), 0, t_vector->element_size);
	}
	++t_vector->element_count;
	return 1;
}

void vector_remove(vector* t_vector, unsigned int t_index)
{
	assert(t_vector);
	assert(vector_find_capacity(t_vector) > t_index);

	if (t_index != t_vector->element_count - 1)
	{
		char* dest = (char*)vector_get_index(t_vector, t_index);
		char* source = dest + t_vector->element_size;
		memmove(dest, source, (t_vector->element_count - t_index) * t_vector->element_size);
	}
	--t_vector->element_count;
}

void vector_for_each(vector* t_vector, void (*t_for_each_func)(unsigned int, void*))
{
	assert(t_vector);
	assert(t_for_each_func);
	
	unsigned int i = 0;
	char* element = (char*)t_vector->buffer.data;
	
	for (; i < t_vector->element_count; ++i, element += t_vector->element_size)
	{
		t_for_each_func(i, element);
	}
}

void vector_for_each_with_context(p_vector t_vector, void (*t_for_each_func)(unsigned int, void*, void*), void* t_context)
{
	assert(t_vector);
	assert(t_for_each_func);
	
	unsigned int i = 0;
	char* element = (char*)t_vector->buffer.data;
	
	for (; i < t_vector->element_count; ++i, element += t_vector->element_size)
	{
		t_for_each_func(i, element, t_context);
	}
}

int link_list_init(link_list* t_list)
{
	assert(t_list);

	t_list->end = malloc(sizeof(link));
	t_list->end->data = 0;
	t_list->end->next = t_list->end;
	t_list->end->prev = t_list->end;
	
	return 1;
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

	p_link link = (p_link)t_list->end->next;
	for (; link != t_list->end; link = (p_link)t_list->end->prev)
	{
		link_list_remove(link);
	}
	free(t_list->end);
}

void link_list_for_each(p_link_list t_list, void (*t_for_each_func)(p_link))
{
	assert(t_list);
	assert(t_for_each_func);
	
	p_link link = (p_link)t_list->end->next;
	for (; link != t_list->end; link = (p_link)link->next)
	{
		t_for_each_func(link);
	}
}

void link_list_for_each_with_context(p_link_list t_list, void (*t_for_each_func)(p_link, void*), void* t_context)
{
	assert(t_list);
	assert(t_for_each_func);
	
	p_link link = (p_link)t_list->end->next;
	for (; link != t_list->end; link = (p_link)link->next)
	{
		t_for_each_func(link, t_context);
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
	memcpy(pair, &temp, sizeof(hash_pair));

	return pair;
}

void hash_pair_free(p_hash_pair t_pair)
{
	assert(t_pair);

	free(t_pair);
}

int hash_list_init(hash_list* t_list)
{
	assert(t_list);
	
	if (!link_list_init(&t_list->pairs))
	{
		return 0;
	}
	
	int c = 0;
	for (; c < 256; ++c)
	{
		t_list->buckets[c] = t_list->pairs.end;
	}
	
	return 1;
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

	for (; link != t_list->pairs.end && pair->hash == hash; link = (p_link)link->next)
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
	if (!pair)
	{
		return 0;
	}
	
	p_link prev = t_list->buckets[pair->hash];
	p_link link = link_list_insert(prev, pair);
	if (prev == t_list->pairs.end)
	{
		t_list->buckets[pair->hash] = link;
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
		t_list->buckets[pair->hash] = (next_pair->hash == pair->hash) ? next : t_list->pairs.end;
	}
	hash_pair_free(pair);
	link_list_remove(t_link);
}

int factory_init(factory* t_factory, size_t t_block_size, size_t t_alloc_capacity)
{
	assert(t_factory);
	assert(t_block_size);
	assert(t_alloc_capacity);
	
	if (!vector_init(&t_factory->alloc, sizeof(void*)))
	{
		return 0;
	}
	if (!vector_init(&t_factory->free, sizeof(void*)))
	{
		vector_final(&t_factory->alloc);
		return 0;
	}
	t_factory->block_size = t_block_size;
	t_factory->alloc_capacity = t_alloc_capacity;
	return 1;
}

void factory_final(factory* t_factory)
{
	assert(t_factory);
	
	unsigned int i = 0;
	for (; i < t_factory->alloc.element_count; ++i)
	{
		free(*((void**)vector_get_index(&t_factory->alloc, i)));
	}
	
	vector_final(&t_factory->free);
	vector_final(&t_factory->alloc);
}

void* factory_alloc(factory* t_factory)
{
	assert(t_factory);
	
	void* output = 0;
	unsigned int i = 0;
	if (t_factory->free.element_count)
	{
		i = t_factory->free.element_count - 1;
		output = (*(void**)vector_get_index(&t_factory->free, i));
		vector_remove(&t_factory->free, i);
		return output;
	}
	output = malloc(t_factory->block_size * t_factory->alloc_capacity);
	char* chunk = (char*)output;
	if (!output)
	{
		return 0;
	}
	if (!vector_push(&t_factory->alloc, &chunk))
	{
		free(output);
		return 0;
	}
	i = 1;
	for (; i < t_factory->alloc_capacity; ++i)
	{
		chunk += t_factory->block_size;
		int result = vector_push(&t_factory->free, &chunk);
		
		if (!result)
		{
			free(output);
			return 0;
		}
	}
	return output;
}

int factory_free(factory* t_factory, void* t_memptr) 
{
	assert(t_factory && t_memptr);
	
	int result = vector_push(&t_factory->free, t_memptr);
	
	return result;
}
