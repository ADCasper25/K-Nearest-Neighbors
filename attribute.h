#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Attribute{
 public:
  void Set_Name(const string& s);
  string Get_Name() const;
  void Set_Numeric(bool b);
  bool Numeric() const;
  void Add_Category(string s);
  int Num_Categories() const;
  bool In_Domain(string s);
  vector<string> Get_Category_List() const;
  string Get_Category(int num) const;

 private:
  string name;
  bool is_numeric;
  vector<string> categories;
};
#endif
