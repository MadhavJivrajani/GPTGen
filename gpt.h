#ifndef GPT_H
#define GPT_H

struct node {
	char *key;
	struct node* left;
	struct node* right;
};
typedef struct node node_t;

struct tree {
	node_t *root;
};
typedef struct tree tree_t;

void init_tree(tree_t *ptr_tree);
int check_root(tree_t *ptr_tree);
void begin_boilerplate();
void add_node(tree_t *ptr_tree, char *parent, char *key);
void generate_code(tree_t *ptr_tree);
void end_boilerplate();
void clean_tree(tree_t *ptr_tree);
void ground_tree(tree_t *ptr_tree);

#endif