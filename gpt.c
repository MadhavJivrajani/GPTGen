#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpt.h"

void init_tree(tree_t *ptr_tree) {
	ptr_tree->root = NULL;
}

// create a node and return it.
node_t* make_node(char *key) {
	node_t* temp = (node_t*)malloc(sizeof(node_t));
	temp->key = (char*)malloc(1025*sizeof(char));
	strcpy(temp->key, key);
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

// helper function to recursively traverse over the tree
// and return the found node.
node_t* search_node_r(node_t *node, char *key) {
	if(node == NULL) {
		return NULL;
	}
	else {
		if(strcmp(node->key, key) == 0) {
			return node;
		}
		else {
			node_t* search = search_node_r(node->left, key);
			if(search == NULL) {
				search = search_node_r(node->right, key);
			}
			return search;
		}
	}
}

// searches for a node in a tree and returns it if found else returns NULL
node_t* search_node(tree_t *ptr_tree, char *key) {
	return search_node_r(ptr_tree->root, key);
}

// checks is the tree is grounded.
int check_root(tree_t *ptr_tree) {
	return ptr_tree->root == NULL;
}

// adds a node to the tree depending on whether it is a root or non-root node
void add_node(tree_t *ptr_tree, char *parent, char *key) {
	/*
		- First find the parent from the given key.
		- From the parent go left, if left is empty, add that node there.
		- Otherwise keep moving to the right and when it is null attach the child.

		- If the parent node is a space " " then this means that the node is to be added as
		  as a root node to the GPT.
		- To add as root, keep moving right from the root of the binary tree, when root->right
		  becomes NULL, insert node there.
	*/
	node_t *temp = make_node(key);

	// add the first root to the tree.
	if(ptr_tree->root == NULL && strcmp(parent, " ") == 0) {
		ptr_tree->root = temp;
	}

	// add a GPT root.
	else if(ptr_tree->root!=NULL && strcmp(parent, " ") == 0) {
		node_t *root_iter = ptr_tree->root;
		while(root_iter->right) {
			root_iter = root_iter->right;
		}
		root_iter->right = temp;
	}

	// add a non-GPT-root node.
	else {
		node_t *parent_node = search_node(ptr_tree, parent);
		if(parent_node != NULL) {
			if(parent_node->left == NULL) {
				parent_node->left = temp;
			}
			else {
				parent_node = parent_node->left;
				while(parent_node->right!=NULL) {
					parent_node = parent_node->right;
				}
				parent_node->right = temp;
			}
		}
	}
}

void gen_header() {
	printf("#include <stdio.h>\n\n");
}

void gen_main() {
	printf("int main()\n");
	printf("{\n");
}

void gen_outermost_while() {
	printf("\tint opt;\n");
	printf("\tscanf(\"%%d\", &opt);\n");
	printf("\twhile(opt)\n");
	printf("\t{\n");
	printf("\tswitch(opt)\n");
	printf("\t{\n");
}

void begin_boilerplate() {
	gen_header();
	gen_main();
	gen_outermost_while();
}

void close_outermost_while() {
	printf("\tscanf(\"%%d\", &opt);\n");
	printf("\t}\n");
}

void close_outermost_switch() {
	printf("\t}\n");
}

void close_main() {
	printf("}\n");
}

void end_boilerplate() {
	close_outermost_switch();
	close_outermost_while();
	close_main();
}

void gen_case(int i, const char* key) {
	printf("\tcase %d:\n", i);
	printf("\tprintf(\"%s\\n\");\n", key);
}

void close_case() {
	printf("\tbreak;\n");
}

void gen_switch(const char* key) {
	printf("\tswitch(%s) {\n", key);
}

void close_switch() {
	printf("\t}\n");
}

void gen_while(const char *key) {
	printf("\tint %s;\n", key);
	printf("\tscanf(\"%%d\", &%s);\n", key);
	printf("\twhile(%s) {\n", key);
}

void close_while(const char *key) {
	printf("\tscanf(\"%%d\", &%s);\n", key);
	printf("\t}\n");
}

// All siblings should be under one switch statement.
// To traverse siblings of a node, keep moving right
// from that node, if a child is encountered (left != NULL)
// recursively call the function and repeat the same.
void recursive_generate(node_t *root, int count) {
	if(root == NULL) {
		return;
	}
	
	gen_case(count, root->key);
	if(root->left != NULL) {
		gen_while(root->key);
		gen_switch(root->key);
		recursive_generate(root->left, 1);
		close_switch();
		close_while(root->key);
	}
	close_case();
	
	if(root->right) {
		recursive_generate(root->right, count+1);
	}
}

// generate the menu driven code based on the GPT 
// constructed from user input.
void generate_code(tree_t *ptr_tree) {
	recursive_generate(ptr_tree->root, 1);
}

// recursively free memory allocated to nodes.
void recursive_clean(node_t *root) {
	if(root == NULL) {
		return;
	}

	free(root->key);
	recursive_clean(root->left);
	recursive_clean(root->right);
	free(root);
}

// free the memory allocated to the tree.
void clean_tree(tree_t *ptr_tree) {
	recursive_clean(ptr_tree->root);
}

// ground the root of tree as part of cleanup
void ground_tree(tree_t *ptr_tree) {
	ptr_tree->root = NULL;
}
