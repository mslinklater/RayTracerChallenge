#pragma once

#include "color.hpp"
#include <string>
#include <vector>

/**
 * @brief A 2D pixel buffer that can be written to a PPM image file.
 *
 * Pixels are stored in row-major order. Coordinates are zero-based with
 * (0, 0) at the top-left corner.
 */
class Canvas
{
  public:
    /**
     * @brief Constructs a canvas filled with black pixels.
     * @param width  Number of pixel columns.
     * @param height Number of pixel rows.
     */
    Canvas(int width, int height);

    /** @brief Returns the number of pixel columns. */
    int GetWidth() const;

    /** @brief Returns the number of pixel rows. */
    int GetHeight() const;

    /**
     * @brief Returns the colour of the pixel at (@p x, @p y).
     * @param x Zero-based column index.
     * @param y Zero-based row index.
     */
    Color GetPixel(int x, int y) const;

    /**
     * @brief Sets the colour of the pixel at (@p x, @p y).
     * @param x     Zero-based column index.
     * @param y     Zero-based row index.
     * @param color The colour to write.
     */
    void WritePixel(int x, int y, const Color& color);

    /**
     * @brief Writes the canvas contents to a PPM file.
     * @param filename Path to the output file (will be created or overwritten).
     */
    void WriteToPPM(const std::string& filename) const;

  private:
    int width;                 ///< Number of pixel columns.
    int height;                ///< Number of pixel rows.
    std::vector<Color> pixels; ///< Flat row-major pixel storage.
};

/**
 * @brief Compares two canvases pixel-by-pixel using floating-point tolerance.
 * @return true if both canvases have the same dimensions and all pixels match.
 */
bool operator==(const Canvas& lhs, const Canvas& rhs);
