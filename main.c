# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "int_tree.h"

// printf("inside overlap func");

int main(){
    Intv_tree t;
    init(&t);
    Intv_tree t1;
    init(&t1);
    Intv_tree t2;
    init(&t2);
    Intv_tree t3;
    init(&t3);
    Intv_tree t4;
    init(&t4);
    
    // insert(&t, "9:30", "10:30", "DSA-LECT");
    // insert(&t, "9:48", "11:00", "Dean-meet");
    // insert(&t, "11:15", "12:00", "Staff-gathering");
    // insert(&t, "13:00", "14:30", "Btech-viva");
    // insert(&t, "14:17", "15:00", "ESE-paper-seeting-SY");



    // construct_intTree(&t1, &t2, &t3, &t4, "t1.txt");
    // construct_intTree(&t1, &t2, &t3, &t4, "t2.txt");
    construct_intTree(&t1, &t2, &t3, &t4, "t3.txt");
    // construct_intTree(&t1, &t2, &t3, &t4, "t4.txt");
    display_schedule(t1, "t1s.txt");
    display_schedule(t2, "t2s.txt");
    display_schedule(t3, "t3s.txt");
    display_schedule(t4, "t4s.txt");

    return 0;
}