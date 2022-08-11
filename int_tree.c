# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "int_tree.h"

void init(Intv_tree* t){
    *t = NULL;
    return;
}

void insert(Intv_tree* t, char* l, char* h, char* info){
    // printf("inside insert func");
    node* nn = (node*)malloc(sizeof(node));
    nn->low = l; 
    nn->high = h; 
    nn->max = h;
    nn->info = info;
    // strcpy(nn->low, l);
    // strcpy(nn->high, h);
    // strcpy(nn->max, h);
    // strcpy(nn->info, info);
    // printf("%s %s %s\n", nn->info, nn->low, nn->high);
    nn->colour = 1;
    nn->left = NULL; nn->right = NULL; nn->parent = NULL;

    if (*t == NULL){
        *t = nn; 
        nn->colour = 0; return;
    }
    
    node* p = *t;
    node* q = NULL;
    while(p != NULL){
        int time = hr_to_min(p->low);
        int new_time = hr_to_min(l);
        q = p;
        if (new_time < time) p = p->left;
        else if (new_time > time) p = p->right;
        else return;
    }
    int time = hr_to_min(q->low);
    int new_time = hr_to_min(l);
    if (new_time < time) q->left = nn;
    else if (new_time > time) q->right = nn;
    nn->parent = q;

    // change_maximum(&q);

    while (nn->parent != NULL){
        if (nn->colour == 0 && (q->colour == 0 || q->colour == 1)){
            change_maximum(&q);
            return;
        }
        else if (nn->colour == 1 && q->colour == 0){
            change_maximum(&q);
            return;
        }
        else{
            node* u = find_uncle(nn);
            // Case 1 : if uncle is red
            if (u != NULL && u->colour == 1){
                if (q->colour == 0) return;
                u->colour = 0; nn->parent->colour = 0;
                if (u->parent != *t){
                    u->parent->colour = 1;
                }
                change_maximum(&q);
                nn = u->parent;
                q = nn->parent;
                
            }
            // Case 2 : if uncle is black or NULL
            else if (u == NULL || u->colour == 0){
                node* grf = nn->parent->parent;

                int nn_time = hr_to_min(nn->low);
                int parent_time = hr_to_min(nn->parent->low);
                int grf_time = hr_to_min(grf->low);

                if (nn_time < grf_time && nn_time > parent_time){
                    node* par = nn->parent;
                    left_right(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->colour == 0) nn->colour = 1;
                    else nn->colour = 0;
                    change_maximum(&par);
                    change_maximum(&grf);
                    q = nn->parent;
                }
                else if (nn_time > grf_time && nn_time < parent_time){
                    node* par = nn->parent;
                    right_left(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->colour == 0) nn->colour = 1;
                    else nn->colour = 0;
                    change_maximum(&par);
                    change_maximum(&grf);
                    q = nn->parent;
                }
                else if (nn_time < grf_time && nn_time < parent_time){
                    left_left(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->parent->colour == 0) nn->parent->colour = 1;
                    else nn->parent->colour = 0;
                    change_maximum(&grf);
                    nn = nn->parent; 
                    q = nn->parent;
                }
                else if (nn_time > grf_time && nn_time > parent_time){
                    right_right(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->parent->colour == 0) nn->parent->colour = 1;
                    else nn->parent->colour = 0;
                    change_maximum(&grf);
                    nn = nn->parent; 
                    q = nn->parent;
                }
            }
        }
    }
    return;
}

int hr_to_min(char* s){
    int i = 0, j = 0;;
    char* hr = (char*)malloc(sizeof(char) * 3);
    char* min = (char*)malloc(sizeof(char) * 3);
    while(s[i] != ':'){
        hr[i] = s[i];
        i++;
    }
    hr[i] = '\0';
    i++;
    int hour = str_to_int(hr);
    while(s[i] != '\0'){
        min[j] = s[i];
        i++; j++;
    }
    min[j] = '\0';
    int minute = str_to_int(min);
    return hour * 60 + minute;
}

int str_to_int(char* eq){
    int t = 0, i = 0;
    while(eq[i] != '\0'){
      t *= 10;
      t = t + (eq[i] - '0');
      i++;
    }
    return t;
}

char* int_to_str(int num){
    int i = 0, j = 0;
    int *arr = (int*)malloc(sizeof(int) * 10);
    while(num != 0){
      int z = num % 10;
      arr[j] = z;
      num /= 10;
      i++; j++;
    }
    char* ans = (char*)malloc(sizeof(char) * i+1);
    for (int k=i-1;k>=0;k--){
      ans[k] = arr[i-k-1] + '0';
    }
    ans[i] = '\0';
    return ans;
}

void inorder(Intv_tree t, char* filename){
    if (t==NULL){
        return;
    }
    // printf("%s - %s : %s : %d\n", t->low, t->high, t->max, t->colour);
    inorder(t->left, filename);
    // printf("%s : From %s to %s\n", t->info, t->low, t->high);
    FILE * fp;
    fp = fopen(filename, "a");
    if (fp == NULL) printf("cant open\n");
    else{
        fprintf(fp, "%s : From %s to %s\n", t->info, t->low, t->high);
    }
    fclose(fp);
    inorder(t->right, filename);  
}

node* find_uncle(node* a){
    node* grf = a->parent->parent;
    if (grf != NULL){
        int a_time = hr_to_min(a->low);
        int grf_time = hr_to_min(grf->low);
        if (a_time < grf_time) return grf->right;
        else return grf->left;
    }
}

void left_left(Intv_tree* t, Intv_tree n){
    Intv_tree A = n;
    Intv_tree B = n->left;
    A->left = B->right;
    if (B->right != NULL) B->right->parent = A;
    B->right = A;
    if (A == *t) *t = B;
    else{
        if (A->parent->left == A) A->parent->left = B;
        else A->parent->right = B;
    }
    B->parent = A->parent;
    A->parent = B;
}

void right_right(Intv_tree* t, Intv_tree n){
    Intv_tree A = n;
    Intv_tree B = n->right;
    A->right = B->left;
    if (B->left != NULL) B->left->parent = A;
    B->left = A;
    if (A == *t) *t = B;
    else{
        if (A->parent->left == A) A->parent->left = B;
        else A->parent->right = B;
    }
    B->parent = A->parent;
    A->parent = B;
}

void left_right(Intv_tree* t, Intv_tree n){
    right_right(t, n->left);
    left_left(t, n);
}

void right_left(Intv_tree* t, Intv_tree n){
    left_left(t, n->right);
    right_right(t, n);
}

int max3(int a, int b, int c){
    if (a >= b && a >= c) 
        return a;
    else if (b >= a && b >= c)
        return b;
    else
        return c;
}

void change_maximum(Intv_tree* new){
    if (*new == NULL)
        return;
    else{
        (*new)->max = find_maximum(*new);
        change_maximum(&(*new)->parent);
    }
    return;
}

char* find_maximum(Intv_tree t){
    if (t != NULL){
        if (t->left == NULL && t->right == NULL)
            return t->high;
        else{
            if (t->left != NULL && t->right != NULL){
                char* l = find_maximum(t->left);
                char* r = find_maximum(t->right);
                int num = hr_to_min(t->max);
                int numl = hr_to_min(l);
                int numr = hr_to_min(r);
                int final = max3(num, numl, numr);
                if (final == hr_to_min(l))
                    return t->left->max;
                else if (final == hr_to_min(r))
                    return t->right->max;
                else
                    return t->max;
            }
            else if (t->left != NULL && t->right == NULL){
                char* l = find_maximum(t->left);
                int num = hr_to_min(t->max);
                int numl = hr_to_min(l);
                if (num < numl)
                    return t->left->max;
                else
                    return t->max;
            }
            else if (t->left == NULL && t->right != NULL){
                char* r = find_maximum(t->right);
                int num = hr_to_min(t->max);
                int numr = hr_to_min(r);
                if (num < numr)
                    return t->right->max;
                else
                    return t->max;
            }
        }
    }
}

// void insert_node(Intv_tree* t, char* l, char* h, char* info){
//     // printf("%s %s %s\n", info, l, h);
//     if (*t == NULL){
//         insert(t, l, h, info);
//         return;
//     }
//     insert(t, l, h, info);
//     node* p = find_node(*t, l);
//     if (find_overlap(*t, l, h) != NULL){
//         eval_overlap_timing(p->parent, l, h);
//         printf("\n"); 
//     } 
// }

node* find_node(Intv_tree t, char* l){
    if (t != NULL){
        if (t->low == l)
            return t;
        else if (hr_to_min(l) < hr_to_min(t->low))
            return find_node(t->left, l);
        else
            return find_node(t->right, l);
    }
}

node* overlap(node* t, int l, int h){
    if (t == NULL)
        return NULL;
    if (hr_to_min(t->low) == l && hr_to_min(t->high) == h)
        return NULL;
    if (hr_to_min(t->low) < h && l < hr_to_min(t->high))
        return t;
    if (t->left != NULL && hr_to_min(t->left->max) >= l)
        return overlap(t->left, l, h);
    else
        return overlap(t->right, l, h);
}

node* find_overlap(node* t, char* l, char* h){
    // printf("inside findoverlap func\n");
    int low = hr_to_min(l);
    int high = hr_to_min(h);
    return overlap(t, low, high);
}

int overlap_timing(node* t, char* l, char* h){
    int answer;
    int low = hr_to_min(l);
    int high = hr_to_min(h);
    // case 1:
    // 1st interval partially overlaps the 2nd one 

    // case a:
    // 1st interval to the left of 2nd one 
    if (hr_to_min(t->low) < low && hr_to_min(t->high) <= high)
        answer = hr_to_min(t->high) - low;

    // case b:
    // 1st interval to the right of 2nd one 
    else if (hr_to_min(t->low) >= low && hr_to_min(t->high) > high)
        answer = high - hr_to_min(t->low);

    // case 2:
    // 1st interval completly overlaps the 2nd one 

    // case a:
    // 1st interval larger than 2nd
    else if (hr_to_min(t->low) < low && hr_to_min(t->high) > high)
        answer = high - low;

    // case b:
    // 1st interval smaller than 2nd
    else    
        answer = hr_to_min(t->high) - hr_to_min(t->low);

    return answer;
}

void eval_overlap_timing(node* t, char* l, char* h, char* info, char* filename){
    // printf("inside evaloverlap func\n");
    int final = overlap_timing(t, l, h);
    FILE * fp;
    fp = fopen(filename, "a");
    if (fp == NULL){
        printf("cant open\n");
        return;
    }
    if (final < 60)
        // printf("Overlap of %d minutes between %s to %s and %s to %s", final, t->low, t->high,
        // l, h);
        fprintf(fp, "Overlap of %d minutes between %s to %s, %s meet and %s to %s, %s\n\n\n", final, t->low, t->high, t->info, l, h, info);
    else{
        int hr = final/60;
        int min = final - (hr * 60);
        if (min != 0)
            // printf("Overlap of %d hours, %d minutes between %s to %s and %s to %s", hr, min, t->low,
            // t->high, l, h);
            fprintf(fp, "Overlap of %d hours, %d minutes between %s to %s, %s meet and %s to %s, %s\n\n\n", hr, min, t->low, t->high, t->info, l, h, info);
        else
            // printf("Overlap of %d hours between %s to %s and %s to %s", hr, t->low,
            // t->high, l, h);
            fprintf(fp, "Overlap of %d hours between %s to %s, %s meet and %s to %s, %s\n\n\n", hr, t->low, t->high, t->info, l, h, info);
    }
    fclose(fp);
    return;
}

int is_equal(char* x, char* y){
    int l1 = strlen(x);
    int l2 = strlen(y);
    if (l1 == l2){
        for (int i=0;i<l1;i++){
            if (x[i] != y[i])
                return 0;
        }
        return 1;
    }
    return 0;
}

void do_opt_file(Intv_tree* t1, Intv_tree* t2, Intv_tree* t3, Intv_tree* t4, char* filename, char* low, char* high, char* infot, char* prev_filename){
    // printf("inside oper func\n");
    FILE* fp;
    char* ch;
    char c;
    fp = fopen(filename, "r");
    if (fp == NULL) printf("cant open");
    int line = 1;
    while(c != EOF){
        c = getc(fp);
        if (c == '\n')
            line++;
    }
    rewind(fp);
    char  numid[5], info[50], l[50], h[50];
    for(int i=0;i<line;i++){
        fscanf(fp, "%s %s %s %s", numid, info, l, h);
        char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
        char* infor = (char*)malloc(sizeof(char) * strlen(info));
        char* low = (char*)malloc(sizeof(char) * strlen(l));
        char* high = (char*)malloc(sizeof(char) * strlen(h));
        strcpy(num_id, numid);
        strcpy(infor, info);
        strcpy(low, l);
        strcpy(high, h);

        if (filename == "t1.txt")
            insert(t1, low, high, infor);
        else if (filename == "t2.txt")
            insert(t2, low, high, infor);
        else if (filename == "t3.txt")
            insert(t3, low, high, infor);
        else if (filename == "t4.txt")
            insert(t4, low, high, infor);
    }
    
    node* ol;
    if (filename == "t1.txt")
        ol = find_overlap(*t1, low, high);
    else if (filename == "t2.txt")
        ol = find_overlap(*t2, low, high);
    else if (filename == "t3.txt")
        ol = find_overlap(*t3, low, high);
    else if (filename == "t4.txt")
        ol = find_overlap(*t4, low, high);

    // printf("%s %s\n", prev_filename, filename);

    if (ol != NULL){
        // if (prev_filename == "t1.txt" && filename == "t2.txt"){
        if (is_equal(prev_filename, "t1.txt")){
            FILE* fp;
            char* ch;
            char c;
            fp = fopen("t1.txt", "r");
            if (fp == NULL) printf("cant open");
            int line = 1;
            while(c != EOF){
                c = getc(fp);
                if (c == '\n')
                    line++;
            }
            rewind(fp);
            char  numid[5], info[50], l[50], h[50];
            for(int i=0;i<line;i++){
                fscanf(fp, "%s %s %s %s", numid, info, l, h);
                char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
                char* infor = (char*)malloc(sizeof(char) * strlen(info));
                char* low = (char*)malloc(sizeof(char) * strlen(l));
                char* high = (char*)malloc(sizeof(char) * strlen(h));
                strcpy(num_id, numid);
                strcpy(infor, info);
                strcpy(low, l);
                strcpy(high, h);
                insert(t1, low, high, infor);
            }
            // inorder_trav(*t1);
        }
        if (is_equal(prev_filename, "t2.txt")){
            FILE* fp;
            char* ch;
            char c;
            fp = fopen("t2.txt", "r");
            if (fp == NULL) printf("cant open");
            int line = 1;
            while(c != EOF){
                c = getc(fp);
                if (c == '\n')
                    line++;
            }
            rewind(fp);
            char  numid[5], info[50], l[50], h[50];
            for(int i=0;i<line;i++){
                fscanf(fp, "%s %s %s %s", numid, info, l, h);
                char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
                char* infor = (char*)malloc(sizeof(char) * strlen(info));
                char* low = (char*)malloc(sizeof(char) * strlen(l));
                char* high = (char*)malloc(sizeof(char) * strlen(h));
                strcpy(num_id, numid);
                strcpy(infor, info);
                strcpy(low, l);
                strcpy(high, h);
                insert(t2, low, high, infor);
            }
            // inorder_trav(*t1);
        }
        if (is_equal(prev_filename, "t3.txt")){
            FILE* fp;
            char* ch;
            char c;
            fp = fopen("t3.txt", "r");
            if (fp == NULL) printf("cant open");
            int line = 1;
            while(c != EOF){
                c = getc(fp);
                if (c == '\n')
                    line++;
            }
            rewind(fp);
            char  numid[5], info[50], l[50], h[50];
            for(int i=0;i<line;i++){
                fscanf(fp, "%s %s %s %s", numid, info, l, h);
                char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
                char* infor = (char*)malloc(sizeof(char) * strlen(info));
                char* low = (char*)malloc(sizeof(char) * strlen(l));
                char* high = (char*)malloc(sizeof(char) * strlen(h));
                strcpy(num_id, numid);
                strcpy(infor, info);
                strcpy(low, l);
                strcpy(high, h);
                insert(t3, low, high, infor);
            }
            // inorder_trav(*t1);
        }
        if (is_equal(prev_filename, "t4.txt")){
            FILE* fp;
            char* ch;
            char c;
            fp = fopen("t4.txt", "r");
            if (fp == NULL) printf("cant open");
            int line = 1;
            while(c != EOF){
                c = getc(fp);
                if (c == '\n')
                    line++;
            }
            rewind(fp);
            char  numid[5], info[50], l[50], h[50];
            for(int i=0;i<line;i++){
                fscanf(fp, "%s %s %s %s", numid, info, l, h);
                char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
                char* infor = (char*)malloc(sizeof(char) * strlen(info));
                char* low = (char*)malloc(sizeof(char) * strlen(l));
                char* high = (char*)malloc(sizeof(char) * strlen(h));
                strcpy(num_id, numid);
                strcpy(infor, info);
                strcpy(low, l);
                strcpy(high, h);
                insert(t4, low, high, infor);
            }
            // inorder_trav(*t1);
        }
        if (is_equal(prev_filename, "t1.txt") && is_equal(filename, "t2.txt")){
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
        }
        else if (is_equal(prev_filename, "t1.txt") && is_equal(filename, "t3.txt")){
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
        }
        else if (is_equal(prev_filename, "t1.txt") && is_equal(filename, "t4.txt")){
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
        }
        else if (is_equal(prev_filename, "t2.txt") && is_equal(filename, "t1.txt")){
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
        }
        else if (is_equal(prev_filename, "t2.txt") && is_equal(filename, "t3.txt")){
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
        }
        else if (is_equal(prev_filename, "t2.txt") && is_equal(filename, "t4.txt")){
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
        }
        else if (is_equal(prev_filename, "t3.txt") && is_equal(filename, "t1.txt")){
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
        }
        else if (is_equal(prev_filename, "t3.txt") && is_equal(filename, "t2.txt")){
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
        }
        else if (is_equal(prev_filename, "t3.txt") && is_equal(filename, "t4.txt")){
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
        }
        else if (is_equal(prev_filename, "t4.txt") && is_equal(filename, "t1.txt")){
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
            eval_overlap_timing(ol, low, high, infot, "t1s.txt");
        }
        else if (is_equal(prev_filename, "t4.txt") && is_equal(filename, "t2.txt")){
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
            eval_overlap_timing(ol, low, high, infot, "t2s.txt");
        }
        else if (is_equal(prev_filename, "t4.txt") && is_equal(filename, "t3.txt")){
            eval_overlap_timing(ol, low, high, infot, "t4s.txt");
            eval_overlap_timing(ol, low, high, infot, "t3s.txt");
        }
    }
    return;
}

void construct_intTree(Intv_tree* t1, Intv_tree* t2, Intv_tree* t3, Intv_tree* t4, char* filename){
    // printf("inside const func");
    // char* l, *h, *info;
    FILE* fp;
    char* ch;
    fp = fopen(filename, "r");
    if (fp == NULL) printf("cant open");
    char c;
    int line = 1;
    while(c != EOF){
        c = getc(fp);
        if (c == '\n')
            line++;
    }
    rewind(fp);
    char  numid[5], info[50], l[50], h[50];

    // if (filename == "t1.txt"){
    //     if (*t1 != NULL)
    //         destroy_tree(t1);
    //     }
    // else if (filename == "t2.txt"){
    //     if (*t2 != NULL)
    //         destroy_tree(t2);
    //     }
    // else if (filename == "t3.txt"){
    //     if (*t3 != NULL)
    //         destroy_tree(t3);
    //     }
    // else if (filename == "t4.txt"){
    //     if (*t4 != NULL)
    //         destroy_tree(t4);
    //     }

    for(int i=0;i<line;i++){
        fscanf(fp, "%s %s %s %s", numid, info, l, h);
        char* num_id = (char*)malloc(sizeof(char) * strlen(numid));
        char* infor = (char*)malloc(sizeof(char) * strlen(info));
        char* low = (char*)malloc(sizeof(char) * strlen(l));
        char* high = (char*)malloc(sizeof(char) * strlen(h));
        strcpy(num_id, numid);
        strcpy(infor, info);
        strcpy(low, l);
        strcpy(high, h);

        if (filename == "t1.txt")
            insert(t1, low, high, infor);
        else if (filename == "t2.txt")
            insert(t2, low, high, infor);
        else if (filename == "t3.txt")
            insert(t3, low, high, infor);
        else if (filename == "t4.txt")
            insert(t4, low, high, infor);

        if (str_to_int(num_id) == 1)
            do_opt_file(t1, t2, t3, t4, "t1.txt", low, high, infor, filename);
        else if (str_to_int(num_id) == 2)
            do_opt_file(t1, t2, t3, t4, "t2.txt", low, high, infor, filename);
        else if (str_to_int(num_id) == 3)
            do_opt_file(t1, t2, t3, t4, "t3.txt", low, high, infor, filename);
        else if (str_to_int(num_id) == 4)
            do_opt_file(t1, t2, t3, t4, "t4.txt", low, high, infor, filename);
    }
    fclose(fp);
    // destroy_tree(t1);
    // destroy_tree(t2);
    // destroy_tree(t3);
    // destroy_tree(t4);
    return;
}

void display_schedule(Intv_tree t, char* filename){
    FILE * fp;
    fp = fopen(filename, "a");
    if (fp == NULL) printf("cant open\n");
    else{
        char* spc = "\t\t\t\t\tThis is your schedule";
        fprintf(fp, "%s\n\n\n", spc);
    }
    fclose(fp);
    inorder(t, filename);
    return;
}

void inorder_trav(Intv_tree t){
    if (t == NULL) return;
    inorder_trav(t->left);
    printf("%s : From %s to %s\n", t->info, t->low, t->high);
    inorder_trav(t->right);
}

void destroy_tree(Intv_tree* t){
    if (*t == NULL)
        return;
    if ((*t)->left == NULL && (*t)->right == NULL){
        node* par = (*t)->parent;
        if (par->left == *t)
            par->left = NULL;
        else
            par->right = NULL;
        free(*t);
    }
    destroy_tree(&((*t)->left));
    destroy_tree(&((*t)->right));
}



// samruddhikamthe2121@gmail.com
// akhileshkaroshi06@gmail.com
// ktanvarada@gmail.com