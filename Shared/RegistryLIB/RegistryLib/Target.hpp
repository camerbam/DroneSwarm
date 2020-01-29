#ifndef TARGET_HPP
#define TARGET_HPP

#include <set>

class Target
{
public:
  Target(double x,
         double y,
         int id,
         std::set<int> dependents = {},
         int falseAfter = 0);

  double getX() const;
  double getY() const;
  int getId() const;
  std::set<int> getDependents() const;
  int getFalseAfter() const;
  void removeDependent(int id);
  bool isReady() const;

private:
  double m_x;
  double m_y;
  int m_id;
  std::set<int> m_dependents;
  int m_falseAfter;
};

bool operator==(const Target& lhs, const Target& rhs);

#endif
