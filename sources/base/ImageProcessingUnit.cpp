/* ImageProcessingUnit.cpp
*
*  MIT License
*
*  Copyright (c) 2023 awawa-dev
*
*  Project homesite: https://github.com/awawa-dev/HyperHDR
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.

*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
 */

#include <utils/Image.h>
#include <base/HyperHdrInstance.h>
#include <base/ImageProcessingUnit.h>
#include <base/ImageProcessor.h>
#include <base/ImageToLedsMap.h>

// Blacborder includes
#include <blackborder/BlackBorderProcessor.h>

using namespace hyperhdr;


ImageProcessingUnit::ImageProcessingUnit(HyperHdrInstance* hyperhdr)
	: QObject(hyperhdr),
	_priority(-1),
	_hyperhdr(hyperhdr)
{
	
	connect(this, &ImageProcessingUnit::processImageSignal, this, &ImageProcessingUnit::processImage, Qt::ConnectionType::QueuedConnection);
	connect(this, &ImageProcessingUnit::clearQueueImageSignal, this, &ImageProcessingUnit::clearQueueImage);
	connect(this, &ImageProcessingUnit::queueImageSignal, this, &ImageProcessingUnit::queueImage);
}

ImageProcessingUnit::~ImageProcessingUnit()
{
	clearQueueImage();
}

void ImageProcessingUnit::queueImage(int priority, const Image<ColorRgb>& image)
{
	if (image.width() != 1 || image.height() != 1)
	{
		_frameBuffer = image;
		_priority = priority;

		emit processImageSignal();
	}
}

void ImageProcessingUnit::clearQueueImage()
{
	_frameBuffer = Image<ColorRgb>();
	_priority = -1;
}


void ImageProcessingUnit::processImage()
{
	if (_priority < 0 || (_frameBuffer.width() == 1 && _frameBuffer.height() == 1))
		return;
	
	ImageProcessor* imageProcessor = _hyperhdr->getImageProcessor();

	if (imageProcessor != nullptr)
	{
		imageProcessor->setSize(_frameBuffer);;
		imageProcessor->verifyBorder(_frameBuffer);

		std::shared_ptr<hyperhdr::ImageToLedsMap> image2leds = imageProcessor->_imageToLedColors;

		if (image2leds != nullptr && image2leds->width() == _frameBuffer.width() && image2leds->height() == _frameBuffer.height())
		{
			std::vector<ColorRgb> colors = image2leds->Process(_frameBuffer, imageProcessor->advanced);	

			_hyperhdr->updateLedsValues(_priority, colors);
			emit dataReadySignal(colors);
			emit _hyperhdr->onCurrentImage();
		}
	}

	clearQueueImage();
}
