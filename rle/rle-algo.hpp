#pragma once
#include <ostream>

// RLE Compression/Decompression
template <typename T>
struct RLE
{
	// Memory which stores either compressed or decompressed data
	T* m_Data;
	// Number of elements of type T that data is pointing to
	int m_Size;

	RLE()
		: m_Data(nullptr)
		, m_Size(0)
	{ }

	~RLE()
	{
		delete m_Data;
	}

	// Compresses input data and stores it in m_Data
	void Compress(const T* input, int inSize);

	// Decompresses input data and stores it in m_Data
	void Decompress(const T* input, int inSize, int outSize);

	// Outputs m_Data
	friend std::ostream& operator<< (std::ostream& stream, const RLE<T>& rhs)
	{
		for (int i = 0; i < rhs.m_Size; i++)
		{
			stream << rhs.m_Data[i];
		}
		return stream;
	}

	// Shenanigans to get the maximum run size
	int MaxRunSize();
};

template <typename T>
void RLE<T>::Compress(const T* input, int inSize)
{
	//clear previous data
	delete m_Data;
	m_Size = 0;

	// get maximum size
	int max_size = MaxRunSize();

	// allocate memory for m_Data
	m_Data = new T[inSize * 2];

	// main compressing algorithm

	const T* temp = input;

	int count = 0, negative_digit = 0;

	while (inSize > 0)
	{
		// Normal runs
		if (*temp == *(temp + 1))
		{
			count = 1;

			while (*temp == *(temp + 1) && inSize > 0 && count < max_size)
			{
				count++;
				temp++;
				inSize--;
			}

			m_Data[m_Size] = count;
			m_Size++;

			m_Data[m_Size] = *temp;
			m_Size++;

			temp++;
			inSize--;
		}
		// Negative runs
		else
		{
			count = 0;

			negative_digit = m_Size;

			m_Data[m_Size] = *temp;
			m_Size++;

			while (*temp != *(temp + 1) && inSize > 0)
			{
				
				m_Data[m_Size] = *temp;
				m_Size++;
				count--;
				temp++;
				inSize--;
			}
			m_Data[negative_digit] = count;
		}

	}
	
}

template <typename T>
void RLE<T>::Decompress(const T* input, int inSize, int outSize)
{
	//clear previous data
	delete m_Data;
	m_Size = 0;

	// allocate memory for m_Data
	m_Data = new T[outSize];

	// Iterating pointer temp starting at the head of input stream
	const T* temp = input;
	int negative_count = 0;

	while (inSize > 0)
	{
		if (*temp > 0)
		{
			for (int i = 0; i < *temp; i++)
			{
				m_Data[m_Size] = *(temp + 1);
				m_Size++;
			}

			temp += 2;
			inSize -= 2;
		}

		else
		{
			negative_count = -(*temp);
			temp++;
			inSize--;

			for (int i = 0; i < negative_count; i++)
			{
				m_Data[m_Size] = *temp;
				m_Size++;
				temp++;
				inSize--;
			}
		}
	}
	std::cout << "Decompression Finished." << std::endl;
	
}
