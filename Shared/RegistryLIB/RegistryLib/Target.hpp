#ifndef TARGET_HPP
#define TARGET_HPP

#include <set>

class Target
{
public:
  Target(int x,
         int y,
         int id,
         std::set<int> dependents = {},
         int falseAfter = 0);

  int getX() const;
  int getY() const;
  int getId() const;
  std::set<int> getDependents() const;
  int getFalseAfter() const;
  void removeDependent(int id);
  bool isReady() const;

private:
  int m_x;
  int m_y;
  int m_id;
  std::set<int> m_dependents;
  int m_falseAfter;
};

bool operator==(const Target& lhs, const Target& rhs);

#endif
