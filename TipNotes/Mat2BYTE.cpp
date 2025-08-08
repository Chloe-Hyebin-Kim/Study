//왜 cv::Mat BYTE 배열로 변환하는가?

/*
​

OpenCV cv::Mat은 내부적으로 2D 구조 (row, col)를 가지므로 접근할 때 ptr() 등의 간접 참조가 필요함.

SSE 또는 단순 반복 접근 성능을 위해, 1차원 평탄화된 BYTE 배열로 변환해서 직접 인덱싱 방식으로 빠르게 처리하려는 목적

src_img_greyvalue[y * width + x]처럼 직접 인덱싱을 통해 픽셀을 읽기 위함.

또한 cv::Mat은 레퍼런스 카운팅 구조이므로, 안전한 접근을 위해 복사본으로 가공하는 것일 수도 있음.

*/

void Mat2BYTE(const cv::Mat& imgSrc, std::vector<BYTE>& vecDst)
{
	if (imgSrc.isContinuous())
	{
		std::copy(imgSrc.datastart, imgSrc.dataend, vecDst.begin());
	}
	else
	{
		BYTE* p = vecDst.data();
		for (int i = 0; i < imgSrc.rows; ++i)
		{
			memcpy(p, imgSrc.ptr<BYTE>(i), imgSrc.cols * imgSrc.elemSize());
			p += imgSrc.cols * imgSrc.elemSize();
		}
	}
}
