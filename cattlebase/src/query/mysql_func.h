#ifndef __MYSQL_FUNC_H__ 
#define __MYSQL_FUNC_H__

#define escape_string(s1,s2,ll) escape_string_for_FB(s1, 0, s2, ll);

typedef unsigned long ulong  ;
typedef char my_bool ;

ulong escape_string_for_FB(char *to, ulong to_length,
                               const char *from, ulong length);


#endif