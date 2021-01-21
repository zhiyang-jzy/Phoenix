//
// Created by jzy99 on 2021/1/18.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_PARSERTOOL_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_PARSERTOOL_H_


#include <phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN

float str_to_float(const string& str);
Vector3f str_to_vector3f(const string& str);
Point3f str_to_point3f(const string& str);
vector<string> tokenize(const string &s, const string &delim = ", ", bool includeEmpty = false);
int str_to_integer(const string& str);


PHOENIX_NAMESPACE_END



#endif //PHOENIX_INCLUDE_PHOENIX_CORE_PARSERTOOL_H_
