#pragma once

#include "IModule.h"
#include "Capture.h"

namespace Capture
{
  namespace Simulate
  {
    class Simulate : public Base
    {
    public:
      Simulate(IParameters *params, IData *input, IData *output);

    private:
      void Process();
      void LoadVideo();
      void LoadPhoto();

      cv::VideoCapture _cap;
    };
  }
}