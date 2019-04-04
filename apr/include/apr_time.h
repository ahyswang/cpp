#ifndef APR_TIME_H
#define APR_TIME_H

#include "apr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/**
	 * ��ȡ��ǰ��ʱ�䡣
	 * �����Լ�Ԫ Epoch��1970-01-01 00:00:00 UTC���𾭹���ʱ�䣬��΢��Ϊ��λ��
	 * @return ���ص�ǰʱ�䣨΢�룩
	 */
	APR_DECLARE(uint64_t) apr_time_now(void);

	/**
	 * ����ָ����ʱ�䣨΢�룩��
	 * @param t ����ʱ��
	 */
	APR_DECLARE(void) apr_sleep(uint64_t t);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APR_TIME_H */