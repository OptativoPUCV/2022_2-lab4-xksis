#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  Pair *newPair = createPair(key,value);
  size_t i = hash(key,map->capacity);
  while(1){
    if(map->buckets[i] == NULL || map->buckets[i]->key == NULL){
      map->buckets[i] = newPair;
      map->size++;
      map->current = i;
      break;
    }
    i++;
    if(i == map->capacity){
      i = 0;
    }
  }

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair **bucketaux = map->buckets;
  size_t i;
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity,sizeof(Pair *));
  map->size = 0;
  for (i = 0; i < map->capacity; i++)
  {
    if(bucketaux[i] != NULL)
    {
      insertMap(map,bucketaux[i]->key,bucketaux[i]->value);
    }
  }
}


HashMap * createMap(long capacity) {
  HashMap *mapa= (HashMap*)malloc(sizeof(HashMap));
  mapa->buckets = (Pair**)calloc(capacity,sizeof(Pair*));
  mapa->size=0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
  Pair *Eliminado =searchMap(map,key);
  if(Eliminado != NULL)
  {
    Eliminado->key = NULL;
    map->size--;
  }
  
}

Pair * searchMap(HashMap * map,  char * key) {   
  size_t i = hash(key, map->capacity);
  size_t cont = 0;
  
  while(1){
    if(map->buckets[i] == NULL || map->buckets[i]->key == NULL)return NULL;
    if(is_equal(key, map->buckets[i]->key))
    {
      map->current = i;
      return map->buckets[i];
    }
    i++;
    cont++;
    if(cont == map->capacity)break;
    if(i == map->capacity)
    {
      i = 0;
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  size_t i;
  for(i=0; i < map->size; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  size_t i;
  for(i = map->current; i < map->capacity; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL && i != map->current)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
