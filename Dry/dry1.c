#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 

// WRONG FUNCTION
char *stringduplicator(char *s, int times) { /*Convention error: should be stringDuplicator, 
s should have a more meaningful name like str, and { should be a line below */
    assert(!s); //Coding error: Should not use assert to handle errors
    assert(times > 0); 
    int LEN = strlen(*s);/*Convention error: variable names in small laters
    Coding Error: strlen should accept the string s and not *s */
    char *out = malloc(LEN * times); //Coding error: no memory allocated for \0 at the end
    assert(out); //Coding error: Should not use assert to handle errors
    for (int i = 0; i < times; i++) { //Convention error: code inside loop should be indented 
    out = out + LEN; //Coding error: Should copy s to out before doing +LEN
    strcpy(out, s); 
    } 
    return out; //Coding error: Returns pointer to NULL
}

// CORRECT FUNCTION
char *stringDuplicator(char *str, int times) 
{
    if(!str){
        return NULL;
    }
    assert(times > 0);
    int len = strlen(str);
    char *out = malloc((len * times)+1);
    if(!out){
        return NULL;
    }
    for (int i = 0; i < times; i+=len){
        strcpy(out+i, str);
    }
    return out;
}
