#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

char a[10];
long long even=0,odd=0,cells=0;
long long size;
pthread_mutex_t mutex;
long long *matrix_1,*matrix_2;


struct data {
int rows;
int size;
int limit;
};

void *Multiply_Matrix(void * Data){

struct data* thread_data = Data;
long long rows = thread_data->rows;
long long size = thread_data->size;
long long limit = thread_data->limit;


int i = limit-rows;
for (; i<limit; i++){
        for (int j=0; j<size; j++){
                long long cell =0;
                for (int k=0; k<size; k++){
                        cell += matrix_1[(i*size)+k]*matrix_2[j+(size*k)];
                }
		pthread_mutex_lock(&mutex);
                if (cell%2)
                        odd++;
                else even++;
        	cells++;
		pthread_mutex_unlock(&mutex);
	}

}
}

int main (int argc , char * argv[]){

if (argc<2){
	printf("Enter number of threads!\n");
}

else {

int threads_num = atoi(argv[1]);
struct data threads_data[threads_num];

FILE *infile = fopen("in.txt","r");

fgets(a,sizeof(a),infile);
sscanf(a,"%lld",&size);

matrix_1 = malloc((size*size) * sizeof (long long));
matrix_2 = malloc((size*size) * sizeof (long long));


for (int i=0; i<size*size; i++){
        fgets(a,sizeof(a),infile);
	sscanf(a,"%lld",&matrix_1[i]);
}

for (int i=0; i<size*size; i++){
        fgets(a,sizeof(a),infile);
        sscanf(a,"%lld",&matrix_2[i]);
}

fclose(infile);


for (int i=0; i<threads_num; i++){
	threads_data[i].size = size;
	threads_data[i].rows = 0;
	threads_data[i].limit = 0;
}

int result = size/threads_num;
int remainder = size%threads_num;


for (int i=0; i<threads_num; i++){
	threads_data[i].rows = result;
	if (i<remainder)
		threads_data[i].rows++;
	threads_data[i].limit = threads_data[i].rows;
	if (i>0)
		threads_data[i].limit += threads_data[i-1].limit;
}


pthread_t Multiply_thread[threads_num];

FILE *outfile = fopen("out.txt","w");

for (int i=0; i<threads_num; i++){
	fprintf(outfile,"ThreadID=%d,",i);
	fprintf(outfile," startLoop=%d,",threads_data[i].limit-threads_data[i].rows);
	pthread_create(&Multiply_thread[i],NULL,Multiply_Matrix,&threads_data[i]);
	fprintf(outfile," endLoop=%d\n",threads_data[i].limit);
}

for (int i=0; i<threads_num; i++){
        pthread_join(Multiply_thread[i],NULL);
}


fprintf(outfile,"numOfEven=%lld,",even);
fprintf(outfile," numOfOdd=%lld,",odd);
fprintf(outfile," totalCells=%lld\n",cells);

fclose(outfile);
pthread_exit(NULL);
}
}




