
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
//单向链表的节点
//每一个节点是一个数组，开辟一段空间
struct ngx_list_part_s {
    void             *elts;//该节点指向的内存
    ngx_uint_t        nelts;//已经使用的元素个数,不能大于ngx_list_t.nalloc
    ngx_list_part_t  *next;//下一个节点
};


//单向链表，和内存池的实现有点像
//第一个元素直接存储在结构体之中，而不是使用指针
typedef struct {
    ngx_list_part_t  *last;//最后一个数组元素
    ngx_list_part_t   part;//存放第一个数组元素的空间，注意，这里不是指针
    size_t            size;//每个数组元素占用的空间大小
    ngx_uint_t        nalloc;//每个块的容量
    ngx_pool_t       *pool;//分配内存时使用的pool
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

//内部函数，不能被外部程序使用
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);//给第一个节点里面的数组分配空间
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;//指向自己
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
