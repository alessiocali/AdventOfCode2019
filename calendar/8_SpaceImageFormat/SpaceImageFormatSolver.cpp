#include <SpaceImageFormatSolver.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>

std::ostream& operator<<(std::ostream& os, const SpaceImage::Layer& layer)
{
	os << std::endl << std::endl;

	for (const SpaceImage::Row& row : layer)
	{
		for (const SpaceImage::ImageColor color : row)
		{
			switch (color)
			{
			default:
			case SpaceImage::ImageColor::Black:
			case SpaceImage::ImageColor::Transparent:
				os << ' ';
				break;
			case SpaceImage::ImageColor::White:
				os << '#';
				break;
			}
		}

		os << std::endl;
	}

	return os << std::endl;
}

std::size_t SpaceImage::CountValuesInLayer(const Layer& layer, ImageColor value)
{
	return std::accumulate(layer.cbegin(), layer.cend(), std::size_t(0),
	[value](std::size_t count, const Row& row)
	{
		const std::size_t rowCount = std::count(row.cbegin(), row.cend(), value);
		return count + rowCount;
	});
}

std::string SpaceImage::ConvertToASCII(const Layer& layer)
{
	// Simple method, but does a pointless copy of the stream buffer
	std::stringstream stringStream;
	stringStream << layer;
	return stringStream.str();
}

SpaceImage::Layer SpaceImage::GetDecodedImage() const
{
	Layer collapsedLayer(m_Height, Row(m_Width, ImageColor::Transparent));

	for (std::size_t i = 0; i < m_Height; i++)
	{
		for (std::size_t j = 0; j < m_Width; j++)
		{
			auto firstNonTransparent = std::find_if(m_ImageData.cbegin(), m_ImageData.cend(),
			[i, j](const Layer& layer)
			{
				return layer[i][j] != ImageColor::Transparent;
			});

			if (firstNonTransparent != m_ImageData.cend())
			{
				collapsedLayer[i][j] = (*firstNonTransparent)[i][j];
			}
		}
	}

	return collapsedLayer;
}

std::size_t SpaceImageFormatSolver::SolveProblemA() const
{
	auto minLayer = std::min_element(m_Image.GetImageData().begin(), m_Image.GetImageData().end(), 
	[](const auto& layer1, const auto& layer2)
	{
		return 
			SpaceImage::CountValuesInLayer(layer1, SpaceImage::ImageColor::Black) < 
			SpaceImage::CountValuesInLayer(layer2, SpaceImage::ImageColor::Black);
	});

	return	SpaceImage::CountValuesInLayer(*minLayer, SpaceImage::ImageColor::White) * 
			SpaceImage::CountValuesInLayer(*minLayer, SpaceImage::ImageColor::Transparent);
}

std::string SpaceImageFormatSolver::SolveProblemB() const
{
	return SpaceImage::ConvertToASCII(m_Image.GetDecodedImage());
}

SpaceImage::SpaceImage(std::string inputFileName, std::size_t width, std::size_t height)
{
	std::ifstream inputFile(inputFileName);

	m_Width = 0;
	m_Height = 0;

	if (!inputFile.is_open())
	{
		std::cerr << "Can't open file " << inputFileName << std::endl;
		return;
	}

	m_Width = width;
	m_Height = height;

	std::string imageString;
	std::string imageLine;
	while (inputFile >> imageLine)
	{
		imageString += imageLine;
	}

	std::size_t idx = 0;
	while (idx < imageString.size())
	{
		const std::size_t layerIdx = idx / (m_Width * m_Height);
		const std::size_t layerSubIdx = idx % (m_Width * m_Height);
		if (layerIdx >= m_ImageData.size())
		{
			m_ImageData.push_back(Layer());
		}
		Layer& layer = m_ImageData[layerIdx];

		const std::size_t rowIdx = layerSubIdx / width;
		if (rowIdx >= layer.size())
		{
			layer.push_back(Row());
		}
		Row& row = layer[rowIdx];

		row.push_back(static_cast<SpaceImage::ImageColor>(imageString[idx] - '0'));

		idx++;
	}
}