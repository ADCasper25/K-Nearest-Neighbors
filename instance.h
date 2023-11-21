#ifndef INSTANCE_H
#define INSTANCE_H

#include "attribute.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Instance{
 public:
  void Set_Attributes(const vector<Attribute>& c);
  bool Add_Value(int attribute_num, string val);
  string Get_Nominal_Value(int attribute_num) const;
  double Get_Numeric_Value(int attribute_num) const;
  bool Is_Numeric_Attribute(int attribute_num) const;
  string Get_Attribute_Name(int attribute_num);
  Attribute Get_Attribute(int attribute_num) const;
  int Get_Num_Attributes() const;
 protected:
  bool Is_Valid_Attribute(int attribute_num) const;
 private:
  
  vector<Attribute> Attribute_List;
  vector<string> Element_List;
};

#endif
