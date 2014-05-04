/* 
 * File:   main.cpp
 * Author: JLD
 *
 * Created on Streda, 2014, apríl 16, 17:24
 */

#include<cstdlib>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<stdio.h>
#include<iostream>
#include<math.h>
#include"Neural_network.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    ifstream file_read;                 //premenná pre čítanie zo súboru
    ofstream eror;                      //premenná pre zápis do súboru
    string buffer;                      
    string data[69799];
    char charakter='a';                 //počiatičná inicializácia pomocnej premennej
    char hours[69799][2];
    char temperature[69799][5];
    float ftemperature[69799];
    float original[17478];
    float training[17472][7];
    float chyba=0;
    int i=0,j=0,p,h=0;
    file_read.open("serious_data.txt");     //otovorenie súboru pre čítanie
    eror.open("eror_output.txt");           //otvorenie súboru pre zápis
    
    while(!file_read.eof())                 //načítanie vstupného súboru
    {
     getline(file_read,buffer);
     data[i]=buffer;
     i++;
    }
    
    for(i=0;i<69799;i++)                    //získanie vektora hodín a teplôt zo súboru
    { 
      for(p=0;p<3;p++)
      {
       h=0;
       charakter=data[i][j];
       while(charakter!=' ')
       {
        charakter=data[i][j];
        
        if(p==0 && h<2)
        {
         hours[i][h]=charakter;
        }
        
        else if(p==2 && h<4)
        {
            temperature[i][h]=charakter;
        }
        h++;
        j++;
       }
      }
      j=0;
    }
    file_read.close();                              //zatvorenie súboru
    
    for(i=0;i<69792;i++)                            //konverzia char/float
    {
     sscanf(temperature[i],"%f",&ftemperature[i]);
    }
    
    j=0;
    for(i=0;i<69791;i++)                            //separácia unikátnych teplôt
    {
        if(hours[i][1]!=hours[i+1][1])
        {
            original[j]=ftemperature[i];
            j++;
        }
        
        if(i==69790)
        {
         original[j]=ftemperature[i+1];
        }
    }
    
    p=0;
    for(i=0;i<17472;i++)                        //vytváranie vektora vstupov pre neurónovú sieť
    {   
        for(j=0;j<7;j++)                        
        {
         training[i][j]=original[j+p]/40;
        }
        p++;
    }
    
    Neural_network neuronik;                    //vytvorenie siete
    float *training_data;                       //premenná pre vstup do siete
    
    training_data=(float*)malloc(7*sizeof(float));  //alokácia pamäte pre vstup
    
    for(p=0;p<5000;p++)                             //počet iterácii
    {
        for(i=0;i<17472;i++)
        {
            for(j=0;j<7;j++)
            {
             training_data[j]=training[i][j];       //plnenie vstupu údajmi z trénovacej množiny
            }
            neuronik.learning(training_data);
            h=neuronik.life(training_data);
            chyba+=fabs(int(training[i][6]*40)-h);  //výpočet priemernej chyby
        }
        chyba=chyba/17472;
        eror<<chyba<<endl;                          //zápis chyby do súboru
        chyba=0;
        if(p%100==0)
        {
         cout<<p<<endl;
        }
    }
    eror.close();                                   //zatvorenie súboru
    
    string test_data[5666];                         //pomocné premenné                 
    char t_hours[5666][2];
    char t_temperature[5666][5];
    float t_ftemperature[5663];
    float t_original[1417];
    float test[1411][7];
    ofstream file_write;
    i=0,j=0;
    
    file_read.open("test_2014.txt");                //otvorenie súboru s testovacou množinou
    file_write.open("neuron_output.txt");           //otvorenie súboru pre zápis výstupov siete
    
    while(!file_read.eof())                         //načítanie trénovacej množiny
    {
     getline(file_read,buffer);
     test_data[i]=buffer;
     i++;
    }
    
    for(i=0;i<5666;i++)                             //získanie vektora hodín a teplôt
    { 
      for(p=0;p<3;p++)
      {
       h=0;
       charakter=data[i][j];
       while(charakter!=' ')
       {
        charakter=test_data[i][j];
        
        if(p==0 && h<2)
        {
         t_hours[i][h]=charakter;
        }
        
        else if(p==2 && h<4)
        {
            t_temperature[i][h]=charakter;
        }
        h++;
        j++;
       }
      }
      j=0;
    }
    file_read.close();
    
    for(i=0;i<5663;i++)                             //konverzia char/float
    {
     sscanf(t_temperature[i],"%f",&t_ftemperature[i]);
    }
    
    j=0;
    for(i=0;i<5666;i++)                             //separácia unikátnych hodnôt
    {
        if(hours[i][1]!=hours[i+1][1])
        {
            t_original[j]=t_ftemperature[i];
            j++;
        }
        
        if(i==5665)
        {
         t_original[j]=t_ftemperature[i+1];
        }
    }
    
    chyba=0;
    p=0;
    for(i=0;i<1411;i++)                             //vytváranie vstupov pre neurónovú sieť
    {   
        for(j=0;j<7;j++)
        {
         test[i][j]=t_original[j+p]/40;
        }
        p++;
    }
    
    for(i=0;i<1410;i++)                         //odoslanie vstupov do siete
    {
        for(j=0;j<7;j++)
        {
         training_data[j]=test[i][j];
        }
        
        h=neuronik.life(training_data);
        file_write<<test[i][6]*40<<","<<h<<endl;    //zápis výstupu a očakávaného vsupu do súboru
        chyba+=fabs(int(test[i][6]*40)-h);      
    }
    
    chyba=chyba/1410;                               //výpočet priemernej chyby
    cout<<chyba;                                    //výpis priemernej chyby
    file_write.close();
    
    getchar();
    return 0;
}


