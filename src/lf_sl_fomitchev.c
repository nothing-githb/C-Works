/**
 * * LOCK-FREE LINKED LISTS AND SKIP LISTS
 *
 * MIKHAIL FOMITCHEV
 *
 * A thesis submitted to the Faculty of Graduate Studies in partial fulfilment
 * of the requirements for the degree of Master of Science Graduate Programme in
 * Computer Science York University
 *
 * North York, Ontario
 *
 * November, 2003
 *
 * !Old brief:
 * This algorithm is the linked list as published in
 *    Fomitchev and Ruppert
 *    Lock-free linked lists and skip lists
 *    PODC 2004.
 */

#include "lf_sl_fomitchev.h"

#include "limits.h"
#include "stddef.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * The variables of the Node type can be either node pointers, or one of the
 * special return values (DUPLICATE_KEY, NO_SUCH_KEY, NO_SUCH_NODE) used to
 * indicate that the operation failed.
 */
#define DUPLICATE_KEY 0x111
#define NO_SUCH_KEY   0x333
#define NO_SUCH_NODE  0x555

lf_sl_node_gt* new_node(int val, lf_sl_node_gt* next)
{
    lf_sl_node_gt* node = calloc(sizeof(lf_sl_node_gt), 1);

    if (NULL == node)
    {
        perror("malloc");
        exit(1);
    }
    node->val  = val;
    node->next = next;
    return node;
}

// Create a new set.
lf_sl_list_gt* set_new(void)
{
    lf_sl_list_gt* set = malloc(sizeof(lf_sl_list_gt));
    if (NULL == set)
    {
        perror("malloc");
        exit(1);
    }

    lf_sl_node_gt* max = new_node(INT_MAX, NULL);
    lf_sl_node_gt* min = new_node(INT_MIN, max);
    set->head          = min;
    return set;
}

// Delete the set and all its elements.
void set_delete(lf_sl_list_gt* set)
{
    lf_sl_node_gt* prev;
    lf_sl_node_gt* curr;

    curr = set->head;
    while (NULL != curr)
    {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(set);
}

// Get the size of the set, excluding the two head and
// tail sentinels.
int set_size(lf_sl_list_gt* set)
{
    int            size = 0;
    lf_sl_node_gt* curr = set->head->next;

    while (curr->next != NULL)
    {
        size++;
        curr = curr->next;
    }
    return size;
}

// For debugging
void set_print(lf_sl_list_gt* set)
{
    lf_sl_node_gt* curr = set->head;
    while (curr != NULL)
    {
        printf("%d", curr->val);
        if (curr->next != NULL)
        {
            printf(" -> ");
        }
        curr = curr->next;
    }
    printf("\n");
}

// The node.next field in the algorithm is presented as a tuple
// (right, mark, flag) where right is a reference, and the other
// two are booleans. Following the presentation in the paper,
// arrange the tuple in this way. The following functions work
// on tuples.

#define MARK          ((unsigned long) 2)
#define FLAG          ((unsigned long) 1)

// Is the tuple marked?
#define IS_MARKED(n)  (0 != ((unsigned long) (n) &MARK))

// Is the tuple flagged?
#define IS_FLAGGED(n) (0 != ((unsigned long) (n) &FLAG))

// Return the right field of the tuple.
inline static lf_sl_node_gt* get_right(lf_sl_node_gt* n)
{
    return (lf_sl_node_gt*) ((unsigned long) n & ~(unsigned long) 3);
}

// Assemble a new (right, mark, flag) tuple.
inline static lf_sl_node_gt*
    pack_tuple(lf_sl_node_gt* right, int mark, int flag)
{
    unsigned long tuple = (unsigned long) get_right(right);

    if (flag)
    {
        tuple |= (unsigned long) 1;
    }

    if (mark)
    {
        tuple |= (unsigned long) 2;
    }
    return (lf_sl_node_gt*) tuple;
}

// Assumes that prev_node is flagged, del_node is marked, and
// prev_node.right = del_node. Attempts to swing prev_node.right
// to del_node.right.
static void fomitchev_helpmarked(lf_sl_node_gt* prev_node,
                                 lf_sl_node_gt* del_node)
{
    lf_sl_node_gt* next_node = get_right(del_node->next);
    lf_sl_node_gt* expected  = pack_tuple(del_node, 0, 1);
    __sync_bool_compare_and_swap(
        &prev_node->next, &expected, pack_tuple(next_node, 0, 0));
}

// Searches forward from curr_node to find two nodes n1 and n2,
// satisfying n1.val <= val < n2.val.
static void lf_sl_search_to_level(int             val,
                                  lf_sl_node_gt*  curr,
                                  lf_sl_node_gt** n1,
                                  lf_sl_node_gt** n2)
{
    lf_sl_node_gt* next = get_right(curr->next);

    while (next->val <= val)
    {
        // Ensure either next_node is unmarked,
        // or both curr_node and next_node are
        // marked and curr_node was marked earlier.
        while (IS_MARKED(next->next) &&
               (! IS_MARKED(curr->next) || get_right(curr->next) != next))
        {
            if (get_right(curr->next) == next)
            {
                fomitchev_helpmarked(curr, next);
            }
            next = get_right(curr->next);
        }
        if (next->val <= val)
        {
            curr = next;
            next = get_right(curr->next);
        }
    }

    // Return (curr_node, next_node)
    *n1 = curr;
    *n2 = next;
}

// Searches forward from curr_node to find two nodes n1 and n2,
// satisfying n1.val < val <= n2.val.
static void fomitchev_searchfrom2(int             val,
                                  lf_sl_node_gt*  curr_node,
                                  lf_sl_node_gt** n1,
                                  lf_sl_node_gt** n2)
{
    lf_sl_node_gt* next_node = get_right(curr_node->next);
    while (next_node->val < val)
    {
        // Ensure either next_node is unmarked,
        // or both curr_node and next_node are
        // marked and curr_node was marked earlier.
        while (IS_MARKED(next_node->next) &&
               (! IS_MARKED(curr_node->next) ||
                get_right(curr_node->next) != next_node))
        {
            if (get_right(curr_node->next) == next_node)
            {
                fomitchev_helpmarked(curr_node, next_node);
            }
            next_node = get_right(curr_node->next);
        }
        if (next_node->val < val)
        {
            curr_node = next_node;
            next_node = get_right(curr_node->next);
        }
    }

    // Return (curr_node, next_node)
    *n1 = curr_node;
    *n2 = next_node;
}

static void fomitchev_helpflagged(lf_sl_node_gt* prev_node,
                                  lf_sl_node_gt* del_node);

// Assumes that del_node is preceded by a flagged node.
// Attempts to mark the node del_node.
static void fomitchev_trymark(lf_sl_node_gt* del_node)
{
    do
    {
        lf_sl_node_gt* next_node = get_right(del_node->next);
        lf_sl_node_gt* expected  = pack_tuple(next_node, 0, 0);
        __sync_bool_compare_and_swap(
            &del_node->next, &expected, pack_tuple(next_node, 1, 0));
        if (IS_FLAGGED(expected))
        {
            // Failure due to del_node becoming flagged.
            fomitchev_helpflagged(del_node, get_right(expected));
        }
    } while (! IS_MARKED(del_node->next));
}

// Assumes that prev_node is flagged, and prev_node.right = del_node.
// Attempts to mark and then physically del_node.
static void fomitchev_helpflagged(lf_sl_node_gt* prev_node,
                                  lf_sl_node_gt* del_node)
{
    del_node->backlink = prev_node;
    if (! IS_MARKED(del_node->next))
    {
        fomitchev_trymark(del_node);
    }
    fomitchev_helpmarked(prev_node, del_node);
}

// Attempts to flag the predecessor of target_node.
// prev_node is the last known node to be the predecessor.
//
// Upon returning, if ret_node is not null, then ret_node is flagged and
// precedes target_node. If ret_node is null, then some other operation deleted
// the target_node.
//
// The return value (boolean) is true only if ret_node is not null, and this
// function call was the one who performed the flagging.
static int fomitchev_tryflag(lf_sl_node_gt*  prev_node,
                             lf_sl_node_gt*  target_node,
                             lf_sl_node_gt** ret_node)
{
    for (;;)
    {
        if (prev_node->next == pack_tuple(target_node, 0, 1))
        {
            // Predecessor already flagged. Report fail and return predecessor.
            *ret_node = prev_node;
            return 0;
        }

        // Attempt to flag
        lf_sl_node_gt* expected = pack_tuple(target_node, 0, 0);
        __sync_bool_compare_and_swap(
            &prev_node->next, &expected, pack_tuple(target_node, 0, 1));

        if (expected == pack_tuple(target_node, 0, 0))
        {
            // Successful flagging
            *ret_node = prev_node;
            return 1;
        }
        if (expected == pack_tuple(target_node, 0, 1))
        {
            // Some concurrent op flagged it first.
            *ret_node = prev_node;
            return 0;
        }
        // Possibly a fail due to marking. Follow the backlinks to
        // something unmarked.
        while (IS_MARKED(prev_node->next))
        {
            prev_node = prev_node->backlink;
        }

        lf_sl_node_gt* del_node;
        fomitchev_searchfrom2(
            target_node->val, prev_node, &prev_node, &del_node);
        if (del_node != target_node)
        {
            // Target got deleted
            *ret_node = NULL;
            return 0;
        }
    }
}

// Interface functions.

/**
 * @brief The lf_sl_search (Search_SL) routine searches for a root node with the
 * supplied key.
 *
 * @param list [in] Address of the lock-free skip-list
 * @param val  [in] Value to search
 *
 * @retval Address of the node with value @p val
 * @retval NO_SUCH_KEY
 *
 * @return lf_sl_node_gt*
 */
lf_sl_node_gt* lf_sl_search(lf_sl_list_gt* list, int val)
{
    lf_sl_node_gt* curr_node;
    lf_sl_node_gt* next_node;

    lf_sl_search_to_level(val, list->head, &curr_node, &next_node);

    if (curr_node->val == val)
    {
        return curr_node;
    }

    return (lf_sl_node_gt*) NO_SUCH_KEY;
}

// Inserts val into set. Returns 1 if val was inserted, 0 if it already existed.
int lf_sl_insert(lf_sl_list_gt* set, int val)
{
    lf_sl_node_gt* prev_node;
    lf_sl_node_gt* next_node;

    lf_sl_search_to_level(val, set->head, &prev_node, &next_node);

    if (prev_node->val == val)
    {
        return 0;
    }

    lf_sl_node_gt* newnode = new_node(val, NULL);
    for (;;)
    {
        lf_sl_node_gt* prev_next = prev_node->next;
        if (IS_FLAGGED(prev_next))
        {
            fomitchev_helpflagged(prev_node, get_right(prev_next));
        }
        else
        {
            newnode->next           = pack_tuple(next_node, 0, 0);
            lf_sl_node_gt* expected = pack_tuple(next_node, 0, 0);
            __sync_bool_compare_and_swap(
                &prev_node->next, &expected, pack_tuple(newnode, 0, 0));
            if (expected == pack_tuple(next_node, 0, 0))
            {
                // Success
                return 1;
            }
            else
            {
                // Failure due to flagging?
                if (IS_FLAGGED(expected))
                {
                    fomitchev_helpflagged(prev_node, get_right(expected));
                }
                // May have to go through backlinks due to marking.
                while (IS_MARKED(prev_node->next))
                {
                    prev_node = prev_node->backlink;
                }
            }
        }
        lf_sl_search_to_level(val, prev_node, &prev_node, &next_node);
        if (prev_node->val == val)
        {
            // Free newnode? Nah.
            return 0;
        }
    }
}

// Removes val from set. Returns 1 if val was removed, 0 if val was not
// already in there.
int lf_sl_delete(lf_sl_list_gt* set, int val)
{
    lf_sl_node_gt* prev_node;
    lf_sl_node_gt* del_node;
    fomitchev_searchfrom2(val, set->head, &prev_node, &del_node);
    if (del_node->val != val)
    {
        return 0;   // No such key
    }
    int result = fomitchev_tryflag(prev_node, del_node, &prev_node);
    if (prev_node != NULL)
    {
        fomitchev_helpflagged(prev_node, del_node);
    }
    if (! result)
    {
        return 0;   // No such key
    }
    // Could return the deleted node here.
    return 1;
}
