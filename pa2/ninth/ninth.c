#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void canonicalize(double left, double frac, long frac_bits, int *e_val, char** binary_form){
    char leftBinaryR[255]="";
    char leftBinary[255]="";
    char fracBinary[255]="";

    while(floor(left)!=0){
        if (fmod(left,2)==0){
            strcat(leftBinaryR, "0");  
        } else if (fmod(left,2)==1){
            strcat(leftBinaryR, "1");
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

    char mantissa[255]="1";
    for (int i = strlen(leftBinaryR)-1; i>=0; i--){
        strncat(leftBinary, &leftBinaryR[i], 1);
    }

    int t = strlen(leftBinary);
    leftBinary[0]='.';
    strcat(mantissa, leftBinary);
    //mant either 1. or 1.left

//how to difefrentiate when 
    if (t==0){ //decimal<0, mant is 1.
        while(fracBinary[*e_val]!='1'){
            *e_val=*e_val+1;
        } //.01
        int copy_i = *e_val+1;
        *e_val=(*e_val+1)/-1;

        for (int i = copy_i; i<strlen(fracBinary);i++){
            strncat(mantissa, &fracBinary[i],1);
        } 
    } else { //mant is 1.something already 
        *e_val = strlen(leftBinary)-1;
        for (int i = 0; i<strlen(fracBinary);i++){
            strncat(mantissa, &fracBinary[i],1);
        } 
    }
    
    strcpy(*binary_form, mantissa);
    memset(leftBinary, 0, 255);
}

void rounding(char** binary_form, int frac_bits, int* e_val){
    //mantissa at least will be 1., it is not truncated for bits
    char binary_copy[255]="";
    strcpy(binary_copy,*binary_form);

    if (strlen(binary_copy)-2<frac_bits){
        while(strlen(binary_copy)-2<frac_bits){
            strcat(binary_copy, "0");
        }
    }

    //binary copy frac is at least frac bit length, could be more
    char val1[255]="";
    char val2[255]="";

    int index = 0;
    while(index<strlen(binary_copy)&&index<frac_bits+2){
        strncat(val1, &binary_copy[index],1);
        index++;
    }
    

    //val1 is truncated to exactly fit frac bits 
    //for val2, find index of last 1
    strcpy(val2, val1);
    for (int i = strlen(val2)-1; i>=0; i--){
        if (val2[i]=='0'){
            val2[i] ='1';
            break;
        } else if (val2[i]=='1'){
            val2[i]='0';
        }
    }

    int val2UP = 0;
    //check if val 2 is extra power for e
    if (val2[0]=='0'){
        val2[0]='1';
        val2UP=1;
    }

   

    char midpoint[255]="";
    strcpy(midpoint,val1);
    strcat(midpoint,"1");
    char * eptr;
    double original = strtod(binary_copy, &eptr);
    double mp = strtod(midpoint, &eptr);

    if (original>mp){
        strcpy(binary_copy, val2);
        if (val2UP==1){
            *e_val=*e_val+1;
        }
    } else if (original<mp){
        strcpy(binary_copy, val1);
    } else {
        if(val1[strlen(val1)-1]=='0'){
            strcpy(binary_copy, val1);
        } else if(val2[strlen(val2)-1]=='0'){
            strcpy(binary_copy, val2);
            if (val2UP==1){
            *e_val=*e_val+1;
        }

        }
    }
    
    strcpy(*binary_form,"");
    for(int i =2; i<strlen(binary_copy);i++){
        strncat(*binary_form, &binary_copy[i], 1);
    }
    
}

void calcEField(int e_val, int e_bits){
    char E_FieldR[255]="";
    double bias = pow(2,e_bits-1)-1;
    double E_val = e_val+bias;
    while(floor(E_val)!=0){
        if (fmod(E_val,2)==0){
            strcat(E_FieldR, "0");  
        } else if (fmod(E_val,2)==1){
            strcat(E_FieldR, "1");
        }
        E_val=floor(E_val/2);
    } 

    char E_field[255]="";
    for (int i =strlen(E_FieldR)-1; i>=0;i--){
        strncat(E_field,&E_FieldR[i],1);
    }
    
    for (int i = strlen(E_field); i<e_bits;i++){
        printf("0");
    }
    printf("%s", E_field);


}

int main(int argc, char *argv[]){
    FILE * fp;
    fp = fopen(argv[1], "r");
    double dec_frac;
    int total_bits;
    int e_bits;
    int frac_bits;

    

    while (fscanf(fp,"%lf %d %d %d",&dec_frac, &total_bits, &e_bits, &frac_bits)!=EOF){
        double fractPart, intPart;
        int e_val=0;
        char *binary_form;char sign_bit='0';
        if (dec_frac==0){
            printf("0");
            for(int i =0; i<frac_bits+e_bits; i++){
                printf("0");
            }
            printf("\n");
        } else {
            if (dec_frac<0){
                dec_frac=dec_frac/-1;
                sign_bit='1';
            }

        fractPart = modf(dec_frac, &intPart);
        canonicalize(intPart, fractPart, frac_bits, &e_val, &binary_form);
        rounding(&binary_form, frac_bits, &e_val);
        printf("%c", sign_bit);
        calcEField(e_val, e_bits);
        printf("%s\n",binary_form);
        //must acc for special values
        }
    }
    fclose(fp);
    

    return 0; 

}



    
