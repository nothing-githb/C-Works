/**
 * avl tree implementation
 * 
 * int _cmp(unsigned long p1, unsigned long p2)
 * {
 *   return ((p2 < p1) - (p1 < p2));
 * }
 * int main()
 * {
 *      unsigned long array[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
 *      avl_tree_gt* tree;
 *      avl_tree_create(&tree, _cmp);
 *      int             i;
 *      for (i = 0; i < 9; ++i)
 *      {   // insert in the input order
 *          avl_node_gt *q;
 *          avl_node_gt *p;
 *          avl_node_create(&p, array[i]);
 *          q = avl_insert(tree, p, 0);
 *      }
 *      avl_itr_gt itr;
 *      avl_itr_first(tree, &itr);   // place at first
 *      do
 *      {   // traverse
 *          const avl_node_gt* p = avl_at(&itr);
 *          printf("%lu\n", avl_node_get_key(p));
 *      } while (avl_itr_next(&itr));
 *      putchar('\n');
 *      return 0;
 *  }
 */

#ifndef AVL_H
#define AVL_H

#define KAVL_MAX_DEPTH     64

/* forward declarations */

typedef struct avl_node avl_node_gt;
typedef struct avl_tree avl_tree_gt;
typedef struct avl_itr avl_itr_gt;
typedef int (*cmp_func_gt)(unsigned long, unsigned long);

struct avl_itr
{
    const avl_node_gt *stack[KAVL_MAX_DEPTH], **top,
        *right; /* _right_ points to the right child of *top */
};

/* function prototypes */

int avl_tree_create(avl_tree_gt** tree, cmp_func_gt cmp);

int avl_node_create(avl_node_gt** node, unsigned long key);

/**
 * Find a node in the tree
 *
 * @param tree [in]  avl tree
 * @param x    [in]  value to find
 * @param cnt  [out] number of nodes smaller than or equal to _x_; can be NULL
 *
 * @return node equal to key @p x if present, or NULL if absent
 */
avl_node_gt* avl_find(avl_tree_gt* tree, unsigned long x, unsigned int* cnt);

/**
 * Insert a node to the tree
 *
 * @param tree pointer to the root of the tree (in/out: root may change)
 * @param x    node to insert (in)
 * @param cnt  number of nodes smaller than or equal to _x_; can be NULL
 * (out)
 *
 * @return _x_ if not present in the tree, or the node equal to x.
 */
avl_node_gt* avl_insert(avl_tree_gt* tree, avl_node_gt* x, unsigned* cnt);

/**
 * Delete a node from the tree
 *
 * @param proot   pointer to the root of the tree (in/out: root may change)
 * @param x       node value to delete; if NULL, delete the first node (in)
 * 
 * @note #define avl_erase_first ->     avl_erase(proot, NULL, NULL)
 *
 * @return node removed from the tree if present, or NULL if absent
 */
avl_node_gt* avl_erase(avl_tree_gt* tree, const avl_node_gt* x, unsigned* cnt);

/**
 * Place the iterator at the smallest object
 *
 * @param root    root of the tree
 * @param itr     iterator
 */
void avl_itr_first(const avl_tree_gt* tree, struct avl_itr* itr);

/**
 * Place the iterator at the object equal to or greater than the query
 *
 * @param root    root of the tree
 * @param x       query (in)
 * @param itr     iterator (out)
 *
 * @return 1 if find; 0 otherwise. kavl_at(itr) is NULL if and only if query is
 *         larger than all objects in the tree
 */
int avl_itr_find(const avl_tree_gt* tree,
                 const avl_node_gt* x,
                 struct avl_itr*    itr);
/**
 * Move to the next object in order
 *
 * @param itr     iterator (modified)
 *
 * @return 1 if there is a next object; 0 otherwise
 */
int avl_itr_next(struct avl_itr* itr);

const avl_node_gt* avl_at(avl_itr_gt* itr);

unsigned long avl_node_get_key(const avl_node_gt* node);

#endif
