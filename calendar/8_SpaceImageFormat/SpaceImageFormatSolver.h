#include <ProblemSolver.h>

#include <CommonDefines.h>

#include <vector>

struct SpaceImageInput
{
	SpaceImageInput(uint width, uint height, std::string& inputFileName)
		: m_Width(width), m_Height(height), m_InputFileName(inputFileName)
	{ }

	uint m_Width, m_Height;
	std::string m_InputFileName;
};

class SpaceImage
{
public:
	enum class ImageColor : std::uint8_t
	{
		Black = 0,
		White = 1,
		Transparent = 2
	};

	using Row = std::vector<ImageColor>;
	using Layer = std::vector<Row>;
	using ImageData = std::vector<Layer>;

	SpaceImage(std::string inputFileName, std::size_t width, std::size_t height);
	SpaceImage()
	{ }

	inline const ImageData& GetImageData() const { return m_ImageData; }

	Layer GetDecodedImage() const;

	static std::size_t CountValuesInLayer(const Layer& layer, ImageColor value);
	static std::string ConvertToASCII(const Layer& layer);

private:
	std::size_t m_Width = 0;
	std::size_t m_Height = 0;
	ImageData m_ImageData;
};

std::ostream& operator<<(std::ostream& os, const SpaceImage::Layer& layer);

class SpaceImageFormatSolver : public ProblemSolver<SpaceImageInput, std::size_t, std::string>
{
public:
	void Init(SpaceImageInput& input) override { m_Image = SpaceImage(input.m_InputFileName, input.m_Width, input.m_Height); };
	std::size_t SolveProblemA() const override;
	std::string SolveProblemB() const override;

private:
	SpaceImage m_Image;
};