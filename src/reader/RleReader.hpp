#ifndef READER_RLEREADER_HPP_
#define READER_RLEREADER_HPP_

#include "AbstractReader.hpp"

class RleReader final : public AbstractReader
{
  public:

    RleReader()
      : AbstractReader()
      , total(0)
      , x(0)
      , y(0)
    {}

    ~RleReader()
    {}

  protected:
    inline void prepare(std::fstream& f) override
    {
      total = 0;
      x = 0;
      y = 0;
    }

    inline void processLine(const std::string& line, const std::size_t lineNumber) override
    {
      if(line[0] != '#' && line[0] != 'x')
      {
        for(auto& c : line) {
	  unsigned int num = total == 0 ? 1 : total;
	  if(c == 'b') {
	    while(num-- > 0)
	    {
	      x++;
	    }
	    total = 0;
	  } else if(c == 'o') {
	    while(num-- > 0)
	    {
	      mDatas.emplace_back(std::make_pair(x++, y));
	    }
	    total = 0;
	  } else if(c == '$') {
	      y += num;
	      x = 0;
	      total = 0;
	  } else if(c >= '0' && c <= '9') {
	      total = total * 10 + c - '0';
	  }
        }
      }
    }

  private:
    unsigned int total;
    unsigned int x;
    unsigned int y;
};

#endif /* READER_RLEREADER_HPP_ */
