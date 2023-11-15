#ifndef CUBE_HH
#define CUBE_HH

#include <vector>
#include <string>
#include "Vector3D.hh"


class Cube{
    
    std::string _Name;
    std::vector<Vector3D> _Attributes;
    
  public:
    Cube( std::string Name, std::vector<Vector3D> Attributes  );
    std::vector<Vector3D> getAttributes();
    std::string getName();
    std::string to_string();
};

#endif