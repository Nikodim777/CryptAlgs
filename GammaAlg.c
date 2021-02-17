#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "GammaAlg.h"

#define NE 4
#define NS 33
#define EL 1000 //Символ конца строки

static char key[] = "обед";
static int  i,
     flc = 1, //Флаг продолжения обработки строки
     tmpi[NE]; //Переменная для хранения шифруемого куска строки в виде массива чисел
static char tmpc[NE + 1]; //Переменная для хранения шифруемого куска строки

void toIntGamma(char *masc,int *masi){ //Интерпретация строки как массива кодов
    for(i = 0;i < NE;i++){
        if(masc[i] >= 'а' && masc[i] < 'ж') masi[i] = masc[i] - 'а';
        else if(masc[i] == 'ё') masi[i] = 'е' - 'а' + 1;
        else if(masc[i] >= 'ж' && masc[i] <= 'я') masi[i] = masc[i] - 'а' + 1;
        else if(masc[i] == '\0'){
                flc = 0;
                masi[i] = EL;
        } else {
            printf("Неизвестный символ - %d\n",masc[i] < 0 ? masc[i] + 256 : masc[i]);
            masi[i] = 0;
        }
    }
}

void toStrGamma(int *masi,char *masc){ //Интерпретация массива кодов как строки
    for(i = 0;i < NE;i++){
        if(masi[i] < 6) masc[i] = masi[i] + 'а';
        else if(masi[i] == 6) masc[i] = 'е';
        else if(masi[i] >= 7 && masi[i] < NS) masc[i] = masi[i] + 'а' - 1;
        else if(masi[i] == EL) masc[i] = '\0';
        else {
            printf("Неизвестный код - %d\n",masi[i]);
            masi[i] = 'а';
        }
    }
}

void addKey(int *vect){ //Шифрование вектора
    int tmpi2[NE];
    toIntGamma(key,tmpi2);
    for(i = 0;i < NE;i++)
        if(vect[i] != EL)
            vect[i] = (vect[i] + tmpi2[i] + 1)%NS;
}

void subKey(int *vect){ //Дешифрование вектора
    int tmpi2[NE];
    toIntGamma(key,tmpi2);
    for(i = 0;i < NE;i++)
        if(vect[i] != EL)
            vect[i] = (vect[i] - tmpi2[i] - 1 + NS)%NS;
}

void gammaCrypt(char *str,char *res,int fl)
{
    int n = 0;
    res[0] = '\0';
    flc = 1;

    printf("Исходная строка - %s\n",str);
    printf("Ключ - %s\n",key);

    while(str[n] != '\0' && flc){//Пока не конец строки

        strncpy(tmpc,&str[n],NE); //Беру очередные три элемента строки
        toIntGamma(tmpc,tmpi);

        //Шифрование\Дешифрование с помощью матриц
        if(fl) subKey(tmpi);
        else addKey(tmpi);

        toStrGamma(tmpi,tmpc);
        strncpy(&res[n],tmpc,NE); //Добавление строки к результату
        n += NE; //Увеличение счётчика обработанных символов
    }
    res[n] = '\0';

    if(fl) printf("Расшифрованная строка - %s\n",res);
    else printf("Зашифрованная строка - %s\n",res);
}
