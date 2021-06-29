#include "funciones.h"

// vector<double> cmm(int equipos, int partidos, ifstream& iFile);
// vector<double> wp(int equipos, int partidos, ifstream& iFile);

int main(int argc, char **argv) {

    if(argc != 4)                                           //si se pasa una cantidad erronea de parametros
        cout<<"Parametros invalidos. >:c\n";
    else {                                                  //si la cantidad es correcta
        char *path = argv[1];                               //mapeamos los parametros que entran
        string salida(argv[2]);
        int metodo = atoi(argv[3]);

        ifstream iFile;
        iFile.open(path);                                       //abrimos el archivo input
        if(iFile.is_open()){                                    //vemos si el archivo pudo abrirse
            int equipos;
            int partidos;

            iFile >> equipos >> partidos;
            vector<pair<int, double>> res;

            switch (metodo){                                    //seleccionamos el metodo adecuado o hay error
                case 0:
                    res = cmm(equipos, partidos, iFile);
                    break;
                case 1:
                    res = wp(equipos, partidos, iFile);
                    break;
                case 2:
                    res = massey(equipos, partidos, iFile);
                    break;
                default:
                    printf("Metodo invalido.\n");
                    break;
            }

            sort(res.begin(), res.end(), wayToSort);

            iFile.close();                                      //cerramos el archivo input

            ofstream oFile;                                     //creamos el archivo para guardar los datos
            oFile.open(salida);
            for (int i = 0; i < equipos; ++i)                //ponemos los datos
                oFile/* << res[i].first << ", "*/ << res[i].second << "\n";
            oFile.close();                                      //cerramos el archivo output
        }
        else
            cout << "Archivo no encontrado :T\n";               //archivo no encontrado :T
    }
    return 0;
}