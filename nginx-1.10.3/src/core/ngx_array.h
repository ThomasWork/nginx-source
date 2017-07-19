
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    void        *elts;//������׵�ַ
    ngx_uint_t   nelts;//�����е�ǰʹ���˶��ٸ�Ԫ��
    size_t       size;//ÿ��Ԫ�ص�ռ�����ֽ�
    ngx_uint_t   nalloc;//��ǰ�����˶��ٸ�Ԫ�صĿռ䣬���nelts�����˸�ֵ�������ݣ���vector����
    ngx_pool_t  *pool;//�ڴ�ض���
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
void ngx_array_destroy(ngx_array_t *a);
void *ngx_array_push(ngx_array_t *a);
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);

//���Զ������͵�������г�ʼ��
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
//������array->elts֮ǰ����array->nelts��
//��ȻMSVC����Ϊ"array->nelts" ����δ����ʼ���ͱ�ʹ��
    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
