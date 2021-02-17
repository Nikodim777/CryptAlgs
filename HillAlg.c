#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "HillAlg.h"

#define NE 3 //Длина шифруемой за раз строки
#define NS 33 //Мощность алфавита

static int matr[NE][NE], //Матрица для шифрования
    rmatr[NE][NE], //Матрица для дешифрования
    tmpi[NE]; //Переменная для хранения шифруемого куска строки в виде массива чисел
static char tmpc[NE + 1]; //Переменная для хранения шифруемого куска строки

int Determinator(int *matr){ //Определяет детерминант матрицы
    return matr[0]*matr[4]*matr[8] + matr[1]*matr[5]*matr[6] + matr[2]*matr[3]*matr[7]
        - matr[2]*matr[4]*matr[6] - matr[1]*matr[3]*matr[8] - matr[0]*matr[5]*matr[7];
}

int *gcd(int m,int n){ //Расширенный алгоритм Евклида
    int q=1,r=0,s=0,t=1,*rez=(int*) malloc(sizeof(int)*3);
    while(1){
       if(m>n){m-=n;q-=s;r-=t;}
       else{n-=m;s-=q;t-=r;}
       if(n==0){
          rez[0]=m;rez[1]=q;rez[2]=r;
          return rez;
       }else if(m==0){
          rez[0]=n;rez[1]=s;rez[2]=t;
          return rez;
       }
    }
}

void initHill() //Инициализация модуля
{
    int i,j,*tmp,
        det = 0, //Детерминант
        rdet, //Элемент обратный детерминанту
        tmprmatr[3][3];
    while(det <= 0 || det%3 == 0 || det%11 == 0){ //Пока матрица не удовлетфорит условиям
        for(i = 0;i < NE;i++)
            for(j = 0;j < NE;j++)
                matr[i][j] = rand()%NS;
        det = Determinator(&matr[0][0]);
    }
    printf("Сгенерированная матрица:\n");
    for(i = 0;i < NE;i++){
        for(j = 0;j < NE;j++)
            printf("%d ",matr[i][j]);
        printf("\n");
    }
    printf("Детерминант - %d\n",det);

    //Нахождение обратного детерминанту элемента
    tmp = gcd(det,NS);
    rdet = (tmp[1]%NS + NS)%NS;
    printf("Обратный детерминанту элемент - %d\n",rdet);

    //Нахождение матрицы дополнений
    tmprmatr[0][0] = matr[1][1]*matr[2][2] - matr[1][2]*matr[2][1];
    tmprmatr[0][1] = -(matr[1][0]*matr[2][2] - matr[1][2]*matr[2][0]);
    tmprmatr[0][2] = matr[1][0]*matr[2][1] - matr[1][1]*matr[2][0];
    tmprmatr[1][0] = -(matr[0][1]*matr[2][2] - matr[0][2]*matr[2][1]);
    tmprmatr[1][1] = matr[0][0]*matr[2][2] - matr[0][2]*matr[2][0];
    tmprmatr[1][2] = -(matr[0][0]*matr[2][1] - matr[0][1]*matr[2][0]);
    tmprmatr[2][0] = matr[0][1]*matr[1][2] - matr[0][2]*matr[1][1];
    tmprmatr[2][1] = -(matr[0][0]*matr[1][2] - matr[0][2]*matr[1][0]);
    tmprmatr[2][2] = matr[0][0]*matr[1][1] - matr[0][1]*matr[1][0];

    //Нахождение обратной матрицы
    for(i = 0;i < NE;i++){
        for(j = 0;j < NE;j++){
            rmatr[i][j] = ((tmprmatr[j][i]%NS) * rdet)%NS;
            if(rmatr[i][j] < 0) rmatr[i][j] += NS;
        }
    }

    printf("Обратная матрица:\n");
    for(i = 0;i < NE;i++){
        for(j = 0;j < NE;j++)
            printf("%d ",rmatr[i][j]);
        printf("\n");
    }
}

void mulMatrix(int *vect,int *matr){ //Перемножение вектора на матрицу
    int i,j,tmp[NE];
    for(i = 0;i < NE;i++){
        tmp[i] = 0;
        for(j = 0;j < NE;j++)
            tmp[i] += vect[j] * matr[j*NE + i];
    }
    for(i = 0;i < NE;i++)
        vect[i] = tmp[i];
}

void toIntHill(char *masc,int *masi){ //Интерпретация строки как массива кодов
    int i;
    for(i = 0;i < NE;i++){
        if(masc[i] >= 'а' && masc[i] < 'ж') masi[i] = masc[i] - 'а';
        else if(masc[i] == 'ё') masi[i] = 'е' - 'а' + 1;
        else if(masc[i] >= 'ж' && masc[i] <= 'я') masi[i] = masc[i] - 'а' + 1;
        else {
            printf("Неизвестный символ - %d\n",masc[i] < 0 ? masc[i] + 256 : masc[i]);
            masi[i] = 0;
        }
    }
}

void toStrHill(int *masi,char *masc){ //Интерпретация массива кодов как строки
    int i;
    for(i = 0;i < NE;i++){
        if(masi[i] < 6) masc[i] = masi[i] + 'а';
        else if(masi[i] == 6) masc[i] = 'е';
        else if(masi[i] >= 7 && masi[i] < NS) masc[i] = masi[i] + 'а' - 1;
        else {
            printf("Неизвестный код - %d\n",masi[i]);
            masi[i] = 'а';
        }
    }
}

void hillCrypt(char *str,char *res,int fl)
{
    int i,n = 0;
    res[0] = '\0';

    printf("Исходная строка - %s\n",str);

    while(str[n] != '\0'){//Пока не конец строки

        strncpy(tmpc,&str[n],NE); //Беру очередные три элемента строки
        toIntHill(tmpc,tmpi);

        //Шифрование\Дешифрование с помощью матриц
        if(fl) mulMatrix(tmpi,&rmatr[0][0]);
        else mulMatrix(tmpi,&matr[0][0]);
        for(i = 0;i < NE;i++)
            tmpi[i] %= NS;

        toStrHill(tmpi,tmpc);
        strncpy(&res[n],tmpc,NE); //Добавление строки к результату
        n += NE; //Увеличение счётчика обработанных символов
    }
    res[n] = '\0';

    if(fl) printf("Расшифрованная строка - %s\n",res);
    else printf("Зашифрованная строка - %s\n",res);
}
