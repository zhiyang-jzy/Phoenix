//
// Created by jzy99 on 2021/1/9.
//

#ifndef PHOENIX_FILM_H
#define PHOENIX_FILM_H


#include<phoenix/core/phoenix.h>
#include<phoenix/core/object.h>

PHOENIX_NAMESPACE_BEGIN

class Film: public PhoenixObject{
 public:
  explicit Film(const PropertyList& prop){}
  [[nodiscard]] PClassType GetClassType()const override{return PClassType::PFilm;}
  [[nodiscard]] string ToString()const override{return "ok";}
 public:
  Point2i full_resolution_;

};



PHOENIX_NAMESPACE_END


#endif //PHOENIX_FILM_H
