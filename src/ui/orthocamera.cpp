#include "ui/orthocamera.h"

#include <Eigen/Dense>

#include <utility>

namespace ui
{

OrthoCamera::OrthoCamera(
    Eigen::Vector2f film_dimensions,
    float           near,
    float           far)
  : m_film_dimensions(std::move(film_dimensions))
  , m_near(near)
  , m_far(far)
  , m_zoom(1.0f)
{
    update_projection();
}

Eigen::Matrix4f OrthoCamera::projection() const
{
    return m_projection;
}

void OrthoCamera::update_projection()
{
    float x_dim = m_zoom * m_film_dimensions.x();
    float y_dim = m_zoom * m_film_dimensions.y();

    m_projection <<
        2.0f / x_dim, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / y_dim, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (m_far - m_near), 0.0f,
        0.0f, 0.0f, -(m_far + m_near) / (m_far - m_near), 1.0f;
}

} // namespace ui
