/* 
 * File:   Neural_network.cpp
 * Author: JLD
 * 
 * Created on Streda, 2014, apríl 16, 17:25
 */

#include "Neural_network.h"
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define af(x) (1.0/(1.0+exp((-1.0)*x)))                                     //makro pre aktivačnú funkciu                            
#define daf(x) (exp((-1.0)*x)/((1.0+exp((-1.0)*x))*(1.0+exp((-1.0)*x))))    //makro pre prvú deriváciu aktivačnej funkcie



Neural_network::Neural_network()        //konštruktor- počiatočná inicializácia váh
{
    srand(time(NULL));
    
    for(i=0;i<102;i++)
    {
        for(j=0;j<102;j++)
        {
         neuron.w[i][j]=((rand() % 10)-5) /5.0; //interval náhodných hodnôt<-1;1>
        }
    }
}

void Neural_network::input_layer(float* input) //funkcia pre prechádzanie vstupnej vrstvy
{
    for(i=1;i<7;i++)
    {
        neuron.x[i]=input[i-1];
        neuron.y[i]=af(neuron.x[i]);
    }
}

void Neural_network::first_hidden_layer()     //funkcia pre prechádzanie prvej skrytej vrstvy
{
    neuron.y[0]=-1;
    double sum=0;
    
    for(i=7;i<27;i++)
    {
        for(j=1;j<7;j++)
        {
         sum+=neuron.y[j]*neuron.w[i][j];
        }
        neuron.x[i]=sum+neuron.y[0]*neuron.w[i][0];
        neuron.y[i]=af(neuron.x[i]);
        sum=0;
    }
}

void Neural_network::second_hidden_layer()      //funkcia pre prechádzanie druhej skrytej vrstvy
{
    double sum=0;
    
    for(i=27;i<42;i++)
    {
        for(j=1;j<21;j++)
        {
         sum+=neuron.y[j+6]*neuron.w[i][j];
        }
        neuron.x[i]=sum+neuron.y[0]*neuron.w[i][0];
        neuron.y[i]=af(neuron.x[i]);
        sum=0;
    }
}

void Neural_network::output_layer()         //funkcia pre prechádzanie výstupnej vrstvy vrstvy
{
    double sum=0;

    for(i=42;i<102;i++)
    {
        for(j=1;j<16;j++)
        {
         sum+=neuron.y[j+26]*neuron.w[i][j];
        }
        neuron.x[i]=sum+neuron.y[0]*neuron.w[i][0];
        neuron.y[i]=af(neuron.x[i]);
        sum=0;
    }
}

void Neural_network::run_network(float* input)  //spustenie siete
{
    input_layer(input);
    first_hidden_layer();
    second_hidden_layer();
    output_layer();
    
 
}

void Neural_network::learning(float* input)      //režim učenia
{   
    double sum=0;                               //inicializácia pomocnej premennej
    double gamma=0.2;                           //inicializácia parametra učenia
    float answer=input[6]*40;                   //získanie požadovaného výstupu
    neuron.right_answer=int(answer);
    run_network(input);                         //spustenie siete
    
    for(i=42;i<102;i++)                         //výpočet chybového signálu pre výstupnú vrstvu
    {   
        if(i!=62+neuron.right_answer)
        {
            neuron.delta[i]=-neuron.y[i];
            neuron.delta[i]*=daf(neuron.x[i]);
        }
        
        else
        {       
            neuron.delta[i]=1-neuron.y[i];
            neuron.delta[i]*=daf(neuron.x[i]);
            answer=i-62;
        }
    }
    
    for(i=27;i<42;i++)                              //výpočet chybového signálu pre druhú skrytú vrstvu
    {
        for(j=1;j<61;j++)
        {
         sum+=neuron.w[j+41][i-26]*neuron.delta[j+41];
        }
        neuron.delta[i]=sum*daf(neuron.x[i]);
        sum=0;
    }
    
    for(i=7;i<27;i++)                               //výpočet chybového signálu pre prvú skrytú vrstvu
    {
        for(j=1;j<16;j++)
        {
            sum+=neuron.w[j+26][i-6]*neuron.delta[j+26];
        }
        neuron.delta[i]=sum*daf(neuron.x[i]);
        sum=0;
    }
    
    for(i=1;i<16;i++)                               //zmena váh výstupnej vrstvy
    {
        for(j=42;j<102;j++)
        {
          neuron.w[j][i]+=neuron.delta[j]*gamma*neuron.y[i+26];
        }
    }
    
    for(i=1;i<21;i++)                               //zmena váh druhej skrytej vrstvy
    {
        for(j=27;j<42;j++)
        {
         neuron.w[j][i]+=neuron.delta[j]*gamma*neuron.y[i+6];
        }
    }
    
    for(i=1;i<7;i++)                                //zmena váh prvej skrytej vrstvy
    {
        for(j=7;j<27;j++)
        {
         neuron.w[j][i]+=neuron.delta[j]*gamma*neuron.y[i];
        }
    }
    
    for(i=7;i<102;i++)                             //zmena váh pre prah
    {
        neuron.w[i][0]+=neuron.delta[i]*gamma*neuron.y[0];
    }
}

int Neural_network::life(float* input)          //režim života
{
    run_network(input);                         //spustenie siete
    double max=0;                               //inicializácia pomocnej premennej
    int answer;
    
    for(i=42;i<102;i++)                         //algoritmus pre nájdenie neurónu s najvyššou aktiváciou
    {
       if(neuron.y[i]>max)
       {
           max=neuron.y[i];
           answer=i;
       }
    }
    
    return answer-62;                           //prevod na stupne celzia
}


