//
// Created by jzy99 on 2021/1/19.
//

#ifndef PHOENIX_INCLUDE_PHOENIX_CORE_PPM_H_
#define PHOENIX_INCLUDE_PHOENIX_CORE_PPM_H_


#pragma once
#include <string>
#include <eigen3/Eigen/Dense>
#include <vector>
#include<phoenix/core/phoenix.h>

PHOENIX_NAMESPACE_BEGIN


class PPM
{
 private:
  std::string file_name;
  std::vector<Eigen::Vector3f> data;
  int height, width;

 public:
  PPM(const std::string& file_name_,int width,int height);
  void setpixel(int x, int y, Eigen::Vector3f pixel);

  std::vector<Eigen::Vector3f>& getData();
  void write_file();
};

PHOENIX_NAMESPACE_END

#endif //PHOENIX_INCLUDE_PHOENIX_CORE_PPM_H_
