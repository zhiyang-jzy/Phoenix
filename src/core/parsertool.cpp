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
Eigen::Matrix4f str_to_matrix(const string &str){
  std::vector<std::string> tokens = tokenize(str);
  Eigen::Matrix4f res;

  std::vector<float> float_res(16);

  for(int i=0;i<16;i++)
    float_res[i] = str_to_float(tokens[i]);

  if(tokens.size()!=16){
    spdlog::error("wrong matrix");
    return Eigen::Matrix4f::Identity();
  }

  res<< float_res[0],float_res[4],float_res[8],float_res[12],
      float_res[1],float_res[5],float_res[9],float_res[13],
      float_res[2],float_res[6],float_res[10],float_res[14],
      float_res[3],float_res[7],float_res[11],float_res[15];

  res.transposeInPlace();
  return res;
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

