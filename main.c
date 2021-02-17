#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "HillAlg.h"
#include "GammaAlg.h"
#include "VernamAlg.h"

#define CONTINUE '1' //Символ продолжения работы
#define ENCRYPT  '1' //Символ шифрования

int main()
{
    srand(time(NULL)); //Настройка генератора случайных чисел
    SetConsoleOutputCP(1251); //Установка кодировочной страницы для поддержки кириллицы

    FILE *fp, //Указатель на шифруемый файл
        *nfp; //Указатель на зашифрованный файл
    char ch, //Хранит введённые пользователем управляющие программой символы
        type, //Хранит идентификатор типа алгоритма
        fname[1000],
        tmp[1000],
        res[1000]; //Получает за/рас шифрованную строку
    void (*cryptAlg)(char *str,char *res,int fl); //Переменная хранящая функцию для шифрования

    printf("Ввыберите алгоритм который хотите использовать\n");
    printf("1 - алгоритм Хилла\n");
    printf("2 - шифр по модулю N\n");
    printf("3 - шифр Вернама\n");
    printf("другое - алгоритм Эль-Гамаля\n");
    scanf("%s",&type);
    if(type == '1'){
        initHill();
        cryptAlg = hillCrypt;
    } else if(type == '2'){
        cryptAlg = gammaCrypt;
    } else if(type == '3'){
        initVernam();
        cryptAlg = vernamCrypt;
    } else{
        initHill();
        cryptAlg = hillCrypt;
    }

    ch =  CONTINUE;
    while(ch == CONTINUE){
        printf("Введите 1 если хотите зашифровать и другое если хотите расшифровать.\n");
        scanf("%s",&ch);
        if(ch == ENCRYPT) printf("Введите имя файла который хотите зашифровать.\n");
        else printf("Введите имя файла который хотите расшифровать.\n");
        scanf("%s",fname);
        fp = fopen(fname,"r");
        if(fp == NULL){
            printf("Ошибка открытия файла!\n");
            ch = CONTINUE;
            continue;
        }
        nfp = fopen("tmp.txt","w");
        if(nfp == NULL){
            printf("Ошибка открытия временного файла!\n");
            ch = CONTINUE;
            continue;
        }

        if(ch == ENCRYPT) //Если нужно зашифровать
            while(fgets(tmp,1000,fp) != NULL){//Шифрует файл построчно
                cryptAlg(tmp,res,0);
                fputs(res,nfp);
            }
        else
            while(fgets(tmp,1000,fp) != NULL){//Дешифрует файл построчно
                cryptAlg(tmp,res,1);
                fputs(res,nfp);
            }

        fclose(nfp);
        fclose(fp);

        remove(fname);
        rename("tmp.txt",fname); //Переименование временного файла в исходный файл

        printf("Введите 1 для продолжения и другое для выхода.\n");
        scanf("%s",&ch);
    }
    return 0;
}
