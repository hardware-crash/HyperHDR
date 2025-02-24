/* Animation4Music_StereoMultiFast.cpp
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

#include <effectengine/Animation4Music_StereoMultiFast.h>
#include <base/SoundCapture.h>

Animation4Music_StereoMultiFast::Animation4Music_StereoMultiFast() :
	AnimationBaseMusic(AMUSIC_STEREOMULTIFAST),
	_internalIndex(0),
	_oldMulti(0)
{

};

EffectDefinition Animation4Music_StereoMultiFast::getDefinition()
{
	EffectDefinition ed;
	ed.name = AMUSIC_STEREOMULTIFAST;
	ed.args = GetArgs();
	return ed;
}

void Animation4Music_StereoMultiFast::Init(
	QImage& hyperImage,
	int hyperLatchTime
)
{
	SetSleepTime(15);
}

bool Animation4Music_StereoMultiFast::Play(QPainter* painter)
{
	return false;
}

bool Animation4Music_StereoMultiFast::hasOwnImage()
{
	return true;
};

bool Animation4Music_StereoMultiFast::getImage(Image<ColorRgb>& newImage)
{
	bool newData = false;
	auto r = SoundCapture::getInstance()->hasResult(this, _internalIndex, NULL, NULL, &newData, &_oldMulti);

	if (r == NULL || !newData)
		return false;

	QColor selected, empty;
	uint32_t maxSingle, average;

	newImage.clear();

	if (!r->GetStats(average, maxSingle, empty, &selected))
		return false;

	r->RestoreFullLum(selected);

	int value = r->getValue3Step(_oldMulti);

	int hm = (newImage.height() / 2);
	int h = std::min((hm * value) / 255, 255);
	int h1 = std::max(hm - h, 0);
	int h2 = std::min(hm + h, (int)newImage.height() - 1);

	if (h2 > h1)
	{
		int width = newImage.width() * 0.04;

		newImage.gradientVBox(0, h1, width, h2, selected.red(), selected.green(), selected.blue());
		newImage.gradientVBox(newImage.width() - 1 - width, h1, newImage.width() - 1, h2, selected.red(), selected.green(), selected.blue());
	}

	return true;
};







