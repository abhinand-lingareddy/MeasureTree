#include<stdlib.h>
#include<stdio.h>
typedef struct node {
	struct node *left;
	struct node *right;
	int key;
	int height;
} node;
typedef struct text_t {
	struct node*root;
} text_t;
text_t* create_text() {
	text_t *temp = (text_t *) malloc(sizeof(text_t));
	temp->root = NULL;
	return temp;
}

int get_index(node *n) {
	return n->key;
}

bool search(node * temp, int index) {
	while (temp != NULL) {
		if (get_index(temp) == index) {
			return true;
		} else if (get_index(temp) > index) {
			temp = temp->left;
		} else {
			temp = temp->right;
		}
	}
	return false;

}

int max_height(node *a, node* b) {
	if (a == NULL) {
		if (b == NULL)
			return -1;
		return b->height;
	} else if (b == NULL) {
		return a->height;
	} else {
		if (a->height > b->height)
			return a->height;
		else
			return b->height;
	}
}
int height(node *n) {
	n->height = max_height(n->left, n->right) + 1;
	return n->height;
}

node* rotate_left(node * root) {
	node * temp = root;
	root = root->right;
	node * child = root->left;
	root->left = temp;
	temp->right = child;
	height(temp);
	height(root);
	return root;
}

node* rotate_right(node * root) {
	node * temp = root;
	root = root->left;
	node *t = root->right;
	while (t != NULL) {
		t = t->left;
	}
	node * child = root->right;
	root->right = temp;
	temp->left = child;
	height(temp);
	height(root);
	return root;
}
node* balance(node *temp, bool dir, bool pdir) {
	if (dir == pdir) {
		if (dir) {
			return rotate_left(temp);
		} else {
			return rotate_right(temp);
		}
	} else {
		if (dir) {
			temp->right = rotate_right(temp->right);
			return rotate_left(temp);
		} else {
			temp->left = rotate_left(temp->left);
			return rotate_right(temp);
		}
	}
}
int state;
int dir;
int pdir;

bool check_balance(node *n) {
	int left_height, right_height;
	if (n->left != NULL) {
		left_height = n->left->height + 1;
	} else {
		left_height = 0;
	}
	if (n->right != NULL) {
		right_height = n->right->height + 1;
	} else {
		right_height = 0;
	}
	int diff = left_height - right_height;
	if (diff > 1 || diff < -1) {
		return false;
	}
	return true;
}
int check_balance_delete(node *n) {
	int left_height, right_height;
	if (n->left != NULL) {
		left_height = n->left->height + 1;
	} else {
		left_height = 0;
	}
	if (n->right != NULL) {
		right_height = n->right->height + 1;
	} else {
		right_height = 0;
	}
	return left_height - right_height;
}

void post_insert_computation(node *temp, bool cdir) {
	if (state == 0) {
		if (!check_balance(temp)) {
			state = 1;
		}
		pdir = dir;
		dir = cdir;
	} else if (state == 1) {
		if (cdir) {
			temp->right = balance(temp->right, dir, pdir);
		} else {
			temp->left = balance(temp->left, dir, pdir);
		}
		state = 2;
	}
	height(temp);

}
void post_computation_delete(node *temp, bool cdir) {
	if (state == 1) {
		if (cdir) {
			temp->right = balance(temp->right, dir, pdir);
		} else {
			temp->left = balance(temp->left, dir, pdir);
		}
		state = 0;
	}
	if (!check_balance(temp)) {
		node *next;
		if (check_balance_delete(temp) > 0) {
			dir = false;
			next = temp->left;

		} else {
			dir = true;
			next = temp->right;
		}
		if (check_balance_delete(next) > 0) {
			pdir = false;
		} else {
			pdir = true;
		}
		state = 1;
	}
	height(temp);
}

void insert(node* temp, int index, node *new_node) {
	if (get_index(temp) < index) {
		if (temp->right != NULL) {
			insert(temp->right, index, new_node);
			post_insert_computation(temp, true);
		} else {
			temp->right = new_node; //balance
			temp->height = 1;
			dir = true;
		}
	} else {
		if (temp->left != NULL) {
			insert(temp->left, index, new_node);
			post_insert_computation(temp, false);
		} else {
			temp->left = new_node; //balance
			temp->height = 1;
			dir = false;
		}
	}

}
node* delete_current_node(node *n);

void delete_inorder(node *n, node *c) {
	if (c->left->left != NULL) {
		delete_inorder(n, c->left);
		post_computation_delete(c, false);
	} else {
		//copy data
		n->key=c->left->key;
		c->left = delete_current_node(c->left);
		//n->key=n->key+1;
		//free c
	}
}

node* delete_current_node(node *n) {
	if (n->left == NULL) {
		if (n->right == NULL) {
			free(n);
			return NULL;
		} else {
			node * t = n->right;
			free(n);
			return t;
		}
	} else if (n->right == NULL) {
		node *t = n->left;
		free(n);
		return t;
	} else {
		if (n->right->left != NULL) {
			delete_inorder(n, n->right);
		} else {
			//copy data of deltion node
			n->key=n->right->key;
			n->right = delete_current_node(n->right);
		}
		post_computation_delete(n, true);
		return n;
	}
}
void search_delete_node(node* n, int index) {
	if (get_index(n) < index) {
		if (n->right != NULL) {
			if (get_index(n->right) != index) {
				search_delete_node(n->right, index);
				post_computation_delete(n, true);
			} else {
				n->right = delete_current_node(n->right);
				post_computation_delete(n, true);
			}
		}
	} else {
		if (n->left != NULL) {
			if (get_index(n->left) != index) {
				search_delete_node(n->left, index);
				post_computation_delete(n, false);
			} else {
				n->left = delete_current_node(n->left);
				post_computation_delete(n, false);
			}
		}
	}

}
node * get_node(int key) {
	node *temp = (node *) malloc(sizeof(node));
	if (temp != NULL) {
		temp->left = NULL;
		temp->right = NULL;
		temp->key = key;
		temp->height = 0;
	}
	return temp;
}

void intialize_root(text_t *txt, node * new_node) {
	txt->root = new_node;
}
void insert_line(text_t *txt, int index) {
	state = 0;
	node * troot = txt->root;
	node *new_node = get_node(index);
	if (new_node != NULL) {
		if (troot != NULL) {
			insert(troot, index, new_node);
			if (state == 1) {
				txt->root = balance(troot, dir, pdir);
			}
		} else
			intialize_root(txt, new_node);
	}

}

void delete_line(text_t *txt, int index) {
	state = 0;
	node* n = txt->root;
	if (n != NULL) {
		if (get_index(n) == index) {
			txt->root = delete_current_node(n);
		} else {
			search_delete_node(n, index);
		}
		if (state == 1) {
			txt->root = balance(txt->root, dir, pdir);
		}
	}

}
int count=0;
int inorder(node * t) {
	if (t != NULL) {
		count++;
		int hleft = inorder(t->left);
		int hright = inorder(t->right);
		int h;
		if (hleft > hright) {
			h = hleft + 1;
		} else {
			h = hright + 1;
		}
		if (t->height != h) {
			printf("issue at %d \n", t->key);
		}
		int balanced = hleft - hright;
		if (balanced < -1 || balanced > 1) {
			printf("unbalanced at %d\n", t->key);
		}
		return h;
	} else {
		return -1;
	}
}

int main() {
	text_t* t = create_text();
	for (int i = 0; i < 4000000; i++) {
		insert_line(t, i);
	}


	printf("%d\n", t->root->height);
	inorder(t->root);
	printf("done count %d \n",count);
}
