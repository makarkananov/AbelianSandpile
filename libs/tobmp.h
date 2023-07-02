#include <string>
#include <vector>

struct Color {
  float r;
  float g;
  float b;
  Color();
  Color(float r, float g, float b);
  ~Color();
};

class Img {
 public:
  Img(int width, int height);
  ~Img();
  Color GetColor(int x, int y) const;
  void SetColor(const Color& color, int x, int y);
  void Export(const std::string path);

 private:
  int m_width;
  int m_height;
  std::vector<Color> m_colors;
};