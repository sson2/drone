/* Solution to comp20005 Assignment 1, 2019 semester 1.
   Authorship Declaration:
   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.
   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.
   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.
   Signed by: [Sophie(Subin) Son 656483]
Dated:     [6-05-19]
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 999
#define DRONEWEIGHT 3.8
#define PERCENTAGE 100
#define STARTINGCHARGE 100.0
#define MAXTRAVELDISTANCE 6300.0
#define DRONESPEED 4.2

/* Different */
typedef struct {
    double x_axis;
    double y_axis;
    double weight;
    double distance;
    double batteryout;
    double batteryret;
}Packages;

/* Function prototype */
void stage_one(Packages package[MAX], int num_package);
void stage_two(Packages package[MAX], int num_package);
void stage_three(Packages package[MAX], int num_package);
void stage_four(Packages package[MAX], int num_package);
double distance_cal(Packages package1, Packages package2);
double battery_out(Packages package1, Packages package2);
double battery_ret(Packages package1, Packages package2);
void int_swap(int *p1, int *p2);

/* The main funtion to execute the entire program */
int main(int argc, char *argv[]){
    Packages package[MAX];
    char ch;
    int num_package = 0;
    /* Control of Reading input dataset*/
    /* Skip the header*/
    while (scanf("%c", &ch) != EOF && ch != '\n');
    /* Scan the dataset*/
    while (scanf("%lf%lf%lf", &package[num_package].x_axis,
                &package[num_package].y_axis, &package[num_package].weight)!=EOF){
        num_package++;
    }
    /* Stage 1 to 4*/
    stage_one(package, num_package); 
    stage_two(package, num_package);
    stage_three(package, num_package);
    stage_four(package, num_package);
    return 0;
}

/* Stage 1- Control of printing */
void stage_one(Packages package[MAX], int num_package){
    int i = 0;
    double weight_item = 0, total_weight = 0;
    /* Calculate the total weight of packages*/
    for(i=0;i< num_package; i++) {
        weight_item = package[i].weight;
        total_weight += weight_item;
    }
    printf("S1, total data lines: %d \n", num_package);
    printf("S1, first data line :  x=%5.1f, y=%5.1f, kg=%2.2f \n",
            package[0].x_axis, package[0].y_axis, package[0].weight);
    printf("S1, final data line :  x=%5.1f, y=%5.1f, kg=%2.2f \n",
            package[num_package -1].x_axis, package[num_package -1].y_axis,
            package[num_package -1].weight);
    printf("S1, total to deliver: %.2f kg\n\n", total_weight);

}

/* Stage 2 - Sequential Processing*/
void stage_two(Packages package[MAX], int num_package){
    int i = 0, total_batteries =1;
    double current_charge=STARTINGCHARGE, total_distance=0;
    Packages origin;
    origin.x_axis = 0;
    origin.y_axis = 0;
    /* Calculate the distance, battery out and battery return*/
    for(i =0; i< num_package; i++){
        package[i].distance = distance_cal(origin, package[i]);
        package[i].batteryout = battery_out(origin, package[i]);
        package[i].batteryret = battery_ret(origin, package[i]);
        /* Change the battery if it is needed */
        if(current_charge < (package[i].batteryout + package[i].batteryret)){
            printf("S2, change the battery\n");
            total_batteries += 1;
            current_charge = STARTINGCHARGE;
        }
        printf("S2, package= %2d, distance= %5.1fm, battery out=%4.1f%%, "
                "battery ret=%4.1f%%\n",i, package[i].distance,
                package[i].batteryout, package[i].batteryret);

        current_charge-=(package[i].batteryout + package[i].batteryret);
        /* Calculate the total distance*/
        total_distance += (2.0)*distance_cal(origin, package[i]);
    }
    printf("S2, total batteries required:%2.d\n",total_batteries);
    printf("S2, total flight distance=%6.1f meters, total flight time=%4.0f seconds\n\n",total_distance,total_distance/DRONESPEED);
}

/* Stage 3 - Non-Sequential Processing*/
void stage_three(Packages package[MAX], int num_package){
    int i = 0, total_batteries =1;
    double current_charge=STARTINGCHARGE, total_distance=0;
    int deliver_package = 0;
    int delivered[num_package];
    for(i =0; i< num_package; i++)
        delivered[i] = 0;
    Packages origin;
    origin.x_axis = 0;
    origin.y_axis = 0;
    while ( deliver_package < num_package){
        for(i =0; i< num_package; i++){
            package[i].distance = distance_cal(origin, package[i]);
            package[i].batteryout = battery_out(origin, package[i]);
            package[i].batteryret = battery_ret(origin, package[i]);
            if(delivered[i] == 0 && current_charge > (package[i].batteryout + package[i].batteryret )){
                printf("S3, package= %2d, distance= %5.1lfm, battery out=%4.1lf%%, "
                        "battery ret=%4.1lf%%\n",i, package[i].distance,
                        package[i].batteryout, package[i].batteryret);
                current_charge-=(package[i].batteryout + package[i].batteryret);
                delivered[i] = 1;
                deliver_package++;
            }
        }
        if ( num_package > deliver_package){
            printf("S3, change the battery\n");
            total_batteries += 1;
            current_charge = STARTINGCHARGE;
        }
    }

    total_distance += (2.0)*distance_cal(origin, package[i]);

    printf("S3, total batteries required:%4.d\n",total_batteries);
    printf("S3, total flight distance=%.1lf meters, total flight time=%4.0lf seconds\n\n",total_distance,total_distance/DRONESPEED);

}

/* Stage 4- A Further Generalization */
void stage_four(Packages package[MAX], int num_package){
    int i = 0, total_batteries =1;
    double current_charge=STARTINGCHARGE, total_distance=0;
    int deliver_package = 0; 
    int delivered[num_package];
    //initializing all package are not delivered 
    for(i =0; i< num_package; i++)
        delivered[i] = 0;
    
    Packages centroid;
    double sum_x_axis = 0, sum_y_axis = 0;
    /* Calculate the centroid location*/
    for (i=0; i< num_package; i++){
        sum_x_axis += package[i].x_axis;
        sum_y_axis += package[i].y_axis;
    }

    centroid.x_axis = sum_x_axis/num_package;
    centroid.y_axis = sum_y_axis/num_package;
    
    printf("S4, centroid location x= %5.1lfm, y= %5.1lfm\n", centroid.x_axis, centroid.y_axis);
    while ( deliver_package < num_package){
        for(i =0; i< num_package; i++){
            package[i].distance = distance_cal(centroid, package[i]);
            package[i].batteryout = battery_out(centroid, package[i]);
            package[i].batteryret = battery_ret(centroid, package[i]);
            if(delivered[i] == 0 && current_charge > (package[i].batteryout + package[i].batteryret)){
                printf("S4, package= %2d, distance= %5.1fm, battery out=%4.1f%%, "
                        "battery ret=%4.1f%%\n",i, package[i].distance,
                        package[i].batteryout, package[i].batteryret);
                current_charge-=(package[i].batteryout + package[i].batteryret);
                total_distance += (2.0)*distance_cal(centroid, package[i]);
                delivered[i] = 1;
                deliver_package++;
            }
        }
        if (deliver_package < num_package){
            printf("S4, change the battery\n");
            total_batteries += 1;
            current_charge = STARTINGCHARGE;
        }
    }
    printf("S4, total batteries required:%4.d\n",total_batteries);
    printf("S4, total flight distance=%6.1f meters, total flight time=%4.0f seconds\n\n",total_distance,total_distance/DRONESPEED);

}
/* Calculate the distance between two coordinates*/
double distance_cal(Packages package1, Packages package2){
    double distance = 0;
    distance = sqrt(pow(package2.x_axis - package1.x_axis, 2)+
            pow(package2.y_axis - package1.y_axis,2));
    return distance;
}
/* Calculate the battery usuage while heading to the delivery destination*/
double battery_out(Packages package1, Packages package2){
    double distance = distance_cal(package1, package2);
    double batteryout = 0, drone_range_out =0;
    drone_range_out = MAXTRAVELDISTANCE/(DRONEWEIGHT+package2.weight);
    batteryout = distance/drone_range_out *PERCENTAGE;
    return batteryout;
}
/*Calculate the battery usuage while returning*/
double battery_ret(Packages package1, Packages package2){
    double distance = distance_cal(package1, package2); 
    double batteryret = 0, drone_range_ret = 0;
    drone_range_ret = MAXTRAVELDISTANCE/DRONEWEIGHT;
    batteryret = distance/drone_range_ret *PERCENTAGE;
    return batteryret;
}

void 
int_swap(int *p1, int *p2){
    int tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

// "programming is fun"
