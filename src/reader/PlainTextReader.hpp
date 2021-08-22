#ifndef READER_PLAINTEXTREADER_HPP_
#define READER_PLAINTEXTREADER_HPP_

#include "AbstractReader.hpp"
#include <limits>

class PlainTextReader final : public AbstractReader
{
  public:
    PlainTextReader()
      : AbstractReader()
      , start(std::numeric_limits<unsigned int>::max())
    {}

    ~PlainTextReader()
    {}

  protected:
    inline void prepare(std::fstream& f) override
    {
      start = std::numeric_limits<unsigned int>::max();
      std::size_t idx = 0;
      std::string line;
      while(getline(f, line))
      {
	if(line.length() > 0 && line[0] != '!')
	{
	  start = std::min(start, idx);
	}
	idx++;
      }
    }

    inline void processLine(const std::string& line, const std::size_t lineNumber) override
    {
      if(line.length() > 0 && line[0] != '!')
      {
	std::size_t idx = 0;
	for(auto& c : line)
	{
	  if(c == 'O')
	  {
	    mDatas.emplace_back(std::make_pair(idx, lineNumber - start));
	  }
	  idx++;
	}
      }
    }

  private:
    std::size_t start;
};

#endif /* READER_PLAINTEXTREADER_HPP_ */
