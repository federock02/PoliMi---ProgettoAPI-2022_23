#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>



#define BLACK 0
#define RED 1



/* ----------NOTES---------- */
/*
- if slow, unite the check if a car already exists with the insertion or increment of counter
*/
/* ----------NOTES---------- */



/* ----------DATA STRUCTURES---------- */
typedef struct nodoStazione{
    uint32_t key; //distanza
    uint32_t max_autonomia;
    int steps_inmezzo;
    char color;
    struct nodoStazione *left;
    struct nodoStazione *right;
    struct nodoStazione *p;
    struct nodoStazione *prossimo_passo;
    struct treeMacchine *treeM;
} nodoStazione;

typedef struct nodoMacchina{
    uint32_t key; //autonomia
    uint32_t numero;
    char color;
    struct nodoMacchina *left;
    struct nodoMacchina *right;
    struct nodoMacchina *p;
} nodoMacchina;

typedef struct treeStazioni{
    struct nodoStazione *root;
    struct nodoStazione *nil;
} treeStazioni;

typedef struct treeMacchine{
    struct nodoMacchina *root;
    struct nodoMacchina *nil;
} treeMacchine;
/* ----------DATA STRUCTURES---------- */



/* ----------DATA STRUCTURES FUNCTIONS STAZIONI---------- */
nodoStazione* left_rotate_s(treeStazioni *tree, nodoStazione *x){
    nodoStazione *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil)
    {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->p->left)
    {
        x->p->left = y;
    }
    else
    {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
    return tree->root;
}

nodoStazione* right_rotate_s(treeStazioni *tree, nodoStazione *x){
    nodoStazione *y = x->left;
    x->left = y->right;
    if (y->right != tree->nil)
    {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->p->right)
    {
        x->p->right = y;
    }
    else
    {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
    return tree->root;
}

nodoStazione* rb_tree_successor_s(treeStazioni *tree, nodoStazione *x){
    if (x->right != tree->nil)
    {
        x = x->right;
        while (x->left != tree->nil)
        {
            x = x->left;
        }
        return x;
    }
    else
    {
        nodoStazione *y = x->p;
        while (y != tree->nil && x == y->right)
        {
            x = y;
            y = y->p;
        }
        return y;
    }
}

nodoStazione* rb_tree_predecessor_s(treeStazioni *tree, nodoStazione *x){
    if (x->left != tree->nil)
    {
        x = x->left;
        while (x->right != tree->nil)
        {
            x = x->right;
        }
        return x;
    }
    else
    {
        nodoStazione *y = x->p;
        while (y != tree->nil && x == y->left)
        {
            x = y;
            y = y->p;
        }
        return y;
    }
}

nodoStazione* rb_insert_fixup_s(treeStazioni *tree, nodoStazione *z)
{
    while (z->p->color == RED)
    {
        if (z->p == z->p->p->left)
        {
            nodoStazione *y = z->p->p->right;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->right)
                {
                    z = z->p;
                    tree->root = left_rotate_s(tree, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                tree->root = right_rotate_s(tree, z->p->p);
            }
        }
        else
        {
            nodoStazione *y = z->p->p->left;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->left)
                {
                    z = z->p;
                    tree->root = right_rotate_s(tree, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                tree->root = left_rotate_s(tree, z->p->p);
            }

        }
    }
    tree->root->color = BLACK;
    return tree->root;
}

nodoStazione* rb_insert_s(treeStazioni *tree, nodoStazione *z){
    nodoStazione *y = tree->nil;
    nodoStazione *x = tree->root;
    while (x != tree->nil)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->p = y;
    if (y == tree->nil)
    {
        tree->root = z;
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;
    tree->root = rb_insert_fixup_s(tree, z);
    return tree->root;
}

nodoStazione* rb_transplant_s(treeStazioni *tree, nodoStazione *u, nodoStazione *v){
    if (u->p == tree->nil)
    {
        tree->root = v;
    }
    else if (u == u->p->left)
    {
        u->p->left = v;
    }
    else
    {
        u->p->right = v;
    }
    v->p = u->p;
    return tree->root;
}

nodoStazione* rb_delete_fixup_s(treeStazioni *tree, nodoStazione *x){
    nodoStazione *w;
    while (x != tree->root && x->color == BLACK)
    {
        if (x == x->p->left)
        {
            w = x->p->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                tree->root = left_rotate_s(tree, x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }
            else
            { 
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    tree->root = right_rotate_s(tree, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                tree->root = left_rotate_s(tree, x->p);
                x = tree->root;
            }
        }
        else
        {
            w = x->p->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                tree->root = right_rotate_s(tree, x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }
            else
            { 
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    tree->root = left_rotate_s(tree, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                tree->root = right_rotate_s(tree, x->p);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
    return tree->root;
}

nodoStazione* rb_delete_s(treeStazioni *tree, nodoStazione *z){
    nodoStazione *y = z;
    nodoStazione *x;
    char y_original_color = y->color;
    if (z->left == tree->nil)
    {
        x = z->right;
        tree->root = rb_transplant_s(tree, z, z->right);
    }
    else if (z->right == tree->nil)
    {
        x = z->left;
        tree->root = rb_transplant_s(tree, z, z->left);
    }
    else
    {
        y = z->right;
        while (y->left != tree->nil)
        {
            y = y->left;
        }
        y_original_color = y->color;
        x = y->right;
        if (y->p == z)
        {
            x->p = y;
        }
        else
        {
            tree->root = rb_transplant_s(tree, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        tree->root = rb_transplant_s(tree, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK)
    {
        tree->root = rb_delete_fixup_s(tree, x);
    }
    return tree->root;
}

nodoStazione* rb_tree_search_s(treeStazioni *tree, nodoStazione *root, uint32_t x){
    if (root == tree->nil || x == root->key)
    {
        return root;
    }
    if (x < root->key)
    {
        return rb_tree_search_s(tree, root->left, x);
    }
    else
    {
        return rb_tree_search_s(tree, root->right, x);
    }
}

int rb_tree_search_quick_s(treeStazioni *tree, nodoStazione *root, uint32_t x){
    if (root == tree->nil)
    {
        return 0;
    }
    if (x == root->key)
    {
        return 1;
    }
    if (x < root->key)
    {
        return rb_tree_search_quick_s(tree, root->left, x);
    }
    else
    {
        return rb_tree_search_quick_s(tree, root->right, x);
    }
}
/* ----------DATA STRUCTURES FUNCTIONS STAZIONI---------- */



/* ----------DATA STRUCTURES FUNCTIONS MACCHINE---------- */
nodoMacchina* left_rotate_m(treeMacchine *tree, nodoMacchina *x){
    nodoMacchina *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil)
    {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->p->left)
    {
        x->p->left = y;
    }
    else
    {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
    return tree->root;
}

nodoMacchina* right_rotate_m(treeMacchine *tree, nodoMacchina *x){
    nodoMacchina *y = x->left;
    x->left = y->right;
    if (y->right != tree->nil)
    {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->p->right)
    {
        x->p->right = y;
    }
    else
    {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
    return tree->root;
}

nodoMacchina* rb_tree_predecessor_m(treeMacchine *tree, nodoMacchina *x){
    if (x->left != tree->nil)
    {
        x = x->left;
        while (x->right != tree->nil)
        {
            x = x->right;
        }
        return x;
    }
    else
    {
        nodoMacchina *y = x->p;
        while (y != tree->nil && x == y->left)
        {
            x = y;
            y = y->p;
        }
        return y;
    }
}

nodoMacchina* rb_insert_fixup_m(treeMacchine *tree, nodoMacchina *z)
{
    while (z->p->color == RED)
    {
        if (z->p == z->p->p->left)
        {
            nodoMacchina *y = z->p->p->right;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->right)
                {
                    z = z->p;
                    tree->root = left_rotate_m(tree, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                tree->root = right_rotate_m(tree, z->p->p);
            }
        }
        else
        {
            nodoMacchina *y = z->p->p->left;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->left)
                {
                    z = z->p;
                    tree->root = right_rotate_m(tree, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                tree->root = left_rotate_m(tree, z->p->p);
            }

        }
    }
    tree->root->color = BLACK;
    return tree->root;
}

nodoMacchina* rb_insert_m(treeMacchine *tree, nodoMacchina *z){
    nodoMacchina *y = tree->nil;
    nodoMacchina *x = tree->root;
    while (x != tree->nil)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->p = y;
    if (y == tree->nil)
    {
        tree->root = z;
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;
    tree->root = rb_insert_fixup_m(tree, z);
    return tree->root;
}

nodoMacchina* rb_transplant_m(treeMacchine *tree, nodoMacchina *u, nodoMacchina *v){
    if (u->p == tree->nil)
    {
        tree->root = v;
    }
    else if (u == u->p->left)
    {
        u->p->left = v;
    }
    else
    {
        u->p->right = v;
    }
    v->p = u->p;
    return tree->root;
}

nodoMacchina* rb_delete_fixup_m(treeMacchine *tree, nodoMacchina *x){
    nodoMacchina *w;
    while (x != tree->root && x->color == BLACK)
    {
        if (x == x->p->left)
        {
            w = x->p->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                tree->root = left_rotate_m(tree, x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }
            else
            { 
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    tree->root = right_rotate_m(tree, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                tree->root = left_rotate_m(tree, x->p);
                x = tree->root;
            }
        }
        else
        {
            w = x->p->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                tree->root = right_rotate_m(tree, x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }
            else
            { 
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    tree->root = left_rotate_m(tree, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                tree->root = right_rotate_m(tree, x->p);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
    return tree->root;
}

nodoMacchina* rb_delete_m(treeMacchine *tree, nodoMacchina *z){
    nodoMacchina*y = z;
    nodoMacchina *x;
    char y_original_color = y->color;
    if (z->left == tree->nil)
    {
        x = z->right;
        tree->root = rb_transplant_m(tree, z, z->right);
    }
    else if (z->right == tree->nil)
    {
        x = z->left;
        tree->root = rb_transplant_m(tree, z, z->left);
    }
    else
    {
        y = z->right;
        while (y->left != tree->nil)
        {
            y = y->left;
        }
        y_original_color = y->color;
        x = y->right;
        if (y->p == z)
        {
            x->p = y;
        }
        else
        {
            tree->root = rb_transplant_m(tree, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        tree->root = rb_transplant_m(tree, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK)
    {
        tree->root = rb_delete_fixup_m(tree, x);
    }
    return tree->root;
}

nodoMacchina* rb_tree_search_m(treeMacchine *tree, nodoMacchina *root, uint32_t x){
    if (root == tree->nil || x == root->key)
    {
        return root;
    }
    if (x < root->key)
    {
        return rb_tree_search_m(tree, root->left, x);
    }
    else
    {
        return rb_tree_search_m(tree, root->right, x);
    }
}

int rb_tree_search_quick_m(treeMacchine *tree, nodoMacchina *root, uint32_t x){
    if (root == tree->nil)
    {
        return 0;
    }
    if (x == root->key)
    {
        return 1;
    }
    if (x < root->key)
    {
        return rb_tree_search_quick_m(tree, root->left, x);
    }
    else
    {
        return rb_tree_search_quick_m(tree, root->right, x);
    }
}
/* ----------DATA STRUCTURES FUNCTIONS MACCHINE---------- */



/* ----------PROJECT FUNCTIONS---------- */
void rimuovi_tutte_auto(nodoMacchina *root, nodoMacchina *nil){
    if (root->left != nil)
    {
        rimuovi_tutte_auto(root->left, nil);
    }
    if (root->right != nil)
    {
        rimuovi_tutte_auto(root->right, nil);
    }
    free(root);
}

nodoStazione* azzera_percorsi(treeStazioni *tree, nodoStazione *root){
    //printf("%d\n", root->key);
    if (root->left != tree->nil)
    {
        root->left = azzera_percorsi(tree, root->left);
    }
    if (root->right != tree->nil)
    {
        root->right = azzera_percorsi(tree, root->right);
    }
    root->steps_inmezzo = -1;
    root->prossimo_passo = tree->nil;
    return root;
}

int compara_percorsi(nodoStazione *start1, nodoStazione *start2, int best){
    if (start1->steps_inmezzo > 1){
        best = compara_percorsi(start1->prossimo_passo, start2->prossimo_passo, best);
    }
    if (best == 0 && start1->key < start2->key){
        best = 1;
    }
    else if (best == 0 && start1->key > start2->key){
        best = 2;
    }
    return best;
}

nodoStazione* prossimo_passo_dx(treeStazioni *tree, nodoStazione *nodo, uint32_t autonomia_left){
    nodoStazione *next = rb_tree_successor_s(tree, nodo);
    if (nodo->key + autonomia_left >= next->key)
    {
        return next;
    }
    else
    {
        return tree->nil;
    }
}

nodoStazione* prossimo_passo_sx(treeStazioni *tree, nodoStazione *nodo, uint32_t autonomia_left){
    nodoStazione *next, * prec;
    if (nodo->key < autonomia_left)
    {
        next = tree->root;
        while(next->left != tree->nil)
        {
            next = next->left;
        }
        return next;
    }
    next = rb_tree_predecessor_s(tree, nodo);
    prec = tree->nil;
    while (next != tree->nil && nodo->key - autonomia_left <= next->key)
    {
        prec = next;
        next = rb_tree_predecessor_s(tree,next);
    }
    if (prec->steps_inmezzo == -2)
    {
        return tree->nil;
    }
    return prec;
}

nodoStazione* pianifica_percorso_dx(treeStazioni *tree, nodoStazione *start, nodoStazione *finish, uint32_t autonomia){
    if (start->key == finish->key){
        return start;
    }
    else if (start->key + autonomia >= finish->key)
    {
        start->steps_inmezzo = 1;
        start->prossimo_passo = finish;
        return start;
    }
    else
    {
        nodoStazione *next = prossimo_passo_dx(tree, start, autonomia);
        nodoStazione *prox = next;
        if (next == tree->nil){
            return tree->nil;
        }
        if (next->steps_inmezzo == -1)
        {
            next = pianifica_percorso_dx(tree, next, finish, next->max_autonomia);
        }
        if (next != tree->nil)
        {
            start->steps_inmezzo = next->steps_inmezzo + 1;
            start->prossimo_passo = next;
        }
        else
        {
            next = prox;
        }
        next = prossimo_passo_dx(tree, next, autonomia - (next->key - start->key));
        while (next != tree->nil)
        {
            prox = next;
            if (next->steps_inmezzo == -1)
            {
                next = pianifica_percorso_dx(tree, next, finish, next->max_autonomia);
            }
            if (next != tree->nil && (next->steps_inmezzo + 1 < start->steps_inmezzo))
            {
                start->steps_inmezzo = next->steps_inmezzo + 1;
                start->prossimo_passo = next;
            }
            if (next == tree->nil)
            {
                next = prox;
            }
            next = prossimo_passo_dx(tree, next, autonomia - (next->key - start->key));
        }
        if (start->prossimo_passo == tree->nil)
        {
            return tree->nil;
        }
        return start;
    }
}

nodoStazione* pianifica_percorso_sx(treeStazioni *tree, nodoStazione *start, nodoStazione *finish, uint32_t autonomia){
    if (start->key < autonomia)
    {
        autonomia = start->key;
    }
    if (start->key == finish->key){
        return start;
    }
    else if (start->key - autonomia <= finish->key)
    {
        start->steps_inmezzo = 1;
        start->prossimo_passo = finish;
        return start;
    }
    else
    {
        nodoStazione *next = prossimo_passo_sx(tree, start, autonomia);
        nodoStazione *prox = next;
        if (next == tree->nil){
            return tree->nil;
        }
        if (next->key < finish->key)
        {
            next = finish;
        }
        if (next->steps_inmezzo == -1)
        {
            next = pianifica_percorso_sx(tree, next, finish, next->max_autonomia);
        }
        if (next != tree->nil)
        {
            start->steps_inmezzo = next->steps_inmezzo + 1;
            start->prossimo_passo = next;
        }
        else
        {
            prox->steps_inmezzo = -2;
        }
        next = rb_tree_successor_s(tree, prox);
        if (next == start)
        {
            next = tree->nil;
        }
        while (next != tree->nil)
        {
            prox = next;
            if (next->steps_inmezzo == -1)
            {
                next = pianifica_percorso_sx(tree, next, finish, next->max_autonomia);
            }
            if (next != tree->nil && (next->steps_inmezzo + 1 < start->steps_inmezzo))
            {
                start->steps_inmezzo = next->steps_inmezzo + 1;
                start->prossimo_passo = next;
            }
            else if (next != tree->nil && (next->steps_inmezzo + 1 == start->steps_inmezzo))
            {
                if (compara_percorsi(start->prossimo_passo, next, 0) == 2)
                {
                    start->prossimo_passo = next;
                }
            }
            next = rb_tree_successor_s(tree, prox);
            if (next == start)
            {
                next = tree->nil;
            }
        }
        if (start->prossimo_passo == tree->nil)
        {
            return tree->nil;
        }
        return start;
    }
}
/* ----------PROJECT FUNCTIONS---------- */



/* ----------MAIN---------- */
int main(){
    treeStazioni *treeS;
    treeS = malloc(sizeof(treeStazioni));
    treeS->nil = malloc(sizeof(nodoStazione));
    treeS->nil->color = BLACK;
    treeS->root = treeS->nil;
    treeS->nil->left = treeS->nil;
    treeS->nil->right = treeS->nil;
    treeS->nil->p = treeS->nil;
    nodoMacchina *nodoNullm = malloc(sizeof(nodoMacchina));
    nodoNullm->color = BLACK;
    int trash;
    char funzione[19];
    uint32_t distanza, start, finish;
    uint32_t num_auto;
    uint32_t autonomia_auto;
    nodoStazione *nuovaStazione, *temp_s;
    nodoMacchina *nuovaMacchina, *temp_m;
    trash = 0;
    while(scanf("%s", funzione) != EOF)
    {
        switch (funzione[0])
        {
        case 'a': //|A|ggiungi-stazione o |A|ggiungi-auto
            if (funzione[9] == 's') //aggiungi-|S|tazione
            {
                //leggi distanza
                trash = scanf("%u", &distanza);
                //cerca se esiste già una stazione a quella distanza
                if (rb_tree_search_quick_s(treeS, treeS->root, distanza) == 1)
                {
                    //la stazione esiste già
                    printf("non aggiunta\n");
                    //leggi numero auto e autonomie per passare al prossimo comando
                    trash = scanf("%u", &num_auto);
                    for (int i = 0; i < num_auto; i++)
                    {
                        trash = scanf("%u", &autonomia_auto);
                    }
                }
                else
                {
                    //la stazione non esiste
                    //crea nuovo nodo stazione
                    nuovaStazione = malloc(sizeof(nodoStazione));
                    if (nuovaStazione == NULL)
                    {
                        printf("\nerrore malloc nuova stazione\n\n");
                    }
                    nuovaStazione->key = distanza;
                    nuovaStazione->left = treeS->nil;
                    nuovaStazione->right = treeS->nil;
                    nuovaStazione->max_autonomia = 0;
                    nuovaStazione->treeM = malloc(sizeof(treeMacchine));
                    //inserisci nuovo nodo nell'albero stazioni
                    treeS->root = rb_insert_s(treeS, nuovaStazione);
                    //leggi numero auto nella stazione
                    trash = scanf("%u", &num_auto);
                    nuovaStazione->treeM->root = nodoNullm;
                    nuovaStazione->treeM->nil = nodoNullm;
                    nuovaStazione->treeM->nil->left = nodoNullm;
                    nuovaStazione->treeM->nil->right = nodoNullm;
                    nuovaStazione->treeM->nil->p = nodoNullm;
                    for (int i = 0; i < num_auto; i++)
                    {
                        //legge autonomia auto
                        trash = scanf("%u", &autonomia_auto);
                        trash = trash + autonomia_auto;
                        //cerca se c'è già macchina con la stessa autonomia
                        temp_m = rb_tree_search_m(nuovaStazione->treeM, nuovaStazione->treeM->root, autonomia_auto);
                        if (temp_m == nodoNullm)
                        {
                            //controlla se l'autonomia è maggiore di quella massima
                            if (autonomia_auto > nuovaStazione->max_autonomia)
                            {
                                nuovaStazione->max_autonomia = autonomia_auto;
                            }
                            //crea nuovi nodi nell'albero macchine della stazione
                            nuovaMacchina = malloc(sizeof(nodoMacchina));
                            if (nuovaMacchina == NULL)
                            {
                                printf("\nerrore malloc nuova macchina in nuova stazione\n\n");
                            }
                            nuovaMacchina->key = autonomia_auto;
                            nuovaMacchina->numero = 1;
                            nuovaMacchina->left = nodoNullm;
                            nuovaMacchina->right = nodoNullm;
                            //inserisci nuovo nodo macchina
                            nuovaStazione->treeM->root = rb_insert_m(nuovaStazione->treeM, nuovaMacchina);
                        }
                        else
                        {
                            temp_m->numero += 1;
                        }
                    }
                printf("aggiunta\n");
                //printf("aggiunta stazione %d\n", distanza);
                }
            }
            else //aggiungi-|A|uto
            {
                //leggi distanza
                trash = scanf("%u", &distanza);
                //cerca se esiste una stazione a quella distanza
                temp_s = rb_tree_search_s(treeS, treeS->root, distanza);
                if (temp_s == treeS->nil)
                {
                    //la stazione non esiste
                    printf("non aggiunta\n");
                    //leggi autonomia
                    trash = scanf("%u", &autonomia_auto);
                }
                else
                {
                    //la stazione esiste
                    //leggi autonomia auto da aggiungere
                    trash = scanf("%u", &autonomia_auto);
                    //cerca se esiste già auto con stessa autonomia
                    temp_m = rb_tree_search_m(temp_s->treeM, temp_s->treeM->root, autonomia_auto);
                    if (temp_m == nodoNullm)
                    {
                        //crea nuovi nodi nell'albero macchine della stazione
                        nuovaMacchina = malloc(sizeof(nodoMacchina));
                        if (nuovaMacchina == NULL)
                        {
                            printf("\nerrore malloc nuova macchina\n\n");
                        }
                        nuovaMacchina->key = autonomia_auto;
                        nuovaMacchina->numero = 1;
                        nuovaMacchina->left = nodoNullm;
                        nuovaMacchina->right = nodoNullm;
                        //inserisci nuovo nodo macchina
                        temp_s->treeM->root = rb_insert_m(temp_s->treeM, nuovaMacchina);
                        if (autonomia_auto > temp_s->max_autonomia)
                        {
                            temp_s->max_autonomia = autonomia_auto;
                        }
                    }
                    else
                    {
                        temp_m->numero += 1;
                    }
                    printf("aggiunta\n");
                }
            }
            break;
        case 'd': //|D|emolisci-stazione
            //leggi distanza
            trash = scanf("%u", &distanza);
            //cerca se esiste una stazione a quella distanza
            temp_s = rb_tree_search_s(treeS, treeS->root, distanza);
            if (temp_s == treeS->nil)
            {
                //la stazione non esiste
                printf("non demolita\n");
            }
            else
            {
                //la stazione esiste
                //rimuovi e free memoria per tutte le auto della stazione
                rimuovi_tutte_auto(temp_s->treeM->root, nodoNullm);
                free(temp_s->treeM);
                // temp_s->treeM->root = nodoNullm;
                //rimuovi stazione dall'albero
                treeS->root = rb_delete_s(treeS, temp_s);
                printf("demolita\n");
                free(temp_s);
            }
            break;
        case 'r': //|R|ottama-auto
            //leggi distanza
            trash = scanf("%u", &distanza);
            //cerca se esiste una stazione a quella distanza
            temp_s = rb_tree_search_s(treeS, treeS->root, distanza);
            if (temp_s == treeS->nil)
            {
                //la stazione non esiste
                printf("non rottamata\n");
                //leggi autonomia
                trash = scanf("%u", &autonomia_auto);
            }
            else
            {
                //la stazione esiste
                //leggi autonomia
                trash = scanf("%u", &autonomia_auto);
                //cerca se esiste auto con l'autonomia
                temp_m = rb_tree_search_m(temp_s->treeM, temp_s->treeM->root, autonomia_auto);
                if (temp_m == nodoNullm)
                {
                    //l'auto non esiste
                    printf("non rottamata\n");
                }
                else
                {
                    //l'auto esiste
                    //controllo quante auto con l'autonomia data esistono
                    if (temp_m->numero == 1)
                    {
                        if (temp_m->key == temp_s->max_autonomia)
                        {
                            nuovaMacchina = rb_tree_predecessor_m(temp_s->treeM, temp_m);
                            temp_s->max_autonomia = nuovaMacchina->key;
                        }
                        temp_s->treeM->root = rb_delete_m(temp_s->treeM, temp_m);
                        free(temp_m);
                    }
                    else
                    {
                        temp_m->numero -= 1;
                    }
                    printf("rottamata\n");
                }
            }
            break;
        case 'p': //|P|ianifica-percorso
            //azzera l'utilizzo precedente di pianifica_percorso
            treeS->root = azzera_percorsi(treeS, treeS->root);
            //printf("percorsi azzerati\n");
            //leggi distanza partenza
            trash = scanf("%u", &start);
            //leggi distanza arrivo
            trash = scanf("%u", &finish);
            //ricerca dei nodi delle stazioni di partenza e arrivo
            temp_s = rb_tree_search_s(treeS, treeS->root, start);
            nuovaStazione = rb_tree_search_s(treeS, treeS->root, finish);
            nuovaStazione->steps_inmezzo = 0;
            nuovaStazione->prossimo_passo = treeS->nil;
            //due casi in base alla direzione di percorrenza dell'autostrada
            if (start < finish)
            {
                //direzione con distanze crescenti
                temp_s = pianifica_percorso_dx(treeS, temp_s, nuovaStazione, temp_s->max_autonomia);
            }
            else
            {
                //direzione con distanze decrescenti
                temp_s = pianifica_percorso_sx(treeS, temp_s, nuovaStazione, temp_s->max_autonomia);
            }
            if (temp_s != treeS->nil)
            {
                while (temp_s->prossimo_passo != treeS->nil)
                {
                    nuovaStazione = temp_s->prossimo_passo;
                    printf("%u ", temp_s->key);
                    temp_s = nuovaStazione;
                }
                printf("%u\n", temp_s->key);
            }
            else
            {
                printf("nessun percorso\n");
            }
            break;        
        default:
            break;
        }
    }
}
/* ----------MAIN---------- */