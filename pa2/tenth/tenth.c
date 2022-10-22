#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void convert(unsigned int hex, int total_bits, int expBits, int fracBits, int precBits){
    char binaryR[255]="";
    char binaryNotTrun[255]="";
    char binary[255]="";
    if (hex==0){
        strcat(binaryR,"0");
    }
    while (hex!=0){
        if (hex%2==0){
            strcat(binaryR, "0");  
        }else if (hex%2==1){
            strcat(binaryR, "1");
        }
       hex = hex/2;
    }

    for (int i = strlen(binaryR); i<total_bits; i++){
        strcat(binaryR, "0");
    }

    for (int i = strlen(binaryR)-1; i>=0; i--){
        strncat(binaryNotTrun, &binaryR[i], 1);
    }

    for (int i = strlen(binaryNotTrun)-total_bits;i<strlen(binaryNotTrun);i++){
        strncat(binary, &binaryNotTrun[i], 1);
    }
    

    int special_val = 1;
    int denorm_val = 1;
    int normal = 0;
    for (int i = 1; i<=expBits; i++){
        if (binary[i]=='1'){
            denorm_val = 0;
            break;
        }
    }

    for (int i = 1; i<=expBits; i++){
        if (binary[i]=='0'){
            special_val = 0;
            break;
        }
    }

    if (special_val==0&&denorm_val==0){
        normal=1;
    }

    int e;
    int bias = (1<<(expBits-1))-1;
    int E=0;

    //have binary rep up to this point in total bits
    if (normal==1){
        for (int i = 1; i<=expBits; i++){
            if (binary[i]=='1'){
                E = E+(1<<(expBits-i));
            }
        }
        e = E-bias;
        char mantissa[255]="1.";
        for (int i = expBits+1; i<strlen(binary);i++){
            strncat(mantissa, &binary[i],1);
        }


        double mantissa_val = 1.0; 
        int index=-1;
        for (int i = 2; i<strlen(mantissa);i++){
            if (mantissa[i]=='1'){
                mantissa_val+=pow(2,index);
            }
            index--;
        }
        mantissa_val*=pow(2,e);
        if (binary[0]=='1') mantissa_val*=-1;
        printf("%.*f\n", precBits, mantissa_val);

    } else if (denorm_val==1){
        char mantissa[255]="0.";
        for (int i = expBits+1; i<strlen(binary);i++){
            strncat(mantissa, &binary[i],1);
        }
        double mantissa_val = 0;
        int index=-1;
        for (int i = 2; i<strlen(mantissa);i++){
            if (mantissa[i]=='1'){
                mantissa_val+=pow(2,index);
            }
            index--;
        }
        mantissa_val*=pow(2,1-bias);
        if (binary[0]=='1') mantissa_val*=-1;
        printf("%.*f\n", precBits, mantissa_val);


    }

}

int main(int argc,char *argv[]){

FILE *fp;
fp = fopen(argv[1], "r");
int totalBits;
int expBits;
int fracBits;
unsigned int hex;
int precBits;

while (fscanf(fp,"%d %d %d %x %d", &totalBits, &expBits, &fracBits, &hex, &precBits)!=EOF){
    convert(hex, totalBits, expBits, fracBits, precBits);

}

}