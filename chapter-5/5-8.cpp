#include "bit.hpp"

#include "gmock/gmock.h"
using namespace ::testing;

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

template <std::size_t num_cells>
void draw_horizontal_line(std::array<std::uint8_t, num_cells>& screen,
                          const int width,
                          const int x1,
                          const int x2,
                          const int y)
{
  if ((width <= 0) || (width % 8 != 0))
  {
    throw std::invalid_argument("width must be positive and divisible by 8");
  } else if ((x1 < 0) || (x1 > width))
  {
    throw std::invalid_argument("x1 argument out of bounds");
  }
  else if ((x2 < 0) || (x2 > width))
  {
    throw std::invalid_argument("x2 argument out of bounds");
  }
  else if (x1 > x2)
  {
    throw std::invalid_argument("x1 greater than x2");
  }

  const auto num_x_cells = width / 8;
  const auto num_y_cells = num_cells / num_x_cells;

  if ((y < 0) || (y >= num_y_cells))
  {
    throw std::invalid_argument("y argument out of bounds");
  }

  for (int x = x1; x <= x2; x = (x | 7) + 1)
  {
    const auto cell = (y * num_x_cells) + (x >> 3);
    const auto p1   = std::min(x, x2) & 7;
    const auto p2   = std::min(x | 7, x2) & 7;

    screen[cell] |= bit::mask<std::uint8_t>(7 - p2, 7 - p1);
  }
}

template <std::size_t num_cells>
std::string output_screen(
  const std::array<std::uint8_t, num_cells>& screen, const int width)
{
  std::string output{};

  const auto num_x_cells = width / 8;
  const auto num_y_cells = num_cells / num_x_cells;

  for (auto y = 0; y != num_y_cells; ++y)
  {
    for (auto x = 0; x != num_x_cells; ++x)
    {
      const std::uint8_t cell = screen[y * num_x_cells + x];

      for (const auto& bit : bit::make_reverse_range(cell))
      {
        output += (bit ? u8"\u2588" : u8"\u2591");
      }
    }

    if (y != num_y_cells)
    {
      output += '\n';
    }
  }

  return output;
}

TEST(draw_horizontal_line, verify)
{
  const std::array<std::uint8_t, 4> expected =
  {
    0b11011111, 0b11111011,
    0b01101100, 0b00110110
  };

  std::array<std::uint8_t, 4> screen =
  {
    0b11000000, 0b00000011,
    0b01100000, 0b00000110
  };

  draw_horizontal_line(screen, 16,  3, 12, 0);
  draw_horizontal_line(screen, 16,  4,  5, 1);
  draw_horizontal_line(screen, 16, 10, 11, 1);

  ASSERT_EQ(expected, screen);
}
