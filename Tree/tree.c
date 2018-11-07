/**
 *This is the combination which implements the tree.h ADT methods
 *and creates an ordinary binary search
 * or a balanced red black tree based on the enumerated type thats passed to
 *tree_new() method
 *The ADT consists of numerous methods for consrtructing a tree, tree depth etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x) -> colour))
#define IS_RED(x) ((NULL !=(x)) && (RED == (x) -> colour))

typedef enum { RED, BLACK, NOTHING} rbt_colour;


static tree_t tree_type;

struct tree_node{
	char *key;
	rbt_colour colour;
	tree left;
	tree right;
	int freq;
};

/**
 *Function returns the depth of the tree.
 *@param t = tree being checked
 *@return int = returns the depth of the tree as an int
 */	
int tree_depth(tree t){
	if(t == NULL){
		return 0;
	}
	
	if(tree_depth(t -> left) > tree_depth(t->right)){
		return 1 + tree_depth(t -> left);
	}else{
		return 1 + tree_depth(t -> right);
	}
}
/**
 *Frees the memory of the tree recursivly
 *@param t = tree to free memory
 *@return tree = return an empty tree
 */
tree tree_free(tree t){

    if(t == NULL){
        return NULL;
    }
    tree_free(t -> left);
    tree_free(t -> right);
    free(t -> key);
    free(t);

    return NULL;
}

/**
 *Function traverses the tree in order.
 *@param t = tree being traversed
 *@param f = function beging applied to b as it's traversed
 */	
void tree_inorder(tree t, void f(char *str)){
	
	if(t == NULL){
		return;
	}	
	tree_inorder(t -> left, f);
	f(t -> key);
	tree_inorder(t -> right, f);

}

/**
 *Function inserts a new node into the tree. Also counts the frequency
 *of duplicates that have been tried to be added.
 *@param t = tree being added to
 *@param *str = node to be added
 *@return tree = returns the newly updated tree
 */	
tree tree_insert(tree t, char *str){	

	if(t == NULL){
		t = tree_new(tree_type);
	}

	if(t -> key == NULL){
		t->key = emalloc(sizeof *str);
		strcpy(t->key, str);
		t->colour = RED;
                t->freq++;
		return t;
	}

	if (strcmp(str, t -> key) == 0){
		/*increace frequency when its a duplicate*/
		t -> freq++;
		return t;
	}else if(strcmp(str, t -> key) < 0){
		t -> left = tree_insert(t -> left, str);
	}else if(strcmp(str, t -> key) > 0){
		t -> right = tree_insert(t -> right, str);
	}
	return t;
}

/**
 *Function creates a new tree to specifications.
 *@param type = type of tree to create, either BST or RBT 	
 *@return tree = returns a new tree with all elements being null
 */	
tree tree_new(tree_t type){

	tree newtree;
	newtree = emalloc(sizeof *newtree);
	newtree -> key = NULL;
	newtree -> left = NULL;
	newtree -> right = NULL;
	newtree -> freq = 0;
	
	if(type == RBT){
		newtree -> colour = RED;
	}else{
		newtree -> colour = NOTHING;
	}

	return newtree;
}

/**
 *Function traverses the tree in preorder, and prints the frequency
 *of each word.
 *@param t = tree being traversed
 *@param f = function beging applied to b as it's traversed
 */	
void tree_print(tree t, void f(int freq, char *str)){
	
	if(t == NULL){
		return;
	}
	f(t -> freq, t -> key);
	tree_print(t -> left, f);
	tree_print(t -> right, f);
	
}

/**
 *Function searched the tree for a predefined word.
 *@param t = tree being traversed
 *@param key = word being searched for
 *@return int = returns 0 if word isn't found, and 1 if it is
 */	
int tree_search(tree t, char *key){

    if(t == NULL ){
        return 0;
    }

    if(strcmp(t->key, key) == 0){
        return 1;
    }else if(strcmp(t -> key, key) > 0){
        return tree_search(t -> left, key);
    }else if(strcmp(t -> key, key) < 0){
        return tree_search(t -> right, key);
    }
	
    return 0;
}


/**
 *Function rotates the node right for balancing
 *@param t = tree which is being balanced
 *@return tree = return the updated tree
 */
tree r_rotate(tree t){
    tree temp;

    temp = t;
    t = t -> left;
    temp -> left = t -> right;
    t -> right = temp;

    return t;
}

/**
 *Function rotates the node left for balancing
 *@param t = tree which is being balanced
 *@return tree = return the updated tree
 */
tree l_rotate(tree t){
    tree temp;

    temp = t;
    t = t -> right;
    temp -> right = t -> left;
    t -> left = temp;

    return t;
}

/**
 *Function checks supplied tree if it needs balencing, then balences if necessary
 *@param t = tree which is being balanced
 *@return tree = return the updated tree
 */
tree tree_fix(tree t){

    if(IS_RED(t -> left) && IS_RED( t -> left -> left) && IS_RED(t -> right)){
        t -> colour = RED;
        t -> left -> colour = BLACK;
        t -> right -> colour = BLACK;
    }else if(IS_RED(t -> left) && IS_RED(t -> left -> left) && IS_BLACK(t -> 
    right)){
        t = r_rotate(t);
        t -> colour = BLACK;
        t -> right -> colour = RED;
    }else if(IS_RED(t -> left) && IS_RED(t -> left -> right) && IS_RED(t -> 
    right)){
        t -> colour = RED;
        t -> left -> colour = BLACK;
        t -> right -> colour = BLACK;
    }else if(IS_RED(t -> left) && IS_RED(t -> left -> right) && IS_BLACK(t ->
    right)){
        t -> left = l_rotate(t -> left);
        t = r_rotate(t);
        t -> colour = BLACK;
        t -> right = RED;
    }else if(IS_RED(t -> right) && IS_RED(t -> right -> left) && IS_RED(t -> 
    left)){
        t -> colour = RED;
        t -> left->colour = BLACK;
        t -> right->colour = BLACK;
    }else if(IS_RED(t -> right) && IS_RED(t -> right -> left) && IS_BLACK(t ->
    left)){
        t -> right = r_rotate(t -> right);
        t = l_rotate(t);
        t -> colour = BLACK;
        t -> left -> colour = RED;
    }else if(IS_RED(t -> right) && IS_RED(t -> right -> right) && IS_RED(t -> 
    left)){
        t -> colour = RED;
        t -> left -> colour = BLACK;
        t -> right -> colour = BLACK;
    }else if(IS_RED(t -> right) && IS_RED(t -> right -> right) && IS_BLACK(t
    -> left)){
        t = l_rotate(t);
        t -> colour = BLACK;
        t -> left -> colour = RED;
    }

    return t;
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
   fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
   tree_output_dot_aux(t, out);
   fprintf(out, "}\n");
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
void tree_output_dot_aux(tree t, FILE *out) {
   if(t->key != NULL) {
      fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
              t->key, t->key, t->freq,
              (RBT == tree_type && RED == t->colour) ? "red":"black");
   }
   if(t->left != NULL) {
      tree_output_dot_aux(t->left, out);
      fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
   }
   if(t->right != NULL) {
      tree_output_dot_aux(t->right, out);
      fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
   }
}
