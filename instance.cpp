#include "instance.h"

void Instance::Set_Attributes(const vector<Attribute>& c){
  Attribute_List.resize(c.size());
  Element_List.resize(c.size());
  for(int i = 0; i < c.size(); i++)
    Attribute_List[i] = c[i];
}

bool Instance::Add_Value(int attribute_num, string val){
  if(Is_Valid_Attribute(attribute_num)){
    if(Attribute_List[attribute_num].In_Domain(val)){
      Element_List[attribute_num] = val;
      return true;
    }
    else{
      cout << "Error: tried to add value " << val << " not in domain of "
	   << Attribute_List[attribute_num].Get_Name() << endl;
      return false;
    }
  }   
  else{
    cout << "ERROR: Trying to add a value to attribute # " << attribute_num << " , which doesn't exist" << endl;
    exit(1);
	}
}

string Instance::Get_Nominal_Value(int attribute_num) const{
  if(Is_Valid_Attribute(attribute_num)){
    if(!Is_Numeric_Attribute(attribute_num))
      return Element_List[attribute_num];
    else{// legal attribute, wrong type
      cout << "Error- this is a numeric attribute!" << endl;
      return Element_List[attribute_num];
    }
  }
  else{
    cout << "Error= illegal attribute number : " << attribute_num << endl;
    exit(1);
  }
}

double Instance::Get_Numeric_Value(int attribute_num) const{
  if(Is_Valid_Attribute(attribute_num)){
    if(Is_Numeric_Attribute(attribute_num))
      return stod(Element_List[attribute_num].data());
    else{// legal attribute, wrong type
      cout << "Error- this is a nominal attribute!" << endl;
      return 0;
    }
  }
  else{
    cout << "Error- illegal attribute number : " << attribute_num << endl;
    exit(1);
  }
}

bool Instance::Is_Numeric_Attribute(int attribute_num)const{
  if(Is_Valid_Attribute(attribute_num))
    return Attribute_List[attribute_num].Numeric();
  else{
    cout << "Error- illegal attribute number: " << attribute_num << endl;
    exit(1);
  }
}

string Instance::Get_Attribute_Name(int attribute_num){
  if(Is_Valid_Attribute(attribute_num))
    return Attribute_List[attribute_num].Get_Name();
  else{
    cout  << "Error- illegal attribute number: " << attribute_num << endl;
    exit(1);
  }
}

Attribute Instance::Get_Attribute(int attribute_num) const{
  if(Is_Valid_Attribute(attribute_num))
    return Attribute_List[attribute_num];
  else{
    cout  << "Error- illegal attribute number: " << attribute_num << endl;
    exit(1);
  }
}

bool Instance::Is_Valid_Attribute(int attribute_num) const{
  if(attribute_num >= 0 && attribute_num < Attribute_List.size())
    return true;
  else
    return false;
}

int Instance::Get_Num_Attributes() const{
  return Attribute_List.size();
}

