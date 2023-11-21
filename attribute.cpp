#include "attribute.h"

void Attribute::Set_Name(const string& s){
  name = s;
}

string Attribute::Get_Name() const{
  return name;
}

void Attribute::Set_Numeric(bool b){
  is_numeric = b;
}

bool Attribute::Numeric() const{
  return is_numeric;
}

void Attribute::Add_Category(string s){
  categories.push_back(s);
}

int Attribute::Num_Categories() const{
  return categories.size();
}

// I don't usually like returning a vector from a function, but
// I don't want people having a reference to the real private data either
vector<string> Attribute::Get_Category_List() const{
  return categories;
}

bool Attribute::In_Domain(string new_val){
  if(new_val == "?")
    return true;
  if(is_numeric){
    return isdigit(new_val[0]);
  }
  else{
    for(int i = 0; i < categories.size(); i++)
      if(new_val == categories[i])
				return true;
    return false;
  }
}

string Attribute::Get_Category(int num) const{
  return categories[num];
}

