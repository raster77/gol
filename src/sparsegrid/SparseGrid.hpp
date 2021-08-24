#ifndef SPARSEGRID_HPP_
#define SPARSEGRID_HPP_

#include <algorithm>
#include <array>
#include <numeric>
#include <stdint.h>
#include <unordered_map>
#include "robin_hood.h"
#include <vector>

struct Cell
{
  int row;
  int col;
  unsigned short value;
  std::size_t age;

  Cell()
    : row(0)
    , col(0)
    , value(0)
    , age(0)
  {}

  Cell(const int row, const int col, const unsigned short value)
    : row(row)
    , col(col)
    , value(value)
    , age(0)
  {}

  Cell(const int row, const int col, const unsigned short value, const uint32_t age)
    : row(row)
    , col(col)
    , value(value)
    , age(age)
  {}

  bool operator==(const Cell& rhs) const
  {
    return row == rhs.row && col == rhs.col;
  }

  bool operator<(const Cell& rhs) const
  {
    return row < rhs.row && col < rhs.col;
  }

  bool operator<=(const Cell& rhs) const
  {
    return row <= rhs.row && col <= rhs.col;
  }

  bool operator>(const Cell& rhs) const
  {
    return row > rhs.row && col > rhs.col;
  }

  bool operator>=(const Cell& rhs) const
  {
    return row >= rhs.row && col >= rhs.col;
  }
};

struct Coord
{
    int64_t x;
    int64_t y;

    Coord()
      : x(0)
      , y(0)
    {}

    Coord(const int64_t x, const int64_t y)
      : x(x)
      , y(y)
    {}

    bool operator < (const Coord& c)
    {
      return x < c.x && y < c.y;
    }
};

class SparseGrid
{
  public:

    SparseGrid()
    {
      for(short int r = -1; r <= 1; ++r)
      {
	for(short int c = -1; c <= 1; ++c)
	{
	  std::pair<short int, short int> p = std::make_pair(r, c);
	  regions.emplace_back(p);
	  if(r != 0 || c != 0)
	  {
	    offsets.emplace_back(p);
	  }
	}
      }
    }

    ~SparseGrid()
    {}

    const Cell& get(const int row, const int col)
    {
      return mDatas[index(row, col)];
    }

    void set(const int row, const int col, const unsigned short int value)
    {
      mDatas[index(row, col)] = Cell(row, col, value);
    }

    void set(const int row, const int col, const Cell& cell)
    {
      mDatas[index(row, col)] = cell;
    }

    bool exists(const int row, const int col)
    {
      return mDatas.find(index(row, col)) != mDatas.end();
    }

    std::array<Cell, 9> neighbours(const int row, const int col)
    {
      std::array<Cell, 9> res;
      std::size_t i = 0;
      for(auto& r : regions)
      {
	const int x = row + r.first;
	const int y = col + r.second;
	const int64_t idx = index(x, y);
	if(mDatas.find(idx) != mDatas.end())
	{
	  res[i] = mDatas[idx];
	} else {
	  res[i] = Cell(x, y, 0);
	}
	i++;
      }

      return res;
    }

    std::size_t countNeighbours(const int row, const int col)
    {
      std::size_t count = 0;
      for(auto& o : offsets)
      {
	const int64_t idx = index(row + o.first, col + o.second);
	if(mDatas.find(idx) != mDatas.end())
	{
	  count++;
	}
      }
      return count;
    }

    void clear()
    {
      mDatas.clear();
    }

    void erase(const int row, const int col)
    {
      mDatas.erase(index(row, col));
    }

    const std::unordered_map<int64_t, Cell>::const_iterator begin()
    {
      return mDatas.begin();
    }

    const std::unordered_map<int64_t, Cell>::const_iterator end()
    {
      return mDatas.end();
    }

    const std::size_t size()
    {
      return mDatas.size();
    }

    Cell* operator[](const int64_t key)
    {
      return mDatas.find(key) != mDatas.end() ? &mDatas[key] : nullptr;
    }

    bool operator == (const SparseGrid& grid)
    {
      return mDatas == grid.mDatas;
    }

    bool operator != (const SparseGrid& grid)
    {
      return mDatas != grid.mDatas;
    }

  private:
    typedef std::unordered_map<int64_t, Cell> HashMap;
    typedef std::unordered_map<int64_t, Cell>::iterator HashMapIterator;
    HashMap mDatas;
    std::vector<std::pair<short int, short int>> offsets;
    std::vector<std::pair<short int, short int>> regions;
    static constexpr int64_t prime1 = 73856093;
    static constexpr int64_t prime2 = 19349663;
    static constexpr int64_t prime3 = 234713;
    static constexpr int64_t prime4 = 734687;

    static constexpr int64_t index(const int row, const int col)
    {
      return static_cast<int64_t>(((row + prime3) * prime1) ^ ((col + prime4) * prime2));
    }
};



#endif /* SPARSEGRID_HPP_ */
