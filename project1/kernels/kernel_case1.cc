#include "../run.h"
void kernel_case1(float (&A)[32][16]){
    for(int i=0;i<32;i++){
        for(int j=0;j<16;j++){
            A[i][j]=2;
        }
    }
}
