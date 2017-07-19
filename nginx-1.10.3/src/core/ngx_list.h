
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

//scr/core/Ngx_list.h
//��������Ľڵ�
//ÿһ���ڵ���һ�����飬����һ�οռ�
struct ngx_list_part_s {
    void             *elts;//�ýڵ�ָ����ڴ�
    ngx_uint_t        nelts;//�Ѿ�ʹ�õ�Ԫ�ظ���,���ܴ���ngx_list_t.nalloc
    ngx_list_part_t  *next;//��һ���ڵ�
};


//�����������ڴ�ص�ʵ���е���
//��һ��Ԫ��ֱ�Ӵ洢�ڽṹ��֮�У�������ʹ��ָ��
typedef struct {
    ngx_list_part_t  *last;//���һ������Ԫ��
    ngx_list_part_t   part;//��ŵ�һ������Ԫ�صĿռ䣬ע�⣬���ﲻ��ָ��
    size_t            size;//ÿ������Ԫ��ռ�õĿռ��С
    ngx_uint_t        nalloc;//ÿ���������
    ngx_pool_t       *pool;//�����ڴ�ʱʹ�õ�pool
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

//�ڲ����������ܱ��ⲿ����ʹ��
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);//����һ���ڵ�������������ռ�
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;//ָ���Լ�
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
