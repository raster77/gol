#ifndef RULE_HPP_
#define RULE_HPP_

#include <algorithm>
#include <vector>

class Rule
{
  public:

    Rule()
    {}

    ~Rule()
    {}

    void set(std::initializer_list<unsigned short int> born, std::initializer_list<unsigned short int> alive)
    {
      mBorn = born;
      mAlive = alive;
      std::sort(mBorn.begin(), mBorn.end());
      std::sort(mAlive.begin(), mAlive.end());
    }

    unsigned short int check(const bool alive, const unsigned short int neighbours)
    {
      return (!alive && std::binary_search(mBorn.begin(), mBorn.end(), neighbours)) || (alive && std::binary_search(mAlive.begin(), mAlive.end(), neighbours)) ? 1 : 0;
    }

  private:
    std::vector<unsigned short int> mBorn;
    std::vector<unsigned short int> mAlive;
};



#endif /* RULE_HPP_ */
