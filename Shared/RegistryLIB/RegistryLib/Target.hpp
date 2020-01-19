#ifndef TARGET_HPP
#define TARGET_HPP

class Target
{
public:
  Target(double x, double y, int id) : m_x(x), m_y(y), m_id(id) {}

  double getX() const { return m_x; }
  double getY() const { return m_y; }
  int getId() const { return m_id; }

private:
  double m_x;
  double m_y;
  int m_id;
};

#endif
