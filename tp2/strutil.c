//#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L

// #define _GNU_SOURCE

#include "strutil.h"
#include <stdlib.h>
#include <stdio.h>

#include <string.h>


char* empty_string(void);

char *substr (const char* str, size_t n){
	char* new_str = calloc(n+1, sizeof(char));
	if(!new_str)
		return NULL;
	if(n >= strlen(str)){
		strcpy(new_str, str);
		return new_str;
	}
	strncpy(new_str, str, n);
	return new_str;
}

char **split(const char* str, char sep){
	size_t n_sep = 0;
	if(!str)
		return NULL;

	for(size_t i = 0; i<strlen(str); i++){
		if(str[i] == sep)
			n_sep++;
	}
	char** strv = malloc(sizeof(char*) * (n_sep+2));
	if(!strv)
		return NULL;
	if(n_sep == 0 || sep == '\0'){
		strv[0] = strdup(str);
		strv[1] = NULL;
		return strv;
	}
	n_sep = 0;
	size_t index = 0;
	for(size_t i = 0; i<=strlen(str); i++){
		if(str[i] == sep || str[i] == '\0'){
			strv[n_sep] = strndup(str+index, i-index);
			index = i+1;
			n_sep++;
		}
	}
	strv[n_sep] = NULL;
	return strv;
}

char *join(char **strv, char sep){
	size_t size = 0;
	if(!strv)
		return empty_string();
	for(size_t i = 0; strv[i]!=NULL; i++){
		size += strlen(strv[i]);
		size++;
	}
	if(size==0)
		return empty_string();
	char* str = calloc(size, sizeof(char));
	if(!str)
		return NULL;
	char cat[2] = {sep, '\0'};
	size = 0;
	for(size_t i = 0; strv[i]; i++){
		sprintf(str+size, "%s", strv[i]);
		size += strlen(str+size);
		if(strv[i+1])
			sprintf(str+size, "%s", cat);
		if(sep!='\0')
			size++;
	}
	str[strlen(str)] = '\0'; 
	return str;
}

void free_strv(char *strv[]){
	if(!strv)
		return;
	for(size_t i = 0; strv[i]; i++)
		free(strv[i]);
	free(strv);
}

char* empty_string(void){
	char* str = malloc(sizeof(char));
	str[0] = '\0';
	return str;
}
