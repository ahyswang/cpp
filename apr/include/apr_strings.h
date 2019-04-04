#ifndef APR_STRINGS_H
#define APR_STRINGS_H

#include "apr.h"

/**
 * �ַ����Ƚ�(��Сд����).
 * @param a ��һ���ַ���
 * @param b �ڶ����ַ���
 * @return	����0 ��ʾ��һ�����ڵڶ���
 *			����0 ��ʾ�ַ������
 *			С��0 ��ʾ��һ��С�ڵڶ���
 */
APR_DECLARE(int) apr_strcmp(const char* a, const char* b);

/**
 * �ַ����Ƚ�(��Сд������).
 * @param a ��һ���ַ���
 * @param b �ڶ����ַ���
 * @return	����0 ��ʾ��һ�����ڵڶ���
 *			����0 ��ʾ�ַ������
 *			С��0 ��ʾ��һ��С�ڵڶ���
 */
APR_DECLARE(int) apr_stricmp(const char* a, const char* b);

/**
 * �����ַ������½��ڴ棩
 * @param s �����Ƶ��ַ���
 * @return ����NULL ��ʾ����ʧ��
 */
APR_DECLARE(char *) apr_strdup(const char *src);

/**
 * ��ʽ���ַ���
 * @param fmt ��ʽ����
 * @param ... ��ʽ�������б�
 * @return ���ظ�ʽ�����ַ���
 */
APR_DECLARE(char *) apr_asprintf(const char* fmt, ...);

/**
 * ��ʽ���ַ���
 * @param fmt ��ʽ����
 * @param ap  ��ʽ�������б�
 * @return ���ظ�ʽ�����ַ���
 */
APR_DECLARE(char *) apr_vasprintf(const char* fmt, va_list ap);

/**
 * �ַ����ָ�
 * @param str ���ָ���ַ���("abc|123")
 * @param sep �ָ��ַ�("|")
 * @param last �����´ηָ��λ��("123")
 * @return �ɹ����ر��ָ�����ַ���("abc")��ʧ�ܷ���NULL
 * @note ���ָ���ַ����ڴ�ᱻ����"\0",ע�ⲻ�����ַ�������������ᱼ��
 */
APR_DECLARE(char *) apr_strtok(char *str, const char *sep, char **last);

#endif /* !APR_STRINGS_H */