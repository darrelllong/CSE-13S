#include "bst.h"
#include "node.h"
#include "printer.h"
#include <stdio.h>

int main(void) {
    int key = 0;
    Node *root = bst_create();

    while (printf("Insert: ") && scanf("%d", &key) && key >= 0) {
        root = bst_insert(root, key);
        printf("\n");
        binary_tree_print(root, node_print);
        printf("\n");
        printf("Height: %d\n", bst_height(root));
        printf("Balanced: %s\n", bst_balanced(root) ? "yes" : "no");
    }

    while (printf("Remove: ") && scanf("%d", &key) && key >= 0) {
        root = bst_remove(root, key);
        printf("\n");
        binary_tree_print(root, node_print);
        printf("\n");
        printf("Height: %d\n", bst_height(root));
        printf("Balanced: %s\n", bst_balanced(root) ? "yes" : "no");
    }

    while (printf("Find: ") && scanf("%d", &key) != EOF && key >= 0) {
        printf("%d %s\n", key, bst_find(root, key) ? "found" : "not found");
    }

    bst_delete(&root);
    return 0;
}
