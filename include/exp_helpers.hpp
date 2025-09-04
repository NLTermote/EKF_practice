/**
 * @brief Helpers for the experiments
 *
 * @author Juerd Mispelblom Beyer (juerd.mispelbombeijer@exrobotics.global)
 * @version 0.1
 * @date 2025-02-13
 */
#pragma once

#include <chrono>
//#include <iomanip>
#include <string>
//#include <tuple>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

/**
 * @brief A helper function to compute float time difference in seconds between two system_clock::time_point objects.
 */
static float timeDifferenceSeconds(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2)
{
	using namespace std::chrono;
	duration<float> diff = duration_cast<duration<float>>(t2 - t1);
	return diff.count();
}

/**
 * @brief Parses a timestamp in format "2025-01-09_11-51-34" to a std::chrono::system_clock::time_point.
 */
static std::chrono::system_clock::time_point parseTimestamp(const std::string& dateTime)
{
	int y, M, d, h, m, s;
	if (std::sscanf(dateTime.c_str(), "%d-%d-%d %d:%d:%d", &y, &M, &d, &h, &m, &s) != 6)
	{
		throw std::runtime_error("Invalid date/time format: " + dateTime);
	}
	//
	// Prepare struct tm
	std::tm tmStruct{};
	tmStruct.tm_year = y - 1900;
	tmStruct.tm_mon = M - 1;  // <-- months since January (0-11)
	tmStruct.tm_mday = d;
	tmStruct.tm_hour = h;
	tmStruct.tm_min = m;
	tmStruct.tm_sec = s;
	tmStruct.tm_isdst = -1;	 // Let mktime() figure out DST

	// Convert to time_t assuming the struct tm is local time:
	std::time_t t = std::mktime(&tmStruct);
	// If you *know* it's UTC, you might need platform-specific 'timegm(&tmStruct)'.

	if (t == -1)
	{
		throw std::runtime_error("mktime failed on: " + dateTime);
	}

	// Return a std::chrono::system_clock::time_point
	return std::chrono::system_clock::from_time_t(t);
}

namespace csv {

// Forward declaration
class CSVRow;

/**
 * @brief CSVFormat holds configurations for the CSV reader.
 */
class CSVFormat
{
   public:
	CSVFormat() : delimiter_(','), headerRow_(0) {}

	// Set the delimiter character
	CSVFormat& delimiter(char delim)
	{
		delimiter_ = delim;
		return *this;
	}

	// Indicate which row is the header row (0-based)
	CSVFormat& header_row(size_t rowIndex)
	{
		headerRow_ = rowIndex;
		return *this;
	}

	char getDelimiter() const { return delimiter_; }
	size_t getHeaderRow() const { return headerRow_; }

   private:
	char delimiter_;
	size_t headerRow_;
};

/**
 * @brief A single row of the CSV file. Allows accessing data by column name.
 */
class CSVRow
{
   public:
	// Constructor takes the parsed cells of this row and a map of column name -> index
	CSVRow(std::vector<std::string> data, const std::map<std::string, size_t>& headerIndex)
		: data_(std::move(data)), headerIndex_(headerIndex)
	{}

	// Get cell by column index
	std::string get(size_t index) const
	{
		if (index < data_.size())
		{
			return data_[index];
		}
		return {};
	}

	// Get cell by column name
	std::string get(const std::string& colName) const
	{
		auto it = headerIndex_.find(colName);
		if (it != headerIndex_.end())
		{
			return get(it->second);
		}
		return {};
	}

	// Optional: overload operator[] for convenience
	std::string operator[](const std::string& colName) const { return get(colName); }

   private:
	std::vector<std::string> data_;
	const std::map<std::string, size_t>& headerIndex_;
};

/**
 * @brief CSVReader is responsible for reading and storing CSV data.
 *        It provides range-based for loop iteration over CSVRow objects.
 */
class CSVReader
{
   public:
	// The reader is configured by a file path and a CSVFormat object
	CSVReader(const std::string& filepath, const CSVFormat& format) : filepath_(filepath), format_(format)
	{
		parseFile();
	}

	/**
	 * @brief A simple iterator class so we can use range-based for loops (for (auto &row : reader) {...}).
	 */
	class iterator
	{
	   public:
		using iterator_category = std::input_iterator_tag;
		using value_type = CSVRow;
		using difference_type = std::ptrdiff_t;
		using pointer = CSVRow*;
		using reference = CSVRow&;

		iterator(std::vector<CSVRow>* rows, size_t pos) : rows_(rows), pos_(pos) {}

		iterator& operator++()
		{
			++pos_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const iterator& other) const { return (rows_ == other.rows_) && (pos_ == other.pos_); }

		bool operator!=(const iterator& other) const { return !(*this == other); }

		reference operator*() { return (*rows_)[pos_]; }

		pointer operator->() { return &((*rows_)[pos_]); }

	   private:
		std::vector<CSVRow>* rows_;
		size_t pos_;
	};

	// Returns iterator pointing to the first row
	iterator begin() { return iterator(&rows_, 0); }

	// Returns iterator pointing one past the last row
	iterator end() { return iterator(&rows_, rows_.size()); }

   private:
	/**
	 * @brief Actually reads the file from disk, splits lines, stores rows.
	 */
	void parseFile()
	{
		std::ifstream file(filepath_);
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open CSV file: " + filepath_);
		}

		std::string line;
		size_t currentRow = 0;
		while (std::getline(file, line))
		{
			auto cells = splitLine(line, format_.getDelimiter());

			// If this is the header row, record the column name -> index mapping
			if (currentRow == format_.getHeaderRow())
			{
				for (size_t i = 0; i < cells.size(); ++i)
				{
					headerIndex_[cells[i]] = i;
				}
			}
			// If this is after the header row, store it in rows_
			else if (currentRow > format_.getHeaderRow())
			{
				rows_.emplace_back(CSVRow(cells, headerIndex_));
			}

			currentRow++;
		}
		file.close();
	}

	/**
	 * @brief Splits a single line into tokens using the delimiter.
	 */
	static std::vector<std::string> splitLine(const std::string& line, char delimiter)
	{
		std::vector<std::string> result;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, delimiter))
		{
			result.push_back(cell);
		}
		return result;
	}

	// Member variables
	std::string filepath_;
	CSVFormat format_;
	std::map<std::string, size_t> headerIndex_;
	std::vector<CSVRow> rows_;
};

}  // namespace csv