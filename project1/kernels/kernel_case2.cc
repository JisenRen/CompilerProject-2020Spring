#include "../run.h"
void kernel_case2(float (&A)[16][8]){
    for(int i=0;i<16;i++){
        for(int j=0;j<8;j++){
            A[i][j]=A[i][j]+2;
        }
    }
}
