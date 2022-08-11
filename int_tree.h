typedef struct node{
    char* low;
    char* high;
    char* max;
    char* info;
    int colour;  // 0 for black, 1 for red, -1 for double black
    struct node* left;
    struct node* right;
    struct node* parent;
}node;

typedef node* Intv_tree;

void init(Intv_tree* t);
void insert(Intv_tree* t, char* l, char* h, char* info);
int hr_to_min(char* s);
int str_to_int(char* eq);
char* int_to_str(int num);
void inorder(Intv_tree t, char* filename);
void left_left(Intv_tree* t, Intv_tree n);
void right_right(Intv_tree* t, Intv_tree n);
void left_right(Intv_tree* t, Intv_tree n);
void right_left(Intv_tree* t, Intv_tree n);
node* find_uncle(node* a);
char* find_greater(Intv_tree* t, node* x);
int max3(int a, int b, int c);
void change_maximum(Intv_tree* new);
char* find_maximum(Intv_tree t);
void insert_node(Intv_tree* t, char* l, char* h, char* info);
node* find_node(Intv_tree t, char* l);
node* overlap(node* t, int l, int h);
node* find_overlap(node* t, char* l, char* h);
int overlap_timing(node* t, char* l, char* h);
void eval_overlap_timing(node* t, char* l, char* h, char* info, char* filename);
void construct_intTree(Intv_tree* t1, Intv_tree* t2, Intv_tree* t3, Intv_tree* t4, char* filename);
void display_schedule(Intv_tree t, char* filename);
void inorder_trav(Intv_tree t);
void do_opt_file(Intv_tree* t1, Intv_tree* t2, Intv_tree* t3, Intv_tree* t4, char* filename, char* low, char* high, char* infot, char* prev_filename);
void destroy_tree(Intv_tree* t);
int is_equal(char* x, char* y);