/* 
 * File:   Neural_network.h
 * Author: JLD
 *
 * Created on Streda, 2014, apríl 16, 17:25
 */

#ifndef NEURAL_NETWORK_H
#define	NEURAL_NETWORK_H

class Neural_network {
    private:
            struct{
                   double w[102][102];  //vektory váh
                   double x[102];       //vektor vstupov
                   double y[102];       //vektor aktivácií
                   double delta[102];   //vektor chybových signálov
                   int right_answer;    //požadovaný výstup
                  }neuron;
                  
            int i,j;                    //pomocné premenné
            
            void input_layer(float *input); //vstupná vrstva
            void first_hidden_layer(void);  //prvá skrytá vrstva
            void second_hidden_layer(void); //druhá skrytá vrstva
            void output_layer(void);        //výstupná vrstva
            void run_network(float *input); //spustenie siete
            
    public:
           Neural_network(void);            //konštruktor
           void learning(float *input);      //režim učenia
           int life(float *input);          //režim života

};

#endif	/* NEURAL_NETWORK_H */

