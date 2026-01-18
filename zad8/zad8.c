#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

typedef struct node* tree;
typedef struct node* Position;
typedef struct node {
	int el;
	tree L;
	tree R;

}node;

int main() {

	tree root = NULL;
	int choice = 0, x;

	while (choice != 7) {
		printf("\n1 Insert");
		printf("\n2 Search");
		printf("\n3 Delete");
		printf("\n4 Inorder");
		printf("\n5 Preorder");
		printf("\n6 Postorder");
		printf("\n7 Exit");
		printf("\nIzbor: ");
		scanf("%d", &choice);

			if (choice == 1) {
				scanf("%d", &x);
				root = insert(root, x);
			}
			else if (choice == 2) {
				scanf("%d", &x);
				if (search(root, x))
					printf("Postoji\n");
				else
					printf("Ne postoji\n");
			}
			else if (choice == 3) {
				scanf("%d", &x);
				root = deleteEl(root, x);
			}
			else if (choice == 4) {
				inorder(root);
			}
			else if (choice == 5) {
				preorder(root);
			}
			else if (choice == 6) {
				postorder(root);
			}
		}
		return 0;
	}

Position createNode(int el) {
	Position q = malloc(sizeof(node));
	q->el = el;
	q->L = NULL;
	q->R = NULL;
	return q;
}

tree insert(tree root, int el) {
	if (root = NULL)
		return createNode(el);

	if (el < root->el)
		root->L = insert(root->L, el);
	else if (el > root->el)
		root->R = insert(root->R, el);

	return root;
}
Position search(tree root, int el) {
	if (root == NULL || root->el == el)
		return root;

	if (el < root->el)
		return search(root->L, el);

	return search(root->R, el);

}

tree deleteEl(tree root, int el) {
	Position temp;
	Position parent;

	if (root == NULL)
		return root;

	if (el < root->el) {
		root->L = deleteEl(root->L, el);
	}
	else if (el > root->el) {
		root->R = deleteEl(root->R, el);
	}
	else {
		if (root->L = NULL) {
			temp = root->R;
			free(root);
			return temp;
		}
		if (root->R = NULL) {
			temp = root->L;
			free(root);
			return temp;
		}
		parent = root;
		temp = root->R;

		while (temp->L != NULL) {
			parent = temp;
			temp = temp->L;
		}
		root->el = temp->el;

		if (parent == root)
			parent->R = temp->R;
		else
			parent->L = temp->R;

		free(temp);

	}
	return root;
}
int inorder(tree root) {
	if (root == NULL)
		return 0;

	inorder(root->L);
	printf("%d ", root->el);
	inorder(root->R);
	return 1;
}
int preorder(tree root) {
	if (root == NULL)
		return 0;

	printf("%d ", root->el);
	preorder(root->L);
	preorder(root->R);
	return 1;
}
int postorder(tree root) {
	if (root == NULL)
		return 0;

	postorder(root->L);
	postorder(root->R);
	printf("%d ", root->el);
	return 1;
}

