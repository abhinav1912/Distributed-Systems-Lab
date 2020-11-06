#include <stdio.h>
#define max(X, Y) (((X) >= (Y)) ? (X) : (Y))

// arrays to store events and dependencies
int process[2][20], dependency[20][20];

void main()
{
    // input number of events
    int num_events[2];
    int i,j;
    printf("Number of events for both processes: ");
    scanf("%d %d", &num_events[0], &num_events[1]);
    
    //assign a timestamp tp each event
    for (i=0;i<num_events[0];i++){
        process[0][i] = i+1;
    }
    for (i=0;i<num_events[1];i++){
        process[1][i] = i+1;
    }

    //input dependency matrix
    printf("For 1->2, input 1.\nFor 2->1, input -1.\nElse input 0.\n");
    for (i=0;i<num_events[0];i++){
        for (j=0; j<num_events[1];j++){
            printf("e1(%d) -> e2(%d): ",i+1,j+1);
            scanf("%d", &dependency[i][j]);
        }
    }

    for(i=0;i<num_events[0];i++){
        for(j=0;j<num_events[1];j++){

            //modify the timestamp if dependency exists
            if(dependency[i][j]==1){
                process[1][j]=max(process[1][j],process[0][i]+1);
                for(int k=j;k<num_events[1];k++)
                    process[1][k+1]=process[1][k]+1;
            }
            if(dependency[i][j]==-1){
                process[0][i]=max(process[0][i], process[1][j]+1);
                for(int k=i;k<num_events[0];k++)
                    process[0][k+1]=process[0][k]+1;
            }
        }
    }
    //output the event ordering
    printf("\nProcess 1: ");
    for(i=0;i<num_events[0];i++){
        printf("%d ",process[0][i]);
    }
    printf("\nProcess 2: ");
    for(j=0;j<num_events[1];j++)
    printf("%d ",process[1][j]);
    printf("\n");
    return;
}
