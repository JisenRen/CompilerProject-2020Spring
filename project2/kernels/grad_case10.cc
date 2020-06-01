#include "../run2.h"
void grad_case10(float (&dA)[8][8], float (&dB)[10][8]){
    for(int _0=0;_0<10;_0++){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                dB[_0][j] += ((_0==i?dA[i][j]:0)+(_0==i+1?dA[i][j]:0)+(_0==i+2?dA[i][j]:0))/3.0;
            }
        }
    }
}
