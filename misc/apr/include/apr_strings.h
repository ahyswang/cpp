#ifndef APR_STRINGS_H
#define APR_STRINGS_H

#include "apr.h"

/**
 * 字符串比较(大小写敏感).
 * @param a 第一个字符串
 * @param b 第二个字符串
 * @return	大于0 表示第一个大于第二个
 *			等于0 表示字符串相等
 *			小于0 表示第一个小于第二个
 */
APR_DECLARE(int) apr_strcmp(const char* a, const char* b);

/**
 * 字符串比较(大小写不敏感).
 * @param a 第一个字符串
 * @param b 第二个字符串
 * @return	大于0 表示第一个大于第二个
 *			等于0 表示字符串相等
 *			小于0 表示第一个小于第二个
 */
APR_DECLARE(int) apr_stricmp(const char* a, const char* b);

/**
 * 复制字符串（新建内存）
 * @param s 被复制的字符串
 * @return 返回NULL 表示复制失败
 */
APR_DECLARE(char *) apr_strdup(const char *src);

/**
 * 格式化字符串
 * @param fmt 格式化串
 * @param ... 格式化参数列表
 * @return 返回格式化的字符串
 */
APR_DECLARE(char *) apr_asprintf(const char* fmt, ...);

/**
 * 格式化字符串
 * @param fmt 格式化串
 * @param ap  格式化参数列表
 * @return 返回格式化的字符串
 */
APR_DECLARE(char *) apr_vasprintf(const char* fmt, va_list ap);

/**
 * 字符串分割
 * @param str 被分割的字符串("abc|123")
 * @param sep 分割字符("|")
 * @param last 用于下次分割的位置("123")
 * @return 成功返回被分割出的字符串("abc")，失败返回NULL
 * @note 本分割的字符串内存会被增加"\0",注意不能是字符串常量，否则会奔溃
 */
APR_DECLARE(char *) apr_strtok(char *str, const char *sep, char **last);

#endif /* !APR_STRINGS_H */