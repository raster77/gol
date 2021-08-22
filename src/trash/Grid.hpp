#ifndef GRID_HPP_
#define GRID_HPP_

#include <vector>

template <typename T> class Grid
{
  public:
    Grid()
      : mRows(0)
      , mCols(0)
      , mDatas(0)
    {}

    Grid(const std::size_t rows, const std::size_t cols)
      : mRows(rows)
      , mCols(cols)
      , mDatas(rows * cols)
    {}

    ~Grid()
    {}

    void set(const std::size_t row, const std::size_t col, const T& v)
    {
      mDatas[getIndex(row, col)] = v;
    }
/*
    const T& get(const std::size_t row, const std::size_t col) const
    {
      const std::size_t idx = getIndex(row, col);
      return mDatas[idx];
    }
*/
    T& get(const std::size_t row, const std::size_t col)
    {
      const std::size_t idx = getIndex(row, col);
      return mDatas[idx];
    }

    std::vector<T> neightnors(const std::size_t row, const std::size_t col)
    {
      std::vector<T> res;
      for(int r = -1; r <= 1; ++r)
      {
	int x = row + r;
	if(x >= 0 && x < mRows) {
	  for(int c = -1; c <= 1; ++c)
	  {
	    if(r != 0 || c != 0) {
	      int y = col + c;
	      if((y >= 0 && y < mCols))
	      {
		  res.emplace_back(mDatas[getIndex(static_cast<std::size_t>(x), static_cast<std::size_t>(y))]);
	      }
	    }
	  }
	}
      }
      return res;
    }

    const int rows() const
    {
      return mRows;
    }

    const int cols() const
    {
      return mCols;
    }

  private:
    const int mRows;
    const int mCols;
    std::vector<T> mDatas;

    std::size_t getIndex(const std::size_t row, const std::size_t col)
    {
      return row + (col * mRows);
    }
};

#endif /* GRID_HPP_ */
