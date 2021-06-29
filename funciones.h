#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool wayToSort(pair<int, double> A, pair<int, double> B){
    return A.first < B.first;
}

vector<pair<int, double>> resolver(vector<vector<double>> m, vector<int> claves){
    double mij;
    vector<pair<int, double>> res (m.size());           //el divisor
    for(int i = 0; i < m.size()-1; ++i){                //Recorre cada fila
        for(int j = i+1; j < m.size(); ++j){          //j es que fila de abajo de i estamos (cada vez que baja esta uno mas a la derecha)
            mij = m[j][i]/m[i][i];                      //generamos el divisor
            for(int k = i; k < m[0].size(); ++k){       //iteramos cada elemento sobre la fila arrancando por i 
                m[j][k] -= mij*m[i][k];                //le restamos el divisor a cada elemento de la fila.
            }
        }
    }  
    for(int i = m.size()-1; i >= 0; --i){
        for(int j = m[0].size()-2; j >= i; --j){
            if(j == i){ //en el caso de que i=j debemos pasar dividiendo el coeficiente de xj
                double temp = m[i][m[0].size()-1];
                res[j] = make_pair(claves[j], temp/m[i][i]); // calculamos Xj y pasamos diviendo el coef de m[i][i] a b
            }
            else{       //en el caso de que i!=j pasamos el coeficiente.Xj (Xj lo tenemos en res junto a su equipo)
                m[i][m[0].size()-1] -= m[i][j]*res[j].second; 
                m[i][j] = 0;
            }
        }
    }
    return res;
}

vector<pair<int, double>> cmm(int equipos, int partidos, ifstream& iFile){
    vector<vector<double>> m (equipos, vector<double> (equipos+1, 0));      //inicializamos matriz AMPLIADA con 0s
    vector<int> claves;
    vector<double> victorias(equipos, 0);               // guardamos victorias y derrotas para guardar estos datos como enteros, igualmente los guardamos como doubles para utilizarlos en la division mas adelante
    vector<double> derrotas(equipos, 0);
    for(int i = 0; i < m.size(); ++i)
            m[i][i] = 2;
    for(int l = 0; l < partidos; ++l){                                      //iteramos cantidad de partidos veces
        int fecha, equipoI, puntajeI, equipoJ, puntajeJ;                          //iteramos cantidad de parametros por linea veces  0 = fecha | 1 = <equipo i | puntaje equipo i> | 2 = < equipo j | puntaje equipo j>
        iFile>>fecha>>equipoI>>puntajeI>>equipoJ>>puntajeJ;
        
        //definimos equipos en claves
        int i = -1, j = -1;
        for(int  k = 0; k < claves.size(); ++k){    // buscamos indices de equipos I y J
            if(claves[k] == equipoI)
                i = k;
            else if(claves[k] == equipoJ)
                j = k;
        }
        if(i == -1){                                // si equipo I no esta definido, lo define
            claves.push_back(equipoI);
            i = claves.size()-1;
        }
        if(j == -1){                                // si equipo J no esta definido, lo define
            claves.push_back(equipoJ);
            j = claves.size()-1;
        }

        //tenemos el i y el j de los equipos que acabamos de leer en esta linea
        //tmb tenemos las var puntajeJ y puntajeI
        //en la posicion i,j e j,i sumamos 1  tick
        //en la pos i,i e j,j sumamos 1 tick
        //en la posicion i,n e j,n sumamos 1/2 si gano o restar 1/2 si perdio 
        //bi= 1+(ganados-perdidos)/2 = 1 + sum(ganados/2) - sum(perdidos/2) = 1 + sum(1/2) - sum(1/2)
            
        m[i][j]--;
        m[j][i]--;
        m[i][i]++;
        m[j][j]++;

        
        if (puntajeI > puntajeJ) {
            victorias[i] += 1;
            derrotas[j] += 1;
        }else if (puntajeI < puntajeJ){
            victorias[j] += 1;
            derrotas[i] += 1;
        }
    }
    for(int i = 0; i < equipos; i++)
        m[i][m[0].size()-1] = (1 + (victorias[i] - derrotas[i]) / 2);

    return resolver(m, claves);
}



vector<pair<int, double>> wp(int equipos, int partidos, ifstream& iFile){   // cada linea : fecha | equipo i | puntaje equipo i | equipo j| puntaje equipo j|
    unordered_map<int, pair<int, int>> registro;                            //llevamos un registro en una tabla de hash<equpo,<victorias,partidos>>
    vector<int> claves;                                                     //claves para tabla
    for(int i = 0; i < partidos; ++i){                                      //iteramos cantidad de partidos veces
        int fecha, equipoI, puntajeI, equipoJ, puntajeJ;                          //iteramos cantidad de parametros por linea veces  0 = fecha | 1 = <equipo i | puntaje equipo i> | 2 = < equipo j | puntaje equipo j>
        iFile>>fecha>>equipoI>>puntajeI>>equipoJ>>puntajeJ;

        if(registro.find(equipoI) == registro.end()){                       //definimos  el equipo I en la tabla de hash si es que no estaba
            claves.push_back(equipoI);                                      //agregamos clave para tabla
            registro.insert({equipoI, make_pair(0, 0)});                    //inicializamos el equipo en tabla de hash con 0 partidos y victorias
        }
        if(registro.find(equipoJ) == registro.end()){                       //definimos el equipo J si no estaba como el anterior
            claves.push_back(equipoJ);
            registro.insert({equipoJ, make_pair(0, 0)});
        }
        
        registro[equipoI].first++;                                          //sumamos un partido jugado a ambos
        registro[equipoJ].first++;
        if(puntajeI > puntajeJ) {
            registro[equipoI].second++;                                     //sumamos 1 a la cantidad de partidos ganados de I
        }
        if(puntajeJ > puntajeI) {
            registro[equipoJ].second++;                                     //sumamos 1 a la cantidad de partidos ganados de J
        }
    }

    vector<pair<int, double>> res (equipos);                                //armamos la repsuesta
    for(int i = 0; i < equipos; ++i){                                       //recorremos las claves para tabla de hash
        double temp = registro[claves[i]].second;
        res[i] = make_pair(claves[i], temp/registro[claves[i]].first);      //cálculo de WP 
    }
    return res;
}

// SISTEMA ALTERNATIVO:
// Proponemos un sistema conocido (ya que es el que se usa en la liga de futbol argentina) que toma en cuenta el empate:
// A cada equipo se le suman distinta cantidad de puntos dependiendo si gano, perdio, O EMPATO.
// siendo => || victoria: 3 puntos || derrota: 0 puntos || empate: 1 punto ||

vector<pair<int, double>> massey(int equipos, int partidos, ifstream& iFile){
    vector<vector<double>> m (equipos, vector<double> (equipos+1, 0));      //inicializamos matriz AMPLIADA con 0s
    vector<int> claves;
    for(int l = 0; l < partidos; ++l){                                      //iteramos cantidad de partidos veces
        int fecha, equipoI, puntajeI, equipoJ, puntajeJ;                          //iteramos cantidad de parametros por linea veces  0 = fecha | 1 = <equipo i | puntaje equipo i> | 2 = < equipo j | puntaje equipo j>
        iFile>>fecha>>equipoI>>puntajeI>>equipoJ>>puntajeJ;
        
        //definimos equipos en claves
        int i = -1, j = -1;
        for(int  k = 0; k < claves.size(); ++k){    // buscamos indices de equipos I y J
            if(claves[k] == equipoI)
                i = k;
            else if(claves[k] == equipoJ)
                j = k;
        }
        if(i == -1){                                // si equipo I no esta definido, lo define
            claves.push_back(equipoI);
            i = claves.size()-1;
        }
        if(j == -1){                                // si equipo J no esta definido, lo define
            claves.push_back(equipoJ);
            j = claves.size()-1;
        }

        m[i][m[0].size()-1] += puntajeI - puntajeJ;
        m[j][m[0].size()-1] += puntajeJ - puntajeI;

        m[i][i] += 1;
        m[j][j] += 1;

        m[i][j] -= 1;
        m[j][i] -= 1;
    }
    for(int i = 0; i < m[0].size()-1; ++i){
        m[m.size()-1][i] = 1;
    }
    m[m.size()-1][m[0].size()-1] = 0;

    return resolver(m, claves);
}