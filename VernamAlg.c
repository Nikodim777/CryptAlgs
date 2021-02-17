#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "VernamAlg.h"

#define NE 3
#define NS 256
#define EL 1000 //Символ конца строки
#define M 436921 //Модуль для генерации чисел

static int i,
    seed, //Начальное значение для генерации чисел
    simple[15] = {37,41,43,47,53,59,61,67,71,73,79,83,89,97,101}, //Массив простых чисел
    tmpi[NE], //Переменная для хранения шифруемого куска строки в виде массива чисел
    flc;
static char tmpc[NE + 1], //Переменная для хранения шифруемого куска строки
     key[NE]; //Сгенерированная гамма

void toIntVernam(char *masc,int *masi){ //Интерпретация строки как массива кодов
    for(i = 0;i < NE;i++){
        if(masc[i] == '\0'){
            flc = 0;
            masi[i] = EL;
        } else {
            masi[i] = masc[i] < 0 ? masc[i] + NS : masc[i];
        }
    }
}

void toStrVernam(int *masi,char *masc){ //Интерпретация массива кодов как строки
    for(i = 0;i < NE;i++){
        if(masi[i] == EL) masc[i] = '\0';
        else if(masi[i] < 0 && masi[i] >= NS){
            printf("Неизвестный код - %d\n",masi[i]);
            masc[i] = ' ';
        } else {
            masc[i] = masi[i];
        }
    }
}

void initVernam(){
    seed = simple[rand()%15]; //Выбор случайого простого начального значения
    for(i = 0;i < NE;i++){ //Заполнение гаммы генерируемыми по алгоритму BSS значениями
        seed = (seed * seed)%M;
        key[i] = (seed%NS + NS)%NS;
    }
    printf("Ключ - ");
    for(i = 0;i < NE;i++)
        printf("%d ",key[i]);
    printf("\n");
}

void Crypt(int *vect){ //Шифрование вектора
    for(i = 0;i < NE;i++)
        if(vect[i] != EL){
            vect[i] ^= key[i];
            vect[i] &= 0x000000FF; //Так как конечный результат 1-байтный символ обнуляем первые 3 байта
        }
}

void vernamCrypt(char *str,char *res,int fl)
{
    int n = 0;
    res[0] = '\0';
    flc = 1;

    printf("Исходная строка - %s\n",str);

    while(str[n] != '\0' && flc){//Пока не конец строки

        strncpy(tmpc,&str[n],NE); //Беру очередные три элемента строки
        toIntVernam(tmpc,tmpi);

        //Шифрование\Дешифрование с помощью ключа
        Crypt(tmpi);

        toStrVernam(tmpi,tmpc);
        strncpy(&res[n],tmpc,NE); //Добавление строки к результату
        n += NE; //Увеличение счётчика обработанных символов
    }
    res[n] = '\0';

    if(fl) printf("Расшифрованная строка - %s\n",res);
    else printf("Зашифрованная строка - %s\n",res);
}
