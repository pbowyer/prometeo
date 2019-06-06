#include "prmt_mat_blasfeo_wrapper.h"
#include "prmt_heap.h"
#include "dgemm.h"

void p_class_init(struct p_class *object){
    object->attr_1 = 1;
    object->attr_2 = 3.0;
    object->_Z8method_2prmt_matprmt_matprmt_mat = &_Z8method_2prmt_matprmt_matprmt_matp_class_impl;
}



void _Z8method_2prmt_matprmt_matprmt_matp_class_impl(p_class *self, struct prmt_mat * A, struct prmt_mat * B, struct prmt_mat * C) {
    ___c_prmt___fill(C, 0.0);
    ___c_prmt___dgemm(A, B, C, C);
    ___c_prmt___print(C);
    return ;
}


void function1(struct prmt_mat * A, struct prmt_mat * B, struct prmt_mat * C) {

    ___c_prmt___fill(C, 0.0);
    ___c_prmt___dgemm(A, B, C, C);
    ___c_prmt___print(C);
    return ;
}


void *___c_prmt_8_heap; 
void *___c_prmt_64_heap; 
void main() {
    ___c_prmt_8_heap = malloc(1000); 
    char *mem_ptr = (char *)___c_prmt_8_heap; 
    align_char_to(8, &mem_ptr);
    ___c_prmt_8_heap = mem_ptr;
    ___c_prmt_64_heap = malloc(100000); 
    mem_ptr = (char *)___c_prmt_64_heap; 
    align_char_to(64, &mem_ptr);
    ___c_prmt_64_heap = mem_ptr;

    int * n_list[10];
    n_list[0] = 1;
    struct p_class test_class___;
    struct p_class * test_class= &test_class___;
    p_class_init(test_class); // = p_class();
    test_class->attr_1 = 2;
    int j = 0;
    for(int i = 0; i < 10; i++) {
        j = j + 1;
    }

    while(j > 0) {
        j = j - 1;
    }

    int n = 10;
    struct prmt_mat * A = ___c_prmt___create_prmt_mat(n, n);
    ___c_prmt___prmt_mat_set_el(A, 0, 2, 2.0);
    for(int i = 0; i < 2; i++) {
        ___c_prmt___prmt_mat_set_el(A, 0, i, ___c_prmt___prmt_mat_get_el(A, 0, i));
    }

    ___c_prmt___fill(A, 1.0);
    struct prmt_mat * B = ___c_prmt___create_prmt_mat(n, n);
    ___c_prmt___fill(B, 2.0);
    struct prmt_mat * C = ___c_prmt___create_prmt_mat(n, n);
    test_class->_Z8method_2prmt_matprmt_matprmt_mat(test_class, A, B, C);
    struct prmt_mat * * pmat_list[10];
    pmat_list[0] = A;
    ___c_prmt___fill(C, 0.0);
    ___c_prmt___dgemm(A, B, C, C);
    ___c_prmt___print(C);
    ___c_prmt___copy(B, C);
    ___c_prmt___dgead(1.0, A, C);
    ___c_prmt___print(C);
    ___c_prmt___copy(A, C);
    ___c_prmt___dgead(-1.0, B, C);
    ___c_prmt___print(C);
    function1(A, B, C);
    function1(pmat_list[0], B, C);
}