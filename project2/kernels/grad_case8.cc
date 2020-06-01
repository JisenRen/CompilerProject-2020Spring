#include "../run2.h"
void grad_case8(float (&dB)[32], float (&dA)[2][16]){
    for(int _0=0;_0<2;_0++){
        for(int _1=0;_1<16;_1++){
            for(int i=0;i<32;i++){
                dA[_0][_1] += (_0==i/ 16&&_1==i%16?dB[i]:0);
            }
        }
    }
}
