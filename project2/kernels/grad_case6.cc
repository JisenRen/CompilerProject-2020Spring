#include "../run2.h"
void grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]){
    for(int _2=0;_2<7;_2++){
        for(int _3=0;_3<7;_3++){
            for(int c=0;c<16;c++){
                for(int k=0;k<8;k++){
                    for(int n=0;n<2;n++){
                        for(int p=0;p<5;p++){
                            for(int q=0;q<5;q++){
                                for(int r=0;r<3;r++){
                                    for(int s=0;s<3;s++){
                                        dB[n][c][_2][_3] += (_2==p+r&&_3==q+s?dA[n][k][p][q]:0)*C[k][c][r][s];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
