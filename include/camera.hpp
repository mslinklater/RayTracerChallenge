#pragma once
#include "matrix.hpp"
#include "ray.hpp"

class World;

/**
 * @brief Perspective camera that generates primary rays for each pixel.
 *
 * The camera is defined by its image resolution, field of view, and a
 * view transform (set via @c Matrix::ViewTransform). Use @c RayForPixel
 * to obtain the ray that passes through the centre of a given pixel.
 */
class Camera
{
public:
    /**
     * @brief Constructs a camera with the given resolution and field of view.
     * @param hsize      Horizontal image resolution in pixels.
     * @param vsize      Vertical image resolution in pixels.
     * @param fieldOfView Horizontal field of view in radians.
     */
    Camera(int hsize, int vsize, float fieldOfView);

    /** @brief Returns the horizontal image resolution in pixels. */
    int GetHSize() const;

    /** @brief Returns the vertical image resolution in pixels. */
    int GetVSize() const;

    /** @brief Returns the horizontal field of view in radians. */
    float GetFieldOfView() const;

    /** @brief Returns the camera-to-world (view) transform matrix. */
    Matrix GetTransform() const;

    /** @brief Replaces the camera-to-world (view) transform matrix. */
    void SetTransform(const Matrix &transform) { this->transform = transform; }

    /** @brief Returns the world-space width of a single pixel. */
    float GetPixelSize() const;

    /**
     * @brief Returns the world-space ray passing through pixel (@p px, @p py).
     * @param px Zero-based pixel column index.
     * @param py Zero-based pixel row index.
     * @return A normalised ray originating from the camera through the pixel centre.
     */
    Ray RayForPixel(int px, int py) const;

private:
    int hsize;          ///< Horizontal resolution in pixels.
    int vsize;          ///< Vertical resolution in pixels.
    float fieldOfView;  ///< Horizontal field of view in radians.
    Matrix transform;   ///< Camera-to-world transform (view matrix).
    float pixelSize;    ///< World-space width of a single pixel.
    float halfWidth;    ///< Half the world-space width of the image plane.
    float halfHeight;   ///< Half the world-space height of the image plane.
};
