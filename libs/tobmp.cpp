#include "tobmp.h"

#include <iostream>
#include <fstream>

Color::Color()
    : r(0), g(0), b(0) {}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b) {}

Color::~Color() {}

Img::Img(int width, int height)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width*height)) {}

Img::~Img() {}

Color Img::GetColor(int x, int y) const {
  return m_colors[y*m_width + x];
}

void Img::SetColor(const Color& color, int y, int x) {
  m_colors[y*m_width + x].r = color.r;
  m_colors[y*m_width + x].g = color.g;
  m_colors[y*m_width + x].b = color.b;
}

void Img::Export(const std::string path) {
  std::ofstream f;
  f.open(path, std::ios::out | std::ios::binary);
  if (!f.is_open()) {
    std::cout << "Cannot create the file";
    exit(1);
  }
  unsigned char bmpPad[3] = {0, 0, 0};
  const int kPaddingAddition = ((4 - (m_width*3)%4)%4);
  const int kFileHeaderSize = 14;
  const int kInformationHeaderSize = 40;
  const int kFileSize = kFileHeaderSize + kInformationHeaderSize + m_width*m_height*3 + kPaddingAddition*m_height;

  const unsigned char kFileHeader[kFileHeaderSize] = {
      'B', 'M',
      kFileSize, kFileSize >> 8, kFileSize >> 16, kFileSize >> 24,
      0, 0,
      0, 0,
      kFileHeaderSize + kInformationHeaderSize, 0, 0, 0
  };

  const unsigned char kInformationHeader[kInformationHeaderSize] = {
      kInformationHeaderSize, 0, 0, 0,
      m_width, m_width >> 8, m_width >> 16, m_width >> 24,
      m_height, m_height >> 8, m_height >> 16, m_height >> 24,
      1, 0,
      24, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0
  };

  f.write(reinterpret_cast<const char*>(kFileHeader), kFileHeaderSize);
  f.write(reinterpret_cast<const char*>(kInformationHeader), kInformationHeaderSize);

  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      unsigned char r = static_cast<unsigned char>(GetColor(x, y).r*255.0f);
      unsigned char g = static_cast<unsigned char>(GetColor(x, y).g*255.0f);
      unsigned char b = static_cast<unsigned char>(GetColor(x, y).b*255.0f);
      unsigned char color[] = {b, g, r};
      f.write(reinterpret_cast<char*>(color), 3);
    }
    f.write(reinterpret_cast<char*>(bmpPad), kPaddingAddition);
  }
}