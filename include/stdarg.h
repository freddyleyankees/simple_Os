#ifndef _STDARG_H
#define _STDARG_H
#include "./va_list.h"

#define VA_SIZE(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))


#define start_arg(AP, LASTARG) 						\
 (AP = ((va_list) &(LASTARG) + VA_SIZE (LASTARG)))


void end_arg (va_list);
#define end_arg(AP)

#define get_arg(AP, TYPE)						\
 (AP += VA_SIZE (TYPE),					\
  *((TYPE *) (AP - VA_SIZE (TYPE))))

#endif