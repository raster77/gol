#ifndef READER_ABSTRACTREADER_HPP_
#define READER_ABSTRACTREADER_HPP_

#include <fstream>
#include <string>
#include <vector>

class AbstractReader
{
  public:
    AbstractReader()
    {}

    virtual ~AbstractReader()
    {}

    void readFile(const std::string& file)
    {
      std::fstream dataFile(file);
      std::size_t lineNumber = 0;
      mDatas.clear();
      if(dataFile.is_open())
      {
	prepare(dataFile);
	dataFile.clear();
	dataFile.seekg(0);
	std::string line;
	while(getline(dataFile, line))
	{
	  processLine(line, lineNumber);
	  lineNumber++;
	}
	dataFile.close();
      }
    }

    const std::vector<std::pair<int, int>>& getDatas() const
    {
      return mDatas;
    }

  protected:
    std::vector<std::pair<int, int>> mDatas;

    virtual void prepare(std::fstream& f)
    {
      // virtual method
    }
    virtual void processLine(const std::string& line, const std::size_t lineNumber)
    {
      // virtual method
    }
};


#endif /* READER_ABSTRACTREADER_HPP_ */
