#include "monitor.h"

constexpr auto POLLING_PERIOD{std::chrono::milliseconds(1000)};

int main(int argc, char **argv)
{
  NVML nvml;

  auto nvml_info = nvml.get_info();
  NVMLDeviceManager device_manager{nvml};

  const auto devices_begin = device_manager.devices_begin();
  const auto devices_end = device_manager.devices_end();

  //std::chrono::milliseconds timestamp;
  int size = 1;
  if (argc == 2)
    sscanf(argv[1], "%d", &size);
  int cnt = 0;
  float values[size];
  while (true)
  {
    for (auto device = devices_begin; device != devices_end; ++device)
    {
      (*device).refresh_metrics_or_halt();
      const auto &info = (*device).get_info();
      if (info.index == 0)
      {

        values[cnt] = info.metrics.gpu_utilization;
        cnt += 1;
      }
    }
    if (cnt >= size)
    {
      float avg = 0.0;
      for (int i = 0; i < size; ++i)
      {
        avg += values[i];
      }
      avg /= size;
      //timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
      std::cout << avg << std::endl;
      break;
    }

    std::this_thread::sleep_for(POLLING_PERIOD);
  }
}
