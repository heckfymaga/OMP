#include <iostream>
#include <omp.h>
#include <random>
using namespace std;
void Task1(){
    int n;
    #pragma omp parallel num_threads(8) private(n)
    {
        n = omp_get_thread_num();
        printf("Hello world printed by thread %d out of %d\n", n, omp_get_num_threads());
    }
}
void Task2(){
    int n = 3;
    omp_set_num_threads(n);
    #pragma omp parallel if(n > 2)
    {
        if (omp_in_parallel()) {
            printf("thread %d out of %d\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }
    n = 1;
    omp_set_num_threads(n);
    #pragma omp parallel if(n > 2)
    {
        if (omp_in_parallel()) {
            printf("thread %d out of %d\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }
}
void Task3(){
    int a=1, b=2;
    printf("Before parallel a = %d b = %d\n", a, b);
    #pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        a+=omp_get_thread_num();
        b+=omp_get_thread_num();
        printf("In parallel a = %d b = %d thread = %d\n",a, b, omp_get_thread_num());
    }
    printf("After parallel a = %d b = %d\n\n", a, b);

    printf("Before parallel a = %d b = %d\n", a, b);
    #pragma omp parallel num_threads(4) shared(a) private(b)
    {
        a+=omp_get_thread_num();
        b+=omp_get_thread_num();
        printf("In parallel a = %d b = %d thread = %d\n",a, b, omp_get_thread_num());
    }
    printf("After parallel a = %d b = %d\n", a, b);
}
void Task4(){
    const size_t size = 10;
    int a[] = {1,2,3,4,5,6,7,5,3,1};
    int b[] = {4,5,4,3,2,1,4,6,9,0};
    int min = a[0];
    int max = b[0];
    #pragma omp parallel num_threads(2)
    {
        #pragma parallel for
        for(int i=0;i<size;i++){
            #pragma omp master
            {
                if(min > a[i]) min=a[i];
            }
            if(omp_get_thread_num() != 0){
                if(max < b[i]) max = b[i];
            }
        }
    }
    printf("min a[] = %d max b[] = %d",min,max);
}
void Task5(){
    int d[6][8];
    for(int i=0;i<6;i++){
        for(int j=0;j<8;j++){
            d[i][j]=rand()%10;
        }
    }
    int sum = 0;
    int min = d[0][0];
    int max = d[0][0];
    int counter = 0;
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                #pragma omp parallel for num_threads(6) reduction(+: sum)
                for(int i=0;i<6;i++){
                    #pragma omp parallel for
                    for(int j=0;j<8;j++){
                        sum+=d[i][j];
                    }
                }

            }
            #pragma omp section
            {
                #pragma omp parallel for num_threads(6)
                for(int i=0;i<6;i++){
                    #pragma omp parallel for
                    for(int j=0;j<8;j++){
                        if(min > d[i][j]){
                            #pragma omp critical
                            min = d[i][j];
                        }
                        if(max < d[i][j]){
                            #pragma omp critical
                            max = d[i][j];
                        }
                    }
                }
            }
            #pragma omp section
            {
                #pragma omp parallel for num_threads(6) reduction(+: counter)
                for(int i=0;i<6;i++){
                    #pragma omp parallel for
                    for(int j=0;j<8;j++){
                        if(d[i][j]%3 == 0)
                            counter++;
                    }
                }
            }
        }
    }
    printf("average = %d\n", sum/48);
    printf("min = %d max = %d\n", min,max);
    printf("quantity of numbers which are divided by 3  = %d\n", counter);
    for(auto &line:d){
        for(auto item:line){
            cout<<item<<' ';
        }
        cout<<endl;
    }
}
int main() {
}