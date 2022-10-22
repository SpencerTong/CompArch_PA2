#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>


void twos_comp(long num, long bits){
    char* rep = malloc(sizeof(char)*(bits+1));
    rep[bits] = '\0';
    if (num<=-(pow(2,bits-1))){
            rep[0] = '1';
        for (int i =1; i<bits; i++){
            rep[i]='0';
        }
    } else if (num>=(pow(2,bits-1))){
        rep[0] = '0';
        for (int i =1; i<bits; i++){
            rep[i]='1';
        }
    } else { //get binary then flip
        int temp = num;
        int largest_power = bits-1;
        if (num<0){num/=-1;}
        while(num!=0){
            while(pow(2,largest_power)>num){
                rep[bits-largest_power-1]='0';
                largest_power--;
            }
            num=num-pow(2,largest_power);
            rep[bits-largest_power-1] = '1';
            largest_power--;
        }

        if (largest_power+1!=0){ //check if need to fill trailing zero's
            while(bits-largest_power-1!=bits){
                rep[bits-largest_power-1] = '0';
                largest_power--;
            }
        }

        if (temp<0){
        for (int i =0; i<bits; i++){ //flip
            if (rep[i]=='1') rep[i]='0';
            else if (rep[i]=='0') rep[i]='1';
        }

        for (int i=bits-1; i>=0;i--){ //add 1
            if(rep[i]=='1'){
                rep[i] ='0';
            } else if (rep[i]=='0'){
                rep[i] = '1';
                break;
            }
        }
        }
    }
    printf("%s\n", rep); 

    free(rep);

}


int main (int argc, char *argv[]){

    FILE * fp;
    fp = fopen(argv[1], "r");
    char num[255];
    char bits[255];
    char *ptr;
    while(fscanf(fp,"%s %s",num,bits)!=EOF){
        long n = strtol(num,&ptr,10);
        long b = strtol(bits,&ptr,10);
        twos_comp(n,b);
    }
    
    fclose(fp);
    

    

    return 0; 
}
