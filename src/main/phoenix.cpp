#include<iostream>


#include <spdlog/spdlog.h>
#include<tfm/tinyformat.h>
#include<tinyxml2.h>

#include<filesystem>
using namespace std;
using namespace std::filesystem;

void search(tinyxml2::XMLNode* node)
{
    if(node== nullptr)
    {
        return;
    }

    if(node->ToComment())
    {
        return;
    }
    std::cout<<" === "<<node->Value()<<"\n";
    auto att = node->ToElement();


    if(att!= nullptr &&  att->FirstAttribute()!= nullptr)
    {
        auto sub_att = att->FirstAttribute();
        while(sub_att!= nullptr)

        {
            std::cout<< sub_att->Name()<<":"<< sub_att->Value()<<" -- \n";
            sub_att = sub_att->Next();
        }
    }

    cout<<std::endl;
    tinyxml2::XMLNode* child = node->FirstChild();
    while(child!= nullptr)
    {
        search(child);
        child=child->NextSibling();
    }

}


int main()
{
    path a(R"(C:\Users\jzy99\Desktop\test.xml)");
    spdlog::info(a.generic_string());
    tinyxml2::XMLDocument doc;
    doc.LoadFile(a.generic_string().c_str());
    auto root_elec = doc.RootElement();            //等同于不指定参数的FirstChildElement()

    search(root_elec);

//    std::cout << "---------------------teacher-----------------------" << std::endl;
//    auto child_elec = root_elec->FirstChildElement();
//    while (child_elec) {
//        std::cout << child_elec->Name()<<":";
//        std::cout<<child_elec->FirstAttribute()->Value();
//        child_elec = child_elec->NextSiblingElement();
//
//        std::cout<<std::endl;
//    }


}