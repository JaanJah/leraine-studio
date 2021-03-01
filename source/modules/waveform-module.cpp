#include "waveform-module.h"

bool WaveFormModule::StartUp() 
{
    return true;
}

void WaveFormModule::GenerateWaveForm(WaveFormData* const InWaveFormData, const Time InSongLengthMilliSeconds) 
{
    _WaveFormSlices.clear();

    for(Time time = 0; time < InSongLengthMilliSeconds; time += _SliceLength)
    {
        _WaveFormSlices[time].create(_WaveFormWidth, _SliceLength);
        _WaveFormSlices[time].clear({0, 0, 0, 0});

        for(Time waveFormTime = time; waveFormTime < time + _SliceLength; ++waveFormTime)
        {
            if(waveFormTime >= InSongLengthMilliSeconds)
                break;

            sf::Color frontColor = sf::Color(0, 255, 255, 192);
            sf::Color backColor  = sf::Color(255, 255, 0, 128);

            float left = InWaveFormData[waveFormTime].Left;// * 128.f;
            float right = InWaveFormData[waveFormTime].Right;// * 128.f;

            float totalLengthFront = pow((abs(left) + abs(right)) / 2.f, 1.5f) * 256.f;
            float totalLengthBack = (abs(left) + abs(right)) * 196.f;

            sf::RectangleShape lineBack(sf::Vector2f(totalLengthBack, 1));
            lineBack.setFillColor(backColor);
            lineBack.setPosition(_WaveFormWidth / 2 - totalLengthBack / 2, waveFormTime - time);
        
            _WaveFormSlices[time].draw(lineBack);

            sf::RectangleShape lineFront(sf::Vector2f(totalLengthFront, 1));
            lineFront.setFillColor(frontColor);
            lineFront.setPosition(_WaveFormWidth / 2 - totalLengthFront / 2, waveFormTime - time);
        
            _WaveFormSlices[time].draw(lineFront);
        }
    }
}

void WaveFormModule::RenderWaveForm(sf::RenderTarget* const InOutRenderTarget, const Time InTimeBegin, const Time InTimeEnd, const int InScreenX, const float InZoomLevel) 
{
    const float windowHeight = float(InOutRenderTarget->getView().getSize().y);
    const float scale = windowHeight / float(InTimeEnd - InTimeBegin); 

    Time firstTimePoint = InTimeBegin - (InTimeBegin % _SliceLength);

    for(Time time = firstTimePoint; time < InTimeEnd + _SliceLength; time += _SliceLength)
    {
        float vertexPositionY = floor(float(InTimeEnd - time - _SliceLength) * scale);

        _WaveFormSliceSprite.setTexture(_WaveFormSlices[time].getTexture());
        _WaveFormSliceSprite.setPosition(InScreenX - _WaveFormWidth / 2, vertexPositionY);
        _WaveFormSliceSprite.setScale({1.f, InZoomLevel});
        _WaveFormSliceSprite.setColor({255, 255, 255, 196});

        InOutRenderTarget->draw(_WaveFormSliceSprite);
    }
}