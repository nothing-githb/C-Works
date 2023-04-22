//
// Created
// by
// Halis
// Şahin
// on 14.04.2023.
//

#ifndef C_ALGORITHMS_LF_SL_FOMITCHEV_H
#define C_ALGORITHMS_LF_SL_FOMITCHEV_H

typedef struct lf_ls_node lf_sl_node_gt;
typedef struct lf_ls_list lf_sl_list_gt;

struct lf_ls_node
{
    int                val;
    struct lf_ls_node* next;
    struct lf_ls_node* backlink;
};

struct lf_ls_list
{
    lf_sl_node_gt* head;
};

#endif   // C_ALGORITHMS_LF_SL_FOMITCHEV_H
