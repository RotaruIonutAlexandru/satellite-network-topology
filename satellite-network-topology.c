/* ROTARU Ionut-Alexandru - 313CD */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int freq;
    char *name;
    struct node *left, *right;
} Node, *TBinaryTree;

typedef struct heap {
    TBinaryTree *vector;
    int size;
    int capacity;
    int (*compare_func)(const void*, const void*);
} *Heap;

/* Initializează un heap cu capacitatea dată și funcție de comparare */
Heap initHeap(int capacity, int (*compare_func)(const void*, const void*)) {
    Heap h = malloc(sizeof(struct heap));
    if (!h) return NULL;
    h->size = 0;
    h->capacity = capacity;
    h->vector = malloc(capacity * sizeof(TBinaryTree));
    if (!h->vector) { free(h); return NULL; }
    h->compare_func = compare_func;
    return h;
}

/* Coborâre a unui element în heap pentru menținerea proprietății */
Heap siftDown(Heap h, int index) {
    int maxIndex = index;
    int l = index * 2 + 1;
    if (l < h->size && h->compare_func(&h->vector[l], &h->vector[maxIndex]) > 0)
        maxIndex = l;
    int r = index * 2 + 2;
    if (r < h->size && h->compare_func(&h->vector[r], &h->vector[maxIndex]) > 0)
        maxIndex = r;
    if (index != maxIndex) {
        TBinaryTree aux = h->vector[index];
        h->vector[index] = h->vector[maxIndex];
        h->vector[maxIndex] = aux;
        h = siftDown(h, maxIndex);
    }
    return h;
}

/* Urcare a unui element în heap pentru menținerea proprietății */
Heap siftUp(Heap h, int index) {
    while (index > 0 && h->compare_func(&h->vector[(index - 1) / 2], &h->vector[index]) < 0) {
        TBinaryTree aux = h->vector[(index - 1) / 2];
        h->vector[(index - 1) / 2] = h->vector[index];
        h->vector[index] = aux;
        index = (index - 1) / 2;
    }
    return h;
}

/* Inserează un element în heap, redimensionează dacă e necesar */
Heap insertHeap(Heap h, TBinaryTree element) {
    if (!h) return NULL;
    if (h->size == h->capacity) {
        h->capacity *= 2;
        TBinaryTree *newv = realloc(h->vector, sizeof(TBinaryTree) * h->capacity);
        if (!newv) return NULL;
        h->vector = newv;
    }
    h->vector[h->size] = element;
    h = siftUp(h, h->size);
    h->size++;
    return h;
}

/* Extrage nodul cu frecvența minimă și rearanjează heap-ul */
TBinaryTree extractMin(Heap h) {
    if (!h || h->size == 0) return NULL;
    TBinaryTree ret = h->vector[0];
    h->vector[0] = h->vector[--h->size];
    siftDown(h, 0);
    return ret;
}

/* Eliberează memoria alocată pentru heap */
Heap freeHeap(Heap h) {
    if (h) {
        free(h->vector);
        free(h);
    }
    return NULL;
}

/* Compară două noduri: frecvență mai mică are prioritate, la egalitate ordine alfabetică inversă */
int compare_nodes(const void *pa, const void *pb) {
    TBinaryTree na = *(TBinaryTree *)pa;
    TBinaryTree nb = *(TBinaryTree *)pb;
    if (na->freq < nb->freq) return 1;
    if (na->freq > nb->freq) return -1;
    return strcmp(nb->name, na->name);
}

/* Creează un nod frunză cu frecvența și numele dat */
TBinaryTree make_leaf(int f, const char *s) {
    TBinaryTree n = malloc(sizeof(*n));
    if (!n) return NULL;
    n->freq = f;
    size_t len = strlen(s) + 1;
    n->name = malloc(len);
    if (!n->name) { free(n); return NULL; }
    strcpy(n->name, s);
    n->left = n->right = NULL;
    return n;
}

/* Creează un nod intern cu cei doi copii, frecvența este suma, numele este concatenarea */
TBinaryTree make_internal(TBinaryTree a, TBinaryTree b) {
    TBinaryTree parent = malloc(sizeof(*parent));
    if (!parent) return NULL;
    parent->freq = a->freq + b->freq;
    parent->name = malloc(strlen(a->name) + strlen(b->name) + 1);
    if (!parent->name) { free(parent); return NULL; }
    strcpy(parent->name, a->name);
    strcat(parent->name, b->name);
    parent->left = a;
    parent->right = b;
    return parent;
}

/* Eliberează recursiv întregul arbore */
TBinaryTree freeTree(TBinaryTree root) {
    if (!root) return NULL;
    freeTree(root->left);
    freeTree(root->right);
    free(root->name);
    free(root);
    return NULL;
}

/* Calculează înălțimea arborelui */
int height(TBinaryTree root) {
    if (!root || (!root->left && !root->right))
        return 0;
    int left = height(root->left);
    int right = height(root->right);
    int max = left;
    if (right > left) {
        max = right;
    }
    return max + 1;
}

/* Afișează nodurile de la un nivel dat */
void printLevel(TBinaryTree root, int level) {
    if (!root) return;
    if (level == 0) {
        printf("%d-%s ", root->freq, root->name);
        return;
    }
    printLevel(root->left,  level - 1);
    printLevel(root->right, level - 1);
}

/* Parcurgere pe niveluri */
void printLevelOrder(TBinaryTree root) {
    int h = height(root);
    for (int i = 0; i <= h; ++i) {
        printLevel(root, i);
        printf("\n");
    }
}

/* Cerința 1: Construiește arborele și afișează nivelurile */
void cerinta1(const char *infile, const char *outfile) {
    freopen(infile, "r", stdin);
    freopen(outfile, "w", stdout);
    int N;
    scanf("%d", &N);

    Heap h = initHeap(N, compare_nodes);
    if (!h) return;

    for (int i = 0; i < N; ++i) {
        int f;
        char buf[32];
        scanf("%d %31s", &f, buf);
        TBinaryTree leaf = make_leaf(f, buf);
        if (!leaf) { freeHeap(h); return; }
        h = insertHeap(h, leaf);
        if (!h) return;
    }
    while (h->size > 1) {
        TBinaryTree a = extractMin(h);
        TBinaryTree b = extractMin(h);
        TBinaryTree parent = make_internal(a, b);
        h = insertHeap(h, parent);
        if (!h) return;
    }

    TBinaryTree root = (h->size >= 1) ? h->vector[0] : NULL;
    freeHeap(h);

    printLevelOrder(root);
    freeTree(root);
}

/* Cerința 2: Decodifică secvențe binare în nume de sateliți */
void cerinta2(const char *infile, const char *outfile) {
    freopen(infile, "r", stdin);
    freopen(outfile, "w", stdout);

    int N;
    scanf("%d", &N);
    Heap h = initHeap(N, compare_nodes);
    for (int i = 0; i < N; ++i) {
        int f;
        char buf[32];
        scanf("%d %31s", &f, buf);
        TBinaryTree leaf = make_leaf(f, buf);
        insertHeap(h, leaf);
    }
    while (h->size > 1) {
        TBinaryTree a = extractMin(h);
        TBinaryTree b = extractMin(h);
        TBinaryTree parent = make_internal(a, b);
        insertHeap(h, parent);
    }
    TBinaryTree root = h->vector[0];
    freeHeap(h);

    int M;
    scanf("%d", &M);
    for (int i = 0; i < M; ++i) {
        char cod[1001];
        scanf("%1000s", cod);
        TBinaryTree current = root;
        char **results = malloc(1000 * sizeof(char *));
        int count = 0;

        for (char *p = cod; *p; ++p) {
            if (*p == '0') {
                current = current->left;
            } else if (*p == '1') {
                current = current->right;
            }
            if (!current) break;

            if (!current->left && !current->right) {
                results[count] = malloc(strlen(current->name) + 1);
                strcpy(results[count], current->name);
                count++;
                current = root;
            }
        }

        for (int j = 0; j < count; ++j) {
            char sep;
            if (j == count - 1) {
                sep = '\n';
            } else {
                sep = ' ';
            }
            printf("%s%c", results[j], sep);
            free(results[j]);
        }
        free(results);
    }

    freeTree(root);
}

/* Caută calea pentru un nume în arbore, returnează 1 dacă îl găsește */
int find_path(TBinaryTree node, const char *target, char *path, int depth) {
    if (!node) return 0;
    if (node->left == NULL && node->right == NULL) {
        if (strcmp(node->name, target) == 0) {
            path[depth] = '\0';
            return 1;
        }
        return 0;
    }
    if (find_path(node->left, target, path, depth + 1)) {
        path[depth] = '0';
        return 1;
    }
    if (find_path(node->right, target, path, depth + 1)) {
        path[depth] = '1';
        return 1;
    }
    return 0;
}

/* Cerința 3: Concatenează codurile numerelor cerute */
void cerinta3(const char *infile, const char *outfile) {
    freopen(infile, "r", stdin);
    freopen(outfile, "w", stdout);

    int N;
    scanf("%d", &N);
    Heap h = initHeap(N, compare_nodes);
    for (int i = 0; i < N; ++i) {
        int f;
        char buf[32];
        scanf("%d %31s", &f, buf);
        TBinaryTree leaf = make_leaf(f, buf);
        insertHeap(h, leaf);
    }
    while (h->size > 1) {
        TBinaryTree a = extractMin(h);
        TBinaryTree b = extractMin(h);
        TBinaryTree parent = make_internal(a, b);
        insertHeap(h, parent);
    }
    TBinaryTree root = h->vector[0];
    freeHeap(h);

    int K;
    scanf("%d", &K);
    char full_code[10000] = "";
    for (int i = 0; i < K; ++i) {
        char name[16];
        scanf("%15s", name);
        char path[1000];
        if (find_path(root, name, path, 0)) {
            strcat(full_code, path);
        }
    }
    printf("%s\n", full_code);
    freeTree(root);
}

/* Găsește cel mai apropiat strămoș comun pentru un set de noduri */
TBinaryTree find_lca(TBinaryTree root, char **names, int count) {
    if (!root) return NULL;
    for (int i = 0; i < count; ++i) {
        if (strcmp(root->name, names[i]) == 0) {
            return root;
        }
    }
    TBinaryTree left = find_lca(root->left, names, count);
    TBinaryTree right = find_lca(root->right, names, count);
    if (left && right) return root;
    return left ? left : right;
}

/* Cerința 4: Determină LCA pentru nodurile date */
void cerinta4(const char *infile, const char *outfile) {
    freopen(infile, "r", stdin);
    freopen(outfile, "w", stdout);

    int N;
    scanf("%d", &N);
    Heap h = initHeap(N, compare_nodes);
    for (int i = 0; i < N; ++i) {
        int f;
        char buf[32];
        scanf("%d %31s", &f, buf);
        TBinaryTree leaf = make_leaf(f, buf);
        insertHeap(h, leaf);
    }
    while (h->size > 1) {
        TBinaryTree a = extractMin(h);
        TBinaryTree b = extractMin(h);
        TBinaryTree parent = make_internal(a, b);
        insertHeap(h, parent);
    }
    TBinaryTree root = h->vector[0];
    freeHeap(h);

    int K;
    scanf("%d", &K);
    char **names = malloc(K * sizeof(char *));
    for (int i = 0; i < K; ++i) {
        names[i] = malloc(16);
        scanf("%15s", names[i]);
    }
    TBinaryTree lca = find_lca(root, names, K);
    printf("%s\n", lca ? lca->name : "N/A");
    for (int i = 0; i < K; ++i) free(names[i]);
    free(names);
    freeTree(root);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s -cX infile outfile\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-c1") == 0) {
        cerinta1(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-c2") == 0) {
        cerinta2(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-c3") == 0) {
        cerinta3(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-c4") == 0) {
        cerinta4(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Unknown option %s\n", argv[1]);
        return 1;
    }
    return 0;
}