#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void canon_binary(double left, double frac, long bits){
    char leftBinary[255]="";
    char fracBinary[255]=".";

    while(floor(left)!=0){
        if (fmod(left,2)==0){
            strcat(leftBinary, "0");  
        } else if (fmod(left,2)==1){
            strcat(leftBinary, "1");
        }
        left=floor(left/2);
    }

    while(frac!=0){
        frac*=2;
        if (frac>=1){
            strcat(fracBinary,"1");
            frac--;
        } else {
            strcat(fracBinary,"0");
        }
    }

    char mantissa[255]="";

    for (int i =strlen(leftBinary)-1; i>=0; i--){
         strncat(mantissa, &leftBinary[i], 1);
    }

    strcat(mantissa,fracBinary); //combine the two
    int e_val = 0;

    if (mantissa[0]=='.'){
        e_val--;
        for (int i = 1; i<strlen(mantissa);i++){
            if (mantissa[i]=='1'){ break; }
            e_val--;
        }
    } else {
        for (int i = 1; i<strlen(mantissa);i++){
        if (mantissa[i]=='.'){
            break;
        }
        e_val++;
        }
    }

    char final_M[255]="1.";
    if (mantissa[0]=='.'){
        for (int i = (e_val/-1)+1;i<=bits+(e_val/-1);i++){
            if (i<strlen(mantissa)){
                strncat(final_M,&mantissa[i],1);
            } else {
                strcat(final_M,"0");
            }
        } 
    } else {
        for(int i = 1; i<=bits; i++){
        if(i<strlen(mantissa)){
            if (mantissa[i]!='.'){
                strncat(final_M,&mantissa[i],1);
            } else {
                bits++;
            }
        } else {
            strcat(final_M,"0");
        }
    }
    }


    printf("%s %d\n", final_M, e_val);
    memset(leftBinary, 0, 255);

}


int main (int argc, char *argv[]){

    FILE * fp;
    fp = fopen(argv[1], "r");
    double num, fractPart, intPart;
    long bitNum;


    while(fscanf(fp,"%lf %ld",&num, &bitNum)!=EOF){
        fractPart = modf(num, &intPart);
        canon_binary(intPart, fractPart, bitNum);
    }
    
    fclose(fp);
    

    

    return 0; 
}
