#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 

// WRONG FUNCTION
char *stringduplicator(char *s, int times) { //Convention error: should be stringDuplicator, s should be str, { should be line down
    assert(!s); // Should not use assert to handle errors
    assert(times > 0); 
    int LEN = strlen(*s);//Convention error: variables names in small laters
    char *out = malloc(LEN * times); //Should use size of like so: malloc(sizeof(char)*LEN * times)
    assert(out); // Should not use assert to handle errors
    for (int i = 0; i < times; i++) { //Convention error: code inside loop should be indented 
    out = out + LEN; // Should write to out before doing +LEN
    strcpy(out, s); 
    } 
    return out; // Returns pointer to NULL
}

// CORRECT FUNCTION
char *stringDuplicator(char *str, int times) 
{
    if(!str){
        return NULL;
    }
    assert(times > 0);
    int len = strlen(*str);
    char *out = malloc(sizeof(char)* len * times);
    if(!out){
        return NULL;
    }
    for (int i = 0; i < times; i+=len){
        strcpy(out+i, str);
    }
    return out;
}
