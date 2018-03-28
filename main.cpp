#include <iostream>
#include <omp.h>
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
    cout<<min<<" "<<max;
}
int main() {
    Task4();
    return 0;
}