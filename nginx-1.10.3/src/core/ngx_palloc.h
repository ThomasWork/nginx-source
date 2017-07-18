
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
 //NGX_MAX_ALLOC_FROM_POOL 应该是ngx_pagesize-1，也就是x86平台上的4095
 //
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)// 默认内存块大小为16KB

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;//下一个节点
    void                 *alloc;//指向实际的存储空间
};

//内存池中的数据块
typedef struct {
    u_char               *last;//当前已经使用内存的末尾，下一次分配从此开始
    u_char               *end;//内存块结束位置
    ngx_pool_t           *next;//指向下一块内存
    ngx_uint_t            failed;//分配失败次数
} ngx_pool_data_t;

//内存池头部结构
struct ngx_pool_s {
    ngx_pool_data_t       d;//第一个数据块?
    size_t                max;//数据块的数量?
    ngx_pool_t           *current;//当前内存块
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;//指向保存大块内存的节点
    ngx_pool_cleanup_t   *cleanup;//指向需要清理的节点
    ngx_log_t            *log;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
