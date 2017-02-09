#include<stdlib.h>
#include<stdio.h>
typedef struct node {
	struct node *left;
	struct node *right;
	int key;
	char *line;
	int height;
	int incre;
} node;
typedef struct text_t {
	struct node*root;
	int length;
} text_t;
text_t* create_text() {
	text_t *temp = (text_t *) malloc(sizeof(text_t));
	temp->length = 0;
	temp->root = NULL;
	return temp;
}
int length_text(text_t *txt) {
	if (txt != NULL)
		return txt->length;
	else
		return -1;
}
int current_increment;
int get_index(node *n) {
	return n->key + current_increment + n->incre;
}
void update_current_incre(node *n) {
	current_increment = current_increment + n->incre;
}
node * search(text_t *txt, int index) {
	current_increment = 0;
	if (txt->length < index) {
		return NULL;
	}
	node * temp = txt->root;
	while (temp != NULL) {
		if (get_index(temp) == index) {
			return temp;
		} else if (get_index(temp) > index) {
			temp = temp->left;
		} else {
			update_current_incre(temp);
			temp = temp->right;
		}
	}

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
	root->incre = root->incre + temp->incre;
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
	root->key = root->key + root->incre;
	node *t = root->right;
	while (t != NULL) {
		t->incre = t->incre + root->incre;
		t = t->left;
	}
	root->incre = 0;
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

void post_computation(node *temp, bool cdir) {
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
void update_key(node *n) {
	n->key = n->key - current_increment;
}
void insert(node* temp, int index, node *new_node) {
	if (get_index(temp) < index) {
		update_current_incre(temp);
		if (temp->right != NULL) {
			insert(temp->right, index, new_node);
			post_computation(temp, true);
		} else {
			update_key(new_node);
			temp->right = new_node; //balance
			temp->height = 1;
			dir = true;
		}
	} else {
		temp->incre = temp->incre + 1;
		if (temp->left != NULL) {
			insert(temp->left, index, new_node);
			post_computation(temp, false);
		} else {
			update_key(new_node);
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
		n->line = c->left->line;
		c->left = delete_current_node(c->left);
		n->key=n->key+1;
		n->incre=n->incre-1;
		//free c
	}
}

node* delete_current_node(node *n) {
	if (n->left == NULL) {
		if (n->right == NULL) {
			// free n
			return NULL;
		} else {
			//free n
			n->right->incre = n->right->incre + n->incre - 1;
			return n->right;
		}
	} else if (n->right == NULL) {
		//free n
		return n->left;
	} else {
		if (n->right->left != NULL) {
			delete_inorder(n, n->right);
		} else {
			n->line = n->right->line;
			n->right = delete_current_node(n->right);
		}
		post_computation_delete(n, true);
		return n;
	}
}
char* delete_node(node* n, int index) {
	char *temp = NULL;
	if (get_index(n) < index) {
		if (n->right != NULL) {
			update_current_incre(n);
			if (get_index(n->right) != index) {
				temp = delete_node(n->right, index);
				post_computation_delete(n, true);
			} else {
				temp = n->right->line;
				n->right = delete_current_node(n->right);
				post_computation_delete(n, true);
			}
		}
	} else {
		n->incre = n->incre - 1;
		if (n->left != NULL) {
			if (get_index(n->left) != index) {
				temp = delete_node(n->left, index);
				post_computation_delete(n, false);
			} else {
				temp = n->left->line;
				n->left = delete_current_node(n->left);
				post_computation_delete(n, false);
			}
		}
	}
	return temp;

}
node * get_node(char *new_line, int key, int *length) {
	node *temp = (node *) malloc(sizeof(node));
	if (temp != NULL) {
		temp->line = new_line;
		temp->left = NULL;
		temp->right = NULL;
		temp->key = key;
		temp->incre = 0;
		temp->height = 0;
		*length = *length + 1;
	}
	return temp;
}

void intialize_root(text_t *txt, node * new_node) {
	txt->root = new_node;
}
void insert_line(text_t *txt, int index, char * new_line) {
	state = 0;
	if (txt->length + 1 < index) {
		index = txt->length + 1;
	}
	current_increment = 0;
	node * temp = txt->root;
	node *new_node = get_node(new_line, index, &(txt->length));
	if (new_node != NULL) {
		if (temp != NULL) {
			insert(temp, index, new_node);
			if (state == 1) {
				txt->root = balance(temp, dir, pdir);
			}
		} else
			intialize_root(txt, new_node);
	}

}
char * get_line(text_t *txt, int index) {
	node *temp = search(txt, index);
	if (temp != NULL) {
		return temp->line;
	}
	return NULL;
}

char * set_line(text_t *txt, int index, char * new_line) {
	node *temp = search(txt, index);
	char *old_line = NULL;
	if (temp != NULL) {
		old_line = temp->line;
		temp->line = new_line;
	}
	return old_line;
}
void append_line(text_t *txt, char * new_line) {
	insert_line(txt, txt->length + 1, new_line);
}

char * delete_line(text_t *txt, int index) {
	state = 0;
	node* n = txt->root;
	if (index > txt->length) {
		return NULL;
	}
	current_increment = 0;
	txt->length = txt->length - 1;
	if (n != NULL) {
		char *t;
		if (get_index(n) == index) {
			t = n->line;
			txt->root = delete_current_node(n);
		} else {
			t = delete_node(n, index);
		}
		if (state == 1) {
			txt->root = balance(txt->root, dir, pdir);
		}
		return t;
	} else {
		return NULL;
	}

}

void inorder(node *n) {
	if (n->left != NULL)
		inorder(n->left);
	printf("%d %s\n", get_index(n), n->line);
	if (n->right != NULL) {
		update_current_incre(n);
		inorder(n->right);
		current_increment = current_increment - n->incre;
	}

}

