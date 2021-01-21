//
// Created by jzy99 on 2021/1/18.
//

#include<phoenix/core/parsertool.h>
#include<phoenix/core/vector.h>

PHOENIX_NAMESPACE_BEGIN

float phoenix::str_to_float(const string &str) {
  char *end_ptr = nullptr;
  auto result = (float) strtof(str.c_str(), &end_ptr);
  return result;
}
Vector3f str_to_vector3f(const string &str) {
  std::vector<std::string> tokens = tokenize(str);
  Eigen::Vector3f result;
  for (int i=0; i<3; ++i)
    result[i] = str_to_float(tokens[i]);
  return result;
}


vector<string> tokenize(const string &s, const string &delim, bool includeEmpty) {
  std::string::size_type lastPos = 0, pos = s.find_first_of(delim, lastPos);
  std::vector<std::string> tokens;

  while (lastPos != std::string::npos) {
    if (pos != lastPos || includeEmpty)
      tokens.push_back(s.substr(lastPos, pos - lastPos));
    lastPos = pos;
    if (lastPos != std::string::npos) {
      lastPos += 1;
      pos = s.find_first_of(delim, lastPos);
    }
  }

  return tokens;
}
Point3f str_to_point3f(const string &str) {
  std::vector<std::string> tokens = tokenize(str);
  Point3f result;
  for (int i=0; i<3; ++i)
    result[i] = str_to_float(tokens[i]);
  return result;
}
int str_to_integer(const string &str) {
  char *end_ptr = nullptr;
  int result = (int) strtol(str.c_str(), &end_ptr, 10);
  return result;
}

PHOENIX_NAMESPACE_END

