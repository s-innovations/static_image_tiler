
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <msclr\marshal_cppstd.h>

using namespace cv;


void handleColumn(unsigned int yTiles, unsigned int x, unsigned int tileSize, unsigned int lod, const cv::Size & imgSize, cv::Mat & img, const cv::Mat & black, std::vector<std::vector<cv::Mat>> & baseTiles, bool last)
{
	baseTiles[x].resize(yTiles + (yTiles % 2) * 2);
	for (int y = 0; y < yTiles; ++y)
	{
		if (last)
		{
			baseTiles[x][y] = cv::Mat::zeros(tileSize, tileSize, img.type());
			img(cv::Rect(x*tileSize, yTiles*tileSize, imgSize.width%tileSize, tileSize))
				.copyTo(baseTiles[x][yTiles](cv::Rect(0, 0, tileSize, imgSize.height%tileSize)));
		}
		else{

			img(cv::Rect(x*tileSize, y*tileSize, tileSize, tileSize)).copyTo(baseTiles[x][y]);

		}
		auto path = System::String::Format("../data/out/{0}/{1}/{2}.jpg", lod, x, y);
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
		auto cstr = msclr::interop::marshal_as< std::string >(path);
		imwrite(cstr, baseTiles[x][y]);
	}
	if (yTiles % 2 == 1)
	{

		baseTiles[x][yTiles] = cv::Mat::zeros(tileSize, tileSize, img.type());
		img(cv::Rect(x*tileSize, yTiles*tileSize, tileSize, imgSize.height%tileSize))
			.copyTo(baseTiles[x][yTiles](cv::Rect(0, 0, tileSize, imgSize.height%tileSize)));
		auto path = System::String::Format("../data/out/{0}/{1}/{2}.jpg", lod, x, yTiles);
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
		auto cstr = msclr::interop::marshal_as< std::string >(path);
		imwrite(cstr, baseTiles[x][yTiles]);

		black.copyTo(baseTiles[x][yTiles + 1]);

	}
}


int main()
{
	cv::Mat img = imread("../data/DSC_2299.JPG");
	const Size & imgSize = img.size();
	int tileSize = 256;

	int xTiles = imgSize.width / tileSize;
	int yTiles = imgSize.height / tileSize;

	unsigned int lodx = ceil(log2(xTiles));
	unsigned int lody = ceil(log2(yTiles));
	unsigned int lod = max(lodx, lody);


	cv::Mat black = cv::Mat::zeros(tileSize, tileSize, img.type());
	std::vector<std::vector<cv::Mat>> baseTiles;
	baseTiles.resize(xTiles + (xTiles % 2) * 2);

	for (int x = 0; x < xTiles; ++x)
	{
		handleColumn(yTiles, x, tileSize, lod, imgSize, img, black, baseTiles, false);
	}
	if (xTiles % 2 == 1)
	{
		handleColumn(yTiles, xTiles, tileSize, lod, imgSize, img, black, baseTiles, true);

		for (int y = 0; y < yTiles + (yTiles % 2) * 2; ++y)
			black.copyTo(baseTiles[xTiles + 1][y]);

	}

	while (lod-- > 0)
	{
		std::vector<std::vector<cv::Mat>> newBaseTiles;
		newBaseTiles.resize(baseTiles.size() / 2 + (baseTiles.size() / 2 % 2));
		baseTiles.resize(newBaseTiles.size() * 2);



		for (int x = 0; x < newBaseTiles.size(); ++x)
		{
			newBaseTiles[x].resize(baseTiles[x].size() / 2 + (baseTiles[x].size() / 2 % 2));

			while ((newBaseTiles[x].size() * 2 > baseTiles[x * 2].size()))
			{
				baseTiles[x * 2].push_back(black);
				baseTiles[x * 2 + 1].push_back(black);
			}



			for (int y = 0; y < newBaseTiles[x].size(); ++y)
			{
				newBaseTiles[x][y] = cv::Mat(tileSize * 2, tileSize * 2, img.type());


				baseTiles[x * 2][y * 2].copyTo(newBaseTiles[x][y](cv::Rect(0, 0, tileSize, tileSize)));
				baseTiles[x * 2][y * 2 + 1].copyTo(newBaseTiles[x][y](cv::Rect(0, tileSize, tileSize, tileSize)));
				baseTiles[x * 2 + 1][y * 2].copyTo(newBaseTiles[x][y](cv::Rect(tileSize, 0, tileSize, tileSize)));
				baseTiles[x * 2 + 1][y * 2 + 1].copyTo(newBaseTiles[x][y](cv::Rect(tileSize, tileSize, tileSize, tileSize)));

				cv::resize(newBaseTiles[x][y], newBaseTiles[x][y], cv::Size(tileSize, tileSize), 0, 0, INTER_LANCZOS4);
				auto path = System::String::Format("../data/out/{0}/{1}/{2}.jpg", lod, x, y);
				System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
				auto cstr = msclr::interop::marshal_as< std::string >(path);
				imwrite(cstr, newBaseTiles[x][y]);
			}

		}
		baseTiles = newBaseTiles;

	}

	return 0;
}
