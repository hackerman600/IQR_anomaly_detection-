#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define corpus_size 8

int** createIntArray(){
        FILE *filepointer;
        char catchr[100];
        int pre_out[8][4];
        int** out = (int**)malloc(8 * sizeof(int*) * 4);
        int row_itter = -1;
        int col_itter = 0;

        filepointer = fopen("/home/kali/Desktop/machine_learning/anomaly_detection/easy.csv","r");
        if (filepointer == NULL){
            perror("Error opening file");
            
        }

        while(fgets(catchr, sizeof(catchr), filepointer)){
            char *tok = strtok(catchr,",");
            //printf("%s,%s", tok,"\n");
            while (tok != NULL){
                if (strlen(tok) > 0){
                    //printf("tok = %s,%d,%d,%s", tok, row_itter, col_itter,"\n");
                } 

                if (row_itter >= 0){
                    pre_out[row_itter][col_itter] = atoi(tok);
                    col_itter += 1;
                }
                
                tok = strtok(NULL,",");
                
            }
            row_itter += 1;
            col_itter = 0;
        } 

        //check if the data as imported correctly
        printf("\n------------ dataset ------------\n");
        printf("\ngender,weight,height,loves pizza\n\n");
        for (int r = 0; r < 8; r++){
            out[r] = (int*)malloc(sizeof(int) * 4);
            for (int c = 0; c < 4; c++){
                out[r][c] = pre_out[r][c];
                printf("%d,%s",out[r][c],"     ");
            }
            printf("%s","\n");
        }

        printf("\n------------------------------\n");

        return out;

}


struct KeyValuePair {
    char type[50];
    char key[50];
    double value;
};


struct pair {
    double median;
    int index;
};


int compare_size(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

struct pair calculate_median(int arr[],int size){
    int copyarr[8];
    int num_ind_to_erase = size/2-1;
    double catchr;
    struct pair structure;
        
    for (int val = 0; val < size; val++){
        copyarr[val] = arr[val];
    }

    if (size % 2 == 0){
        structure.index = corpus_size / 2;
        int newcopy[2];
        catchr = 0.0;
        for (int c = num_ind_to_erase; c < (size - num_ind_to_erase); c++){
            catchr += copyarr[c];
        }

        structure.median = catchr / 2;

    } else {
        structure.median = copyarr[(size - 1) / 2];
        structure.index = (corpus_size - 1) / 2;
    }

    return structure;

}


struct IQR_output{
    int lower_25[corpus_size/4];
    int middle_50[corpus_size/2];
    int greater_25[corpus_size/4];   
};

struct IQR_output calculate_IQR(int index,int ascending_corpus[8]){
    struct IQR_output structure;
    int indx = 0;
    for (int i = 0; i < 8; i++){
        if (i < (index / 2)){
            structure.lower_25[indx] = ascending_corpus[i];
        }

        else if (i >= (index / 2) && i < (index / 2 * 3)){
            if (i == index / 2){
                indx = 0;
            }
            structure.middle_50[indx] = ascending_corpus[i];
        }

        else if (i >= (index / 2 * 3)){
            if (i == (index / 2 * 3)){
                indx = 0;
            }
            structure.greater_25[indx] = ascending_corpus[i];
        }

        indx += 1;
    }
    // 0 1 | 2 3 | 4 5 | 6 7
    return structure;    

};


int main(){
    int** dataset = createIntArray();
    char columns[50][50] = {"gender", "weight", "height", "loves pizza"};//hehhee, hehehhe

    //make a copy of the dataset. use for colwise MEDIAN and IQR.
    int ascending_colwise_dataset[4][8];
    int corpus[8];
    int corpus_w[8];
    for(int c = 1; c < 2; c++){
        for(int r = 0; r < 8; r++){
            corpus[r] = dataset[r][c];
            corpus_w[r] = dataset[r][c+1];
        } 
        
        qsort(corpus,sizeof(corpus)/sizeof(corpus[0]),sizeof(int),compare_size);
        qsort(corpus_w,sizeof(corpus_w)/sizeof(corpus_w[0]),sizeof(int),compare_size);
          
        for(int r = 0; r < 8; r++){
            //printf("corpus, %d,%s",corpus[r],"\n");
        } 

        printf("\n-------------------------------------- stats --------------------------------------\n\n");
        
        struct pair colwise_median = calculate_median(corpus,sizeof(corpus)/sizeof(corpus[0]));
        struct pair colwise_median_height = calculate_median(corpus_w,sizeof(corpus_w)/sizeof(corpus_w[0]));
        printf("\nmedian weight:  %f,%s,%f",colwise_median.median,"                              median height:  ",colwise_median_height);
        struct IQR_output colwise_iqr_values = calculate_IQR(colwise_median.index,corpus);
        struct IQR_output colwise_iqr_values_height = calculate_IQR(colwise_median.index,corpus_w);
        

        //display feature QR information and state potential anomalies.
        printf("\n\nlower 25% values for weights:                           lower 25% values for heights\n\n");
        for (int a = 0; a < 2; a++){
            printf("%d,%s,%d,%s", colwise_iqr_values.lower_25[a],"                                                      ",colwise_iqr_values_height.lower_25[a],"\n");
        }

        printf("\n\nmiddle 50% values for weights:                           middle 50% values for heights\n\n");
        for (int a = 0; a < 4; a++){
            printf("%d,%s,%d,%s", colwise_iqr_values.middle_50[a],"                                                      ", colwise_iqr_values_height.middle_50[a],"\n");
        }

        printf("\n\ngreater 25% values for weights:                           greater 25% values for heights\n\n");
        for (int a = 0; a < 2; a++){
            printf("%d,%s,%d,%s", colwise_iqr_values.greater_25[a],"                                                      ", colwise_iqr_values_height.greater_25[a],"\n");
        }
        printf("\n\n------------------------------------ anomalies ------------------------------------\n");
        printf("\nanomalies detected for weights:                           anomalies detected for heights:\n\n");
        printf("%d,%s,%d",colwise_iqr_values.greater_25[1],"                                                      ",colwise_iqr_values_height.greater_25[1]);
    } 
    
    
    return 0;
}


















/*int main() {
    FILE *filePointer;
    char line_buffer[10000]; // Adjust the buffer size as needed
    char delimiter = ','; // CSV delimiter
    double *storevals[1000000] = {}; //can not store 47.04 mill in 1. 1 mill max on stack
    int itter = 0; 

    filePointer = fopen("/home/kali/Desktop/machine_learning/neural_networks/from_scratch/cpp_networks/x_train.csv", "r");
    if (filePointer == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(line_buffer, sizeof(line_buffer), filePointer) != NULL) {
        // Tokenize the line based on the delimiter
        char *token = strtok(line_buffer, ",");
        while (token != NULL) {
            // Process each token and store it in storevals
            if (strlen(token) > 1) {
                double val = strtod(token,NULL); 
                //printf("val = %f",val);
                storevals[itter] = val;
                itter += 1;
            }

            if (itter == 999999){
                printf("999999 = %f\n", storevals[3000]);
                printf("%d\n", sizeof(storevals));
                return 0;
            }

            // Get the next token
            token = strtok(NULL, ",");


        }
    }

    fclose(filePointer);

    // Print the first value from storevals
    printf("%.2lf\n", sizeof(storevals));
  

    return 0;
}*/



//import dataset into 2d array from non main function.
//rearrange data in ascending order. rowwise sum (normalize features row wise.)

