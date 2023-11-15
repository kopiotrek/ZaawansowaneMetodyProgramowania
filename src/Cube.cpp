#include "Cube.hh"
#include <string>  
#include <iostream> 
#include <sstream>  

using namespace std;

Cube::Cube( string Name, vector<Vector3D> Attributes ){
    _Name = Name;
    _Attributes = Attributes;
}

vector<Vector3D> Cube::getAttributes(){
    return _Attributes;
}

string Cube::getName(){
    return _Name;
}

string Cube::to_string(){
    stringstream buff;
    buff << _Name <<" Shift=" << _Attributes[0] << 
                    " Scale=" << _Attributes[1] <<
                    " RotXYZ_deg=" << _Attributes[2] <<
                    " Trans_m=" << _Attributes[3] <<
                    " RGB=" << _Attributes[4] << endl;
    return buff.str();

}