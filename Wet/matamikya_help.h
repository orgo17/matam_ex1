#ifndef MATAMIKYA_HELP_H_
#define MATAMIKYA_HELP_H_

#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\