
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"


typedef struct avl_node avl_node_gt;
typedef struct avl_tree avl_tree_gt;
typedef avl_itr_gt avl_itr_gt;

typedef int (*cmp_func_gt)(unsigned long, unsigned long);

struct avl_node
{
    unsigned long key;

    struct
    {
        struct avl_node* p[2];
        signed char      balance; /* balance factor */
        unsigned         size;    /* #elements in subtree */
    } head;
};

struct avl_tree
{
    avl_node_gt* root;
    cmp_func_gt  cmp;
};

int avl_tree_create(avl_tree_gt** tree, cmp_func_gt cmp)
{
    *tree = (avl_tree_gt*) malloc(sizeof(avl_tree_gt));

    if (NULL == *tree)
    {
        return 0;
    }

    (void) memset(*tree, 0x0, sizeof(avl_tree_gt));

    (*tree)->cmp = cmp;

    return 1;
}

int avl_node_create(avl_node_gt** node, unsigned long key)
{
    *node = malloc(sizeof(avl_node_gt));

    if (NULL == node)
    {
        return 0;
    }

    memset(*node, 0x0, sizeof(avl_node_gt));

    (*node)->key = key;

    return 1;
}

inline static unsigned int avl_size_child(const avl_node_gt* node,
                                          unsigned int       index)
{
    return ((node)->head.p[(index)] ? (node)->head.p[(index)]->head.size : 0);
}

avl_node_gt* avl_find(avl_tree_gt* tree, unsigned long x, unsigned int* cnt)
{
    const avl_node_gt* p       = tree->root;
    unsigned           tmp_cnt = 0;

    while (NULL != p)
    {
        int cmp;
        cmp = tree->cmp(x, p->key);
        if (cmp >= 0)
        {
            tmp_cnt += avl_size_child(p, 0) + 1;
        }
        if (cmp < 0)
        {
            p = p->head.p[0];
        }
        else if (cmp > 0)
        {
            p = p->head.p[1];
        }
        else
        {
            break;
        }
    }
    if (cnt)
    {
        *cnt = tmp_cnt;
    }
    return (avl_node_gt*) p;
}

/* one rotation: (a,(b,c)q)p => ((a,b)p,c)q */
inline static avl_node_gt* avl_rotate1(avl_node_gt* p, int dir)
{                                  /* dir=0 to left; dir=1 to right */
    int          opp    = 1 - dir; /* opposite direction */
    avl_node_gt* q      = p->head.p[opp];
    unsigned     size_p = p->head.size;
    p->head.size -= q->head.size - avl_size_child(q, dir);
    q->head.size   = size_p;
    p->head.p[opp] = q->head.p[dir];
    q->head.p[dir] = p;
    return q;
}

/* two consecutive rotations: (a,((b,c)r,d)q)p => ((a,b)p,(c,d)q)r */
inline static avl_node_gt* avl_rotate2(avl_node_gt* p, int dir)
{
    int          b1;
    int          opp        = 1 - dir;
    avl_node_gt* q          = p->head.p[opp];
    avl_node_gt* r          = q->head.p[dir];
    unsigned     size_x_dir = avl_size_child(r, dir);
    r->head.size            = p->head.size;
    p->head.size -= q->head.size - size_x_dir;
    q->head.size -= size_x_dir + 1;
    p->head.p[opp] = r->head.p[dir];
    r->head.p[dir] = p;
    q->head.p[dir] = r->head.p[opp];
    r->head.p[opp] = q;
    b1             = dir == 0 ? +1 : -1;
    if (r->head.balance == b1)
    {
        q->head.balance = 0, p->head.balance = -b1;
    }
    else if (r->head.balance == 0)
    {
        q->head.balance = p->head.balance = 0;
    }
    else
    {
        q->head.balance = b1, p->head.balance = 0;
    }
    r->head.balance = 0;
    return r;
}

avl_node_gt* avl_insert(avl_tree_gt* tree, avl_node_gt* x, unsigned* cnt)
{
    unsigned char stack[KAVL_MAX_DEPTH];
    avl_node_gt*  path[KAVL_MAX_DEPTH];
    avl_node_gt*  bp;
    avl_node_gt*  bq;
    avl_node_gt*  p;
    avl_node_gt*  q;
    avl_node_gt*  r = 0; /* _r_ is potentially the new root */

    int      i;
    int      which = 0;
    int      top;
    int      b1;
    int      path_len;
    unsigned tmp_cnt = 0;

    bp = tree->root;
    bq = 0;
    /* find the insertion location */
    for (p = bp, q = bq, top = path_len = 0; p; q = p, p = p->head.p[which])
    {
        int cmp;
        cmp = tree->cmp(x->key, p->key);
        if (cmp >= 0)
        {
            tmp_cnt += avl_size_child(p, 0) + 1;
        }
        if (cmp == 0)
        {
            if (cnt)
            {
                *cnt = tmp_cnt;
            }
            return p;
        }
        if (p->head.balance != 0)
        {
            bq = q, bp = p, top = 0;
        }
        stack[top++] = which = (cmp > 0);
        path[path_len++]     = p;
    }
    if (cnt)
    {
        *cnt = tmp_cnt;
    }
    x->head.balance = 0, x->head.size = 1, x->head.p[0] = x->head.p[1] = 0;
    if (q == 0)
    {
        tree->root = x;
    }
    else
    {
        q->head.p[which] = x;
    }
    if (bp == 0)
    {
        return x;
    }
    for (i = 0; i < path_len; ++i)
    {
        ++path[i]->head.size;
    }
    for (p = bp, top = 0; p != x;
         p = p->head.p[stack[top]], ++top) /* update balance factors */
    {
        if (stack[top] == 0)
        {
            --p->head.balance;
        }
        else
        {
            ++p->head.balance;
        }
    }
    if (bp->head.balance > -2 && bp->head.balance < 2)
    {
        return x; /* no re-balance needed */
    }
    /* re-balance */
    which = (bp->head.balance < 0);
    b1    = which == 0 ? +1 : -1;
    q     = bp->head.p[1 - which];
    if (q->head.balance == b1)
    {
        r               = avl_rotate1(bp, which);
        q->head.balance = bp->head.balance = 0;
    }
    else
    {
        r = avl_rotate2(bp, which);
    }
    if (bq == 0)
    {
        tree->root = r;
    }
    else
    {
        bq->head.p[bp != bq->head.p[0]] = r;
    }
    return x;
}

avl_node_gt* avl_erase(avl_tree_gt* tree, const avl_node_gt* x, unsigned* cnt)
{
    avl_node_gt* p;
    avl_node_gt* path[KAVL_MAX_DEPTH];
    avl_node_gt  fake;

    unsigned char dir[KAVL_MAX_DEPTH];
    int           i;
    int           d = 0;
    int           cmp;
    unsigned      tmp_cnt = 0;

    fake.head.p[0] = tree->root;
    fake.head.p[1] = 0;
    if (cnt)
    {
        *cnt = 0;
    }
    if (x)
    {
        for (cmp = -1, p = &fake; cmp; cmp = tree->cmp(x->key, p->key))
        {
            int which = (cmp > 0);
            if (cmp > 0)
            {
                tmp_cnt += avl_size_child(p, 0) + 1;
            }
            dir[d]    = which;
            path[d++] = p;
            p         = p->head.p[which];
            if (p == 0)
            {
                if (cnt)
                {
                    *cnt = 0;
                }
                return 0;
            }
        }
        tmp_cnt += avl_size_child(p, 0) + 1; /* because p==x is not counted */
    }
    else
    {
        for (p = &fake, tmp_cnt = 1; p; p = p->head.p[0])
        {
            dir[d] = 0, path[d++] = p;
        }
        p = path[--d];
    }
    if (cnt)
    {
        *cnt = tmp_cnt;
    }
    for (i = 1; i < d; ++i)
    {
        --path[i]->head.size;
    }
    if (p->head.p[1] == 0)
    { /* ((1,.)2,3)4 => (1,3)4; p=2 */
        path[d - 1]->head.p[dir[d - 1]] = p->head.p[0];
    }
    else
    {
        avl_node_gt* q = p->head.p[1];
        if (q->head.p[0] == 0)
        { /* ((1,2)3,4)5 => ((1)2,4)5; p=3 */
            q->head.p[0]                    = p->head.p[0];
            q->head.balance                 = p->head.balance;
            path[d - 1]->head.p[dir[d - 1]] = q;
            path[d] = q, dir[d++] = 1;
            q->head.size = p->head.size - 1;
        }
        else
        { /* ((1,((.,2)3,4)5)6,7)8 => ((1,(2,4)5)3,7)8; p=6 */
            avl_node_gt* r;
            int          e = d++; /* backup _d_ */
            for (;;)
            {
                dir[d]    = 0;
                path[d++] = q;
                r         = q->head.p[0];
                if (r->head.p[0] == 0)
                {
                    break;
                }
                q = r;
            }
            r->head.p[0]                    = p->head.p[0];
            q->head.p[0]                    = r->head.p[1];
            r->head.p[1]                    = p->head.p[1];
            r->head.balance                 = p->head.balance;
            path[e - 1]->head.p[dir[e - 1]] = r;
            path[e] = r, dir[e] = 1;
            for (i = e + 1; i < d; ++i)
            {
                --path[i]->head.size;
            }
            r->head.size = p->head.size - 1;
        }
    }
    while (--d > 0)
    {
        avl_node_gt* q = path[d];
        int          which;
        int          other;
        int          b1 = 1;
        int          b2 = 2;
        which = dir[d], other = 1 - which;
        if (which)
        {
            b1 = -b1, b2 = -b2;
        }
        q->head.balance += b1;
        if (q->head.balance == b1)
        {
            break;
        }
        if (q->head.balance == b2)
        {
            avl_node_gt* r = q->head.p[other];
            if (r->head.balance == -b1)
            {
                path[d - 1]->head.p[dir[d - 1]] = avl_rotate2(q, which);
            }
            else
            {
                path[d - 1]->head.p[dir[d - 1]] = avl_rotate1(q, which);
                if (r->head.balance == 0)
                {
                    r->head.balance = -b1;
                    q->head.balance = b1;
                    break;
                }

                r->head.balance = q->head.balance = 0;
            }
        }
    }
    tree->root = fake.head.p[0];
    return p;
}

void avl_itr_first(const avl_tree_gt* tree, avl_itr_gt* itr)
{
    const avl_node_gt* p;
    for (itr->top = itr->stack - 1, p = tree->root; p; p = p->head.p[0])
    {
        *++itr->top = p;
    }
    itr->right = (*itr->top)->head.p[1];
}

int avl_itr_find(const avl_tree_gt* tree,
                 const avl_node_gt* x,
                 avl_itr_gt*    itr)
{
    const avl_node_gt* p = tree->root;
    itr->top             = itr->stack - 1;
    while (p != 0)
    {
        int cmp;
        cmp = tree->cmp(x->key, p->key);
        if (cmp < 0)
        {
            *++itr->top = p, p = p->head.p[0];
        }
        else if (cmp > 0)
        {
            p = p->head.p[1];
        }
        else
        {
            break;
        }
    }
    if (p)
    {
        *++itr->top = p;
        itr->right  = p->head.p[1];
        return 1;
    }
    if (itr->top >= itr->stack)
    {
        itr->right = (*itr->top)->head.p[1];
        return 0;
    }

    return 0;
}

int avl_itr_next(avl_itr_gt* itr)
{
    for (;;)
    {
        const avl_node_gt* p;
        for (p = itr->right, --itr->top; p; p = p->head.p[0])
        {
            *++itr->top = p;
        }
        if (itr->top < itr->stack)
        {
            return 0;
        }
        itr->right = (*itr->top)->head.p[1];
        return 1;
    }
}

const avl_node_gt* avl_at(avl_itr_gt* itr)
{
    return ((itr)->top < (itr)->stack ? NULL : *(itr)->top);
}

unsigned long avl_node_get_key(const avl_node_gt* node)
{
    return node->key;
}

unsigned int avl_size(const avl_node_gt* node)
{
    return ((node) ? (node)->head.size : 0);
}


