#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  
#include <time.h>
#define Size 50

//coupons.txt contains discount coupons
//trips.txt contains available trip destinations
//saleshistory contains past earnings


//Void with parameters
void printString(char *route);  
//Void without parameters
void ShowTrips();       
void BookTrip();
//Non void with parameters
int validCoupon(char *coupon);  
void costTrip(int tripid,int *costToFind);
int calculateDiscount(int cost);
//Non void without parameters
int DisplayEarning();       
void DeleteCoupon();
void AddTrip();


int main(){

    //localtime() uses the time pointed by t ,to fill a tm structure with the values that represent the corresponding local time.
    time_t t ;
    struct tm *tmp ;
    char MY_TIME[Size];
    time( &t );
    tmp = localtime( &t );

    // using strftime to display time
    strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
    printf("\nAccessing the system on %s\n", MY_TIME );

    while(1){
        int choice;
        
        printf(" ____________________________________________\n");
        printf("|      Welcome to Sunshine Travel Agency     |\n");
        printf("|____________________________________________|\n");
        printf("|      0) Add a Trip                         |\n");
        printf("|      1) View Available Trips               |\n");
        printf("|      2) Book A Trip                        |\n");
        printf("|      3) Display Earning Table              |\n");
        printf("|      4) Delete A Coupon                    |\n");
        printf("|      5) Exit                               |\n");
        printf("|____________________________________________|\n");       
        printf("Enter a choice:  ");
        scanf("%d",&choice);

        if(choice==0){
            AddTrip();
        }
        else if(choice==1){
            ShowTrips();
        }
        else if(choice==2){
            BookTrip();
        }
        else if(choice==3){
            int total=DisplayEarning();
            printf("Total Earnings: RM %d\n\n",total);
        }
        else if(choice==4){
            DeleteCoupon();
        }
        else if(choice==5){
            break;
        }
        
        else{
            printf("Invalid Choice.");
        }
    } //simple menu in an infinite loop where user enters their choice and they input 5 the loop breaks and program exits
    return 0;
}

void AddTrip(){
    char tripname[30];
    int amount;
    FILE *tripfile=fopen("trips.txt","a");
    printf("Enter Trip Name: ");getchar();gets(tripname);
    printf("Enter trip amount: ");scanf("%d",&amount);
    fprintf(tripfile,"\n");
    fputs(tripname,tripfile);
    fprintf(tripfile,"\n%d",amount);
}

int DisplayEarning(){
    int amount,i;
    int total=0;
    int count=0;
    int id=0;
    int **table = (int**)malloc(100 * sizeof(int));     //declare a 2d array of trip number and its cost
    FILE *history;
    history=fopen("saleshistory.txt","r");
   
    while(!feof(history)){
        fscanf(history,"%d",&amount);
        count++;
    }   //count total number of trips
    fclose(history);
    
    for(i = 0; i < count ; i++){
        table[i] = (int*)malloc(2 * sizeof(int));
    }   //allocate memory to number of rows/trips
    history=fopen("saleshistory.txt","r");
    
     while(!feof(history)){
        fscanf(history,"%d",&amount);
        table[id][0]=id+1;
        table[id][1]=amount;
        total+=amount;
        id++;
    }   //fill 2d array with data from file

    printf("\nEarnings: \n");
    for(i=0;i<count;i++){
        printf("%d.  RM %d\n",table[i][0],table[i][1]);
    }   //display the table
    return total;
}

void DeleteCoupon(){
    char couponToDelete[7];
    char couponRead[7];
    FILE *coupon;
    FILE *temp;
    temp=fopen("temp.txt","w");
    coupon=fopen("coupons.txt","r");
    printf("Enter Coupon To Delete: ");getchar();scanf("%s",couponToDelete);        //take input of coupon to delete
    printf("The coupon has been deleted\n");
     while(fgets(couponRead,sizeof(couponRead),coupon)!=NULL){
        if(strcmp(couponToDelete,couponRead)!=0){
            fputs(couponRead,temp);
        }
    }       //create a temp file and put all coupons in it except the one to be deleted
    fclose(temp);
    fclose(coupon);
    temp=fopen("temp.txt","r");
    coupon=fopen("coupons.txt","w");
    while(fgets(couponRead,sizeof(couponRead),temp)!=NULL){
        fprintf(coupon,"%s",couponRead);
    }       //copy the contents of temp file back to original coupons file
    fclose(temp);
    fclose(coupon);
}

void ShowTrips(){
    FILE *tripfile;
    
    int cost,i;
    i=1;
    tripfile=fopen("trips.txt","r");
    printf("\n\n");
    while(1){
         char route[100];
        if(fgets(route,sizeof(route),tripfile)==NULL){
            break;
        }
        fscanf(tripfile,"%d\n",&cost);
        printf("Trip Id: %d\n",i);
        printString(route);
        printf("Cost: RM %d",cost);
        printf("\n\n");
        i++;
    }       //display the available trips with their cost
    fclose(tripfile);
}

int validCoupon(char *coupon){
    FILE *couponfile;
    char couponRead[7];
    couponfile=fopen("coupons.txt","r");
    while(fgets(couponRead,sizeof(couponRead),couponfile)!=NULL){
        if(strcmp(coupon,couponRead)==0){
            fclose(couponfile);
            return 1;
        }
    }   //open the coupon file in read and check if the coupon passed in parameter is valid
    fclose(couponfile);
    return 0;
}

void costTrip(int tripid,int *costToFind){
    FILE *tripfile;
    int i=1;
    int cost;
    *costToFind=-1;
    tripfile=fopen("trips.txt","r");
    printf("\n\n");
    while(1){
         char route[100];
        if(fgets(route,sizeof(route),tripfile)==NULL){
            break;
        }
        fscanf(tripfile,"%d\n",&cost);
        if(i==tripid){
            *costToFind=cost;
        }
        i++;
    } //read trip info file and update the value of cost pointer to the one which matches the trip id
    fclose(tripfile);
}

void BookTrip(){
    
    char name[20];
    char id[20];
    int tripid;
    int choice;
    int cost;
    FILE *history;
    history=fopen("saleshistory.txt","a");
    printf("\nEnter Name: ");getchar();gets(name);
    printf("Enter Your National Id: ");scanf("%s",id);
    printf("Enter Trip Id To Book: ");scanf("%d",&tripid);
    costTrip(tripid,&cost);   //pointer reference..passed through reference
    if(cost==-1){
        printf("Invalid Trip Id.\n\n");
    }
    else{
        printf("The trip is available.\n");
        printf("Cost: RM %d\n\n",cost);
        
        while(1){
        printf("Enter 1 if u have a 10 percent discount coupon otherwise enter anything else: ");scanf("%d",&choice);
        if(choice==1){          //If the user enters 1, he will be asked to enter the coupon. If the coupon matches, a 10 percent discount is given; otherwise, ask again
            char coupon[7];
            printf("Enter 6 digit coupon: ");getchar();gets(coupon);
            if(validCoupon(coupon)==1){     //if value 1 then coupon was valid
                int newamaount=calculateDiscount(cost);
                printf("\nYour Amount After 10 percent discount is RM %d\nThanks For Booking.\n\n",newamaount);
               
                fprintf(history,"\n%d",newamaount);
                break;
            }
            else{       //invalid coupon the user is asked to enter his choice whether or not he wants to enter a coupon
                printf("\nInvalid Coupon.\n\n");
            }
        }
        else{
            printf("\nYour Amount is RM %d\nThanks For Booking.\n\n",cost);
            fprintf(history,"\n%d",cost);   //amount without a coupon
            break;
        }
        }
    }
    fclose(history);
}


int calculateDiscount(int cost){
    int newamount;
    newamount=cost-((cost*10)/100);
    return newamount;
}   //calculate new amount after a 10 percent discount


void printString(char *route){
    int i=0;
    while(1){
        if(route[i]!='\0'){
            printf("%c",route[i]);
        }
        else{
            break;
        }
        i++;
    }
}       //just a function to print a string with spaces