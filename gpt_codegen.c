#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gpt.h"

int main() {
	tree_t t;
	init_tree(&t);
	assert(check_root(&t));
	char ch;

	// generate header files and main function and while loop
	begin_boilerplate();

	// keep track of the latest key at a particular tab.
	// 1024 because each line is constrained to a length of 1024
	// and assuming 1024 tabs.
	char** tabs = (char**)malloc(1024*sizeof(char*));
	for(int i = 0; i < 1024; ++i) {
		tabs[i] = (char*)malloc(1025*sizeof(char));
	}

	int tab_count = 0;
	int i = 0;
	char* item = (char*)malloc(1025*sizeof(char));
	while((ch = getchar())!=EOF) {	
		if(ch == '\n') {
			item[i] = '\0';

			// if tab count is 0, a new root is to be added.
			if(tab_count == 0) {
				add_node(&t, " ", item);
				strcpy(tabs[0], item);
			}

			// If tab count is greater than 0 then a non-root node is added.
			// The parent of the node to be added will be the latest node with tab count 
			// as one less than the current node.
			else {
				add_node(&t, tabs[tab_count-1], item);
				strcpy(tabs[tab_count], item);
			}
			tab_count = 0;
			i = 0;
			free(item);
			item = (char*)malloc(1025*sizeof(char));
		}

		else if(ch == '\t') {
			++tab_count;
		}
		else {
			item[i] = ch;
			++i;
		}
	}

	// generate the menu driven code.
	generate_code(&t);

	// close the parts of the code which are not generated using the GPT.
	end_boilerplate();

	// cleanup
	for(int i=0; i<1024; ++i) {
		free(tabs[i]);
	}
	
	free(tabs);
	free(item);
	clean_tree(&t);
	ground_tree(&t);
	assert(check_root(&t));

	return 0;
}
