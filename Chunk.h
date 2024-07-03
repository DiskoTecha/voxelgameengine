#ifndef CHUNK_H
#define CHUNK_H

#include "Vector3.h"
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace dw;

namespace dw
{
	class Color4
	{
	public:
		float r;
		float g;
		float b;
		float a;

		Color4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	};

	class Chunk
	{
	private:
		size_t size;
		std::vector<float> data;

		Vector3 min;
		Vector3 max;

		Vector3 voxelOffset;
		float voxelSize;

	public:
		Chunk(size_t size, Vector3 min, float voxelSize) : size(size), min(min), max(min + size), voxelOffset(min* voxelSize), voxelSize(voxelSize)
		{
			clearVoxels();
		}

		void clearVoxels()
		{
			data = std::vector<float>(size * size * size * 4 + 4, 0.0f);
			data[0] = min.x;
			data[1] = min.y;
			data[2] = min.z;
			data[3] = voxelSize;
		}

		Color4 operator()(int x, int y, int z)
		{
			if (x < 0 || x >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (y < 0 || y >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (z < 0 || z >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}

			int startingIndex = x * 4 + y * 4 * size + z * 4 * size * size + 4;
			return Color4(data.at(startingIndex), data.at(startingIndex + 1), data.at(startingIndex + 2), data.at(startingIndex + 3));
		}

		void set(int x, int y, int z, Color4 value)
		{
			if (x < 0 || x >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (y < 0 || y >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (z < 0 || z >= size)
			{
				throw std::invalid_argument("received out of bounds value");
			}

			int startingIndex = x * 4 + y * 4 * size + z * 4 * size * size + 4;
			data.at(startingIndex) = value.r;
			data.at(startingIndex + 1) = value.g;
			data.at(startingIndex + 2) = value.b;
			data.at(startingIndex + 3) = value.a;
		}

		const int& voxel_amount() const
		{
			return size * size * size;
		}

		// Returns minimum in voxel units
		const Vector3& getMinVoxelPosition() const
		{
			return min;
		}

		// Returns maximum in voxel units
		Vector3 getMaxVoxelPosition()
		{
			return max;
		}

		// Returns max voxel index per side (x, y, or z)
		int getMaxVoxelIndex()
		{
			return size - 1;
		}

		double getVoxelSize()
		{
			return voxelSize;
		}

		Vector3 getVoxelOffset()
		{
			return voxelOffset;
		}

		float* getArrayPointer()
		{
			return data.data();
		}

		size_t getArrayPointerByteSize()
		{
			return sizeof(float) * data.size();
		}

		void setRegion(int xMinIndex, int xMaxIndex, int yMinIndex, int yMaxIndex, int zMinIndex, int zMaxIndex, Color4 value)
		{
			if (xMinIndex > xMaxIndex)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (yMinIndex > yMaxIndex)
			{
				throw std::invalid_argument("received out of bounds value");
			}
			else if (zMinIndex > zMaxIndex)
			{
				throw std::invalid_argument("received out of bounds value");
			}

			for (int x = xMinIndex; x <= xMaxIndex; x++)
			{
				for (int y = yMinIndex; y <= yMaxIndex; y++)
				{
					for (int z = zMinIndex; z <= zMaxIndex; z++)
					{
						set(x, y, z, value);
					}
				}
			}
		}
	};
}

#endif
