#pragma once

#include <Eigen/Dense>

namespace ui
{

class Camera
{
  public:
    virtual Eigen::Matrix4f projection() const = 0;
    virtual ~Camera() = default;
};

} // namespace ui
