#pragma once

#include "ui/camera.h"

#include <Eigen/Dense>

namespace ui
{

class OrthoCamera : public Camera
{
  public:
    OrthoCamera(
        Eigen::Vector2f film_dimensions,
        float           near,
        float           far);

    virtual Eigen::Matrix4f projection() const override;
  
  private:
    void update_projection();
    
    Eigen::Matrix4f m_projection;
    Eigen::Vector2f m_film_dimensions;
    float           m_near;
    float           m_far;
    float           m_zoom;
};

} // namespace ui
