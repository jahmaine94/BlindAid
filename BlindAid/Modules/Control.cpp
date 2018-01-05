#include "Control.h"
#include "ControlRealtime.h"
#include "ControlSimulate.h"

namespace Control
{
  Base *Base::MakeControl(IParameters *params, IData *input, IData *output)
  {
    if (((Control::Parameters*)params)->GetMode() == SwitchableParameters::Mode::Realtime)
      return new Realtime::Realtime(params, input, output);
    else
      return new Simulate::Simulate(params, input, output);
  }

  Base::Base(IParameters *params, IData *input, IData *output) : IModule(params, input, output)
  {
    for (int i = 0; i < VERT_REGIONS + 2; ++i) _vibrationIntensity[i] = new Result(_params->GetConsecutiveCount());
  }

  void Base::MapVibrationValues()
  {
    for (int i = 0; i < VERT_REGIONS; ++i)
    {
      float intensity = 0.f;
      for (int j = 0; j < HORZ_REGIONS; ++j)
        intensity = max(intensity, MappingFunction((float)(_input->GetDepthObstacleResults()->GetRegionIntensity(_params->GetHandPolarity() ? i : VERT_REGIONS - i - 1, j)), i, j)); // map the value to a vibration intensity ratio, and the maximum for that finger.

      _vibrationIntensity[i]->Update(intensity);

      _input->GetVibrationImage()->at<uchar>(cv::Point(i, 0)) = (int)_vibrationIntensity[i]->Get();
    }
  }

  float Base::MappingFunction(float distance, int col, int row)
  {
    float slope = (_params->GetMaximumVibration() - _params->GetMinimumVibration()) / (_params->GetFarthestBound(col, row) - _params->GetNearestBound()); // calculate the slope between nearest and farthest points.
    return min(_params->GetMaximumVibration(), _params->GetMaximumVibration() - max(0.f, distance - _params->GetNearestBound()) * slope);
  }
}