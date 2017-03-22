#include<stdlib.h>
#include<stdio.h>
#include <limits.h>
typedef struct node {
	int value;
	int other;
	char which;
	int min;
	int max;
	int left_min;
	int right_max;
	int submeasure;
	int height;
	node* left;
	node* right;
} node;
typedef struct text_t {
	struct node*root;
} text_t;

bool search(node * temp, int index, int other);
int get_submeasure(node* root);

text_t* create_text() {
	text_t *temp = (text_t *) malloc(sizeof(text_t));
	temp->root = NULL;
	return temp;
}
void calculate_min(node* root) {
	if (!root->left) {
		root->min = root->value;
	} else {
		root->min = root->left->min;
	}
}

void calculate_max(node* root) {
	if (!root->right) {
		root->max = root->value;
	} else {
		root->max = root->right->max;
	}
}

int min(int a, int b, int c) {
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int max(int a, int b, int c) {
	return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

void calculate_leftmin(node* root) {
	if (root->which == 'l') {
		root->left_min = min(root->value,
				root->left ? root->left->left_min : INT_MAX,
				root->right ? root->right->left_min : INT_MAX);
	} else {
		root->left_min = min(root->other,
				root->left ? root->left->left_min : INT_MAX,
				root->right ? root->right->left_min : INT_MAX);
	}
}

void calculate_rightmax(node* root) {
	if (root->which == 'l') {
		root->right_max = max(root->other,
				root->left ? root->left->right_max : INT_MIN,
				root->right ? root->right->right_max : INT_MIN);
	} else {
		root->right_max = max(root->value,
				root->left ? root->left->right_max : INT_MIN,
				root->right ? root->right->right_max : INT_MIN);
	}
}

int get_leftmin(node *n) {
	if (n != NULL) {
		return n->left_min;
	} else {
		return INT_MAX;
	}

}

int get_rightmax(node *n) {
	if (n != NULL) {
		return n->right_max;
	} else {
		return INT_MIN;
	}
}
int get_min(node *n) {
	if (n != NULL) {
		return n->min;
	} else {
		return INT_MAX;
	}
}
int get_max(node *n) {
	if (n != NULL) {
		return n->max;
	} else {
		return INT_MIN;
	}
}

void calculate_submeasure(node* root) {
	if (root->which == 'l') {
		//case 1
		if ((get_leftmin(root->right) == get_min(root->right)
				|| get_leftmin(root->right) == root->value)
				&& get_rightmax(root->left) == get_max(root->left)) {
			if (search(root->right, root->other, root->value)) {
				root->submeasure = get_submeasure(root->left)
						+ get_submeasure(root->right) + get_min(root->right)
						- root->value;
			} else {
				root->submeasure = get_submeasure(root->left)
						+ get_max(root->right) - root->value;

			}
		} else if (get_rightmax(root->left) != get_max(root->left)) {
			if (search(root->right, root->other, root->value))
				root->submeasure = get_submeasure(root->left)
						+ get_submeasure(root->right) + get_min(root->right)
						- get_max(root->left);
			else
				root->submeasure = get_submeasure(root->left)
						+ get_max(root->right) - get_max(root->left);
		} else if (get_leftmin(root->right) != get_min(root->right)
				&& get_leftmin(root->right) != root->value
				&& get_rightmax(root->left) == get_max(root->left)) {
			if (search(root->right, root->other, root->value))
				root->submeasure = get_submeasure(root->left)
						+ get_submeasure(root->right) + get_min(root->right)
						- root->value;
			else
				root->submeasure = get_submeasure(root->right)
						+ get_min(root->right) - get_min(root->left);
		}
	} else {
		if ((get_rightmax(root->left) == get_max(root->left)
				|| get_rightmax(root->left) == root->value)
				&& get_leftmin(root->right) == get_min(root->right)) {
			if (search(root->left, root->other, root->value))
				root->submeasure = get_submeasure(root->right)
						+ get_submeasure(root->left) + root->value
						- get_max(root->left);
			else
				root->submeasure = get_submeasure(root->right) + root->value
						- get_min(root->left ? root->left : root);
		} else if (get_leftmin(root->right) != get_min(root->right)) {
			if (search(root->left, root->other, root->value))
				root->submeasure = get_submeasure(root->right)
						+ get_submeasure(root->left) + get_min(root->right)
						- get_max(root->left);
			else
				root->submeasure = get_submeasure(root->right)
						+ get_min(root->right) - get_min(root->left);
		} else if (get_rightmax(root->left) != get_max(root->left)
				&& get_rightmax(root->left) != root->value
				&& get_leftmin(root->right) == get_min(root->right)) {
			if (search(root->left, root->other, root->value))
				root->submeasure = get_submeasure(root->right)
						+ get_submeasure(root->left) + root->value
						- get_max(root->left);
			else
				root->submeasure = get_submeasure(root->left)
						+ get_max(root->right) - get_max(root->left);
		}

	}
}

int get_submeasure(node* root) {
	if (!root) {
		return 0;
	}

	return root->submeasure;
}

int get_index(node *n) {
	return n->value;
}

bool search(node * temp, int index, int other) {
	while (temp != NULL) {
		if (get_index(temp) == index) {
			if (temp->other == other) {
				return true;
			} else {
				return search(temp->left, index, other)
						|| search(temp->right, index, other);
			}
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
	calculate_min(n);
	calculate_max(n);
	calculate_leftmin(n);
	calculate_rightmax(n);
	calculate_submeasure(n);
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
int find_balance(node *n) {
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
	return diff;
}

//state 2 case handle
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
		state = 0;
		if (!check_balance(temp)) {
			state = 1;
		}
		pdir = dir;
		dir = cdir;
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
		if (find_balance(temp) > 0) {
			dir = false;
			next = temp->left;

		} else {
			dir = true;
			next = temp->right;
		}
		if (find_balance(next) > 0) {
			pdir = false;
		} else {
			pdir = true;
		}
		state = 1;
	}
	height(temp);
}
void insert(node* temp, int index, node *new_node);

void insertboth(node* temp, int leftindex, int rightindex, node *new_node,
		node * newnode2) {
	if (get_index(temp) < leftindex && get_index(temp) < rightindex) {
		if (temp->right != NULL) {
			insertboth(temp->right, leftindex, rightindex, new_node, newnode2);
			post_insert_computation(temp, true);
		} else {
			temp->right = new_node; //balance
			new_node->right = newnode2;
			height(new_node);
			height(temp);
			state = 1;
			dir = true;
			pdir = true;
		}
	} else if (get_index(temp) >= leftindex && get_index(temp) >= rightindex) {
		if (temp->left != NULL) {
			insertboth(temp->left, leftindex, rightindex, new_node, newnode2);
			post_insert_computation(temp, false);
		} else {
			temp->left = new_node; //balance
			new_node->right = newnode2;
			height(new_node);
			height(temp);
			state = 1;
			dir = false;
			pdir = true;
		}
	} else {
		//take care of globals
		insert(temp, leftindex, new_node);
		int spdir, sdir;
		bool flag = false;
		if (state == 1) {
			spdir = pdir;
			sdir = dir;
			flag = true;
		}
		state = 0;
		insert(temp, rightindex, newnode2);
		if (state == 1) {
			if (flag) {
				dir = sdir;
				pdir = spdir;
			}

		}

	}
}

void insert(node* temp, int index, node *new_node) {
	if (get_index(temp) < index) {
		if (temp->right != NULL) {
			insert(temp->right, index, new_node);
			post_insert_computation(temp, true);
		} else {
			temp->right = new_node; //balance
			height(temp);
			dir = true;
		}
	} else {
		if (temp->left != NULL) {
			insert(temp->left, index, new_node);
			post_insert_computation(temp, false);
		} else {
			temp->left = new_node; //balance
			height(temp);
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
		n->value = c->left->value;
		c->left = delete_current_node(c->left);
		//n->key=n->key+1;
		//free c
	}
}

node * delete_current_node(node * n) {
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
			n->value = n->right->value;
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
node * get_node(int a, int b) {
	node *temp = (node *) malloc(sizeof(node));
	if (temp != NULL) {
		temp->left = NULL;
		temp->right = NULL;
		temp->value = a;
		temp->height = 0;
		temp->other = b;
		temp->which = a > b ? 'r' : 'l';
		temp->min = temp->max = a;
		temp->submeasure = 0;
		temp->left_min = a < b ? a : b;
		temp->right_max = a < b ? b : a;
	}
	return temp;
}

void intialize_root(text_t *txt, node * new_node, node *new_node2) {
	//calculate other parameters
	txt->root = new_node;
	new_node->right = new_node2;
	height(new_node);
}
void insert_line(text_t *txt, int leftindex, int rightindex) {
	state = 0;
	node * troot = txt->root;
	node *new_node = get_node(leftindex, rightindex);
	node * new_node2 = get_node(rightindex, leftindex);
	if (new_node != NULL) {
		if (troot != NULL) {
			insertboth(troot, leftindex, rightindex, new_node, new_node2);
			if (state == 1) {
				txt->root = balance(troot, dir, pdir);
			}
		} else
			intialize_root(txt, new_node, new_node2);
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
int count = 0;
int inorder(node * t) {
	if (t != NULL) {
		count++;
		int hleft = inorder(t->left);
		printf("value %d ",t->value);
				printf("measure %d ", t->submeasure);
					printf("min %d ", t->min);
					printf("max %d ", t->max);
					printf("leftmin %d ", t->left_min);
					printf("rightmax %d \n", t->right_max);
		int hright = inorder(t->right);
		int h;
		if (hleft > hright) {
			h = hleft + 1;
		} else {
			h = hright + 1;
		}
		if (t->height != h) {
			printf("issue at %d \n", t->value);
		}
		int balanced = hleft - hright;
		if (balanced < -1 || balanced > 1) {
			printf("unbalanced at %d\n", t->value);
		}

		return h;
	} else {
		return -1;
	}
}

int main() {
	text_t* t = create_text();
	insert_line(t, 1, 2);
	insert_line(t, 5, 10);
	//insert_line(t,-11,7);

	printf("%d\n", t->root->height);
	inorder(t->root);
	printf("done count %d \n", count);
	printf("measure %d \n", t->root->submeasure);
	printf("min %d \n", t->root->min);
	printf("max %d \n", t->root->max);
	printf("leftmin %d \n", t->root->left_min);
	printf("rightmax %d \n", t->root->right_max);

}
