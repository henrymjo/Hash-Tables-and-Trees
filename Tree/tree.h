/**
 *This is the ADT for the tree program
 *each method is implemented tree.c
 *The ADT also includes the enumerated types.
 */

#ifndef TREE_H_
#define TREE_H_

typedef struct tree_node *tree;
typedef enum tree_e {BST,RBT} tree_t;



extern tree	tree_free(tree t);
extern void	tree_inorder(tree t, void f(char *str));
extern tree	tree_insert(tree t, char *str);
extern tree	tree_new(tree_t type);
extern void	tree_print(tree t, void f(int freq, char *str));
extern int	tree_search(tree t, char *str);

extern tree	tree_fix(tree t);
extern tree 	l_rotate(tree t);
extern tree     r_rotate(tree t);

extern int	tree_depth(tree t);
extern void	tree_output_dot(tree t, FILE *out);
extern void 	tree_output_dot_aux(tree t, FILE *out);


#endif
