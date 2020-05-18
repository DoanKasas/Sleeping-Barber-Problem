#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t waitingArea; // Bekleme Alani Semaforu
sem_t shaving; // Tras Semaforu
sem_t barberSleeping; // Berber Uyumsa Semaforu

int currentNumberCustomer = 0;

void *Barber();
void *Customer();

int main()
{
    int isBarberSleeping = 1;
    int i; //Ortak for d�ng�leri i�in.
    srand(time(NULL));
    int thisDayNumberCustomers = 5 + rand()%35; // G�n boyunca 5 ile 35 aras� m��teri gelmesi isteniyor.

    pthread_t customerThreads[thisDayNumberCustomers], barberThread; // Berber ve m��teri Thread'leri tan�mland�.
    //Semaforlar tan�mlan�yor.
    sem_init(&waitingArea,1,5); // Bekleme alan�n�n m��teri limiti 5 ayarland�.
    sem_init(&shaving,1,1); // Tra� i�in m��teri limiti 1 ayarland�.
    sem_init(&barberSleeping,1,0); // Berber uyuma semaforu konuldu.


    pthread_create(&barberThread,NULL,Barber,NULL);

    //Thread'ler olu�turuluyor
    for(i=0; i< thisDayNumberCustomers;i++)
        pthread_create(&customerThreads[i],NULL,Customer,NULL);

    //Join i�lemi ��kme olmas�n diye create'lerden ayr� yaz�l�yor.
    pthread_join(barberThread,NULL);

    for(i=0; i< thisDayNumberCustomers;i++)
        pthread_create(&customerThreads[i],NULL);


    return 0;
}

void *Customer(){
sleep(1);

printf("\n M��teri berber d�kkan�n�n �n�ne geldi.\n");

sem_wait(waitingArea);

currentNumberCustomer++;

printf("M��teri bekleme alan�na ge�ti.");

if(currentNumberCustomer == 1)
    sem_post(barberSleeping);
sem_wait(shaving);

printf("M��teri tra� olma alan�na ge�ti.");

}

void *Barber(){

if(currentNumberCustomer == 0)
    sem_wait(barberSleeping);

sem_post(shaving);
printf("M��teri tra� oldu.");

sem_post(waitingArea);

currentNumberCustomer--;

}
