
#pragma once
#include <DDImage/Blink.h>
#include <Blink/Blink.h>

namespace utils {

  namespace nuke = DD::Image;

  /**
   * Wrapper for a #Blink::ComputeDevice that preferably selects the GPU
   * unless otherwise explicitly specified in the constructor.
   */
  class Device {
  private:
    bool _isGpu;
    Blink::ComputeDevice _device;
  public:

    /**
     * Initializes the #Device using the current GPU Device or the
     * CPU device if *useGpuIfAvailable* is #false. Always falls back
     * to the CPU device if the GPU is not available.
     */
    inline Device(bool useGpuIfAvailable=true) :
      _isGpu(useGpuIfAvailable),
      _device(useGpuIfAvailable ?
          Blink::ComputeDevice::CurrentGPUDevice() :
          Blink::ComputeDevice::CurrentCPUDevice())
    {
      if (useGpuIfAvailable && !_device.available()) {
        _isGpu = false;
        _device = Blink::ComputeDevice::CurrentCPUDevice();
      }
    }

    inline bool isGpu() const { return _isGpu; }
    inline bool isCpu() const { return !_isGpu; }

    inline operator Blink::ComputeDevice& () { return _device; }
    inline operator Blink::ComputeDevice const& () const { return _device; }
    inline Blink::ComputeDevice& device() { return _device; }
    inline Blink::ComputeDevice const& device() const { return _device; }
    inline Blink::ComputeDevice* operator -> () { return &_device; }
    inline Blink::ComputeDevice const* operator -> () const { return &_device; }
  };

  /**
   * Wrapper for a #Blink::Image that can be created from a #nuke::ImagePlane.
   * Always holds a version of the image that is available to the host. If the
   * device that the image is created for is a GPU, it also holds a separate
   * version of that image for the GPU device.
   *
   * The #hostImage() always returns the host image. The #deviceImage() returns
   * the host image on the CPU and the separate device image on the GPU.
   *
   * Use the #copyToHost() method to ensure that the device image is copied to
   * the host image in case of a GPU device.
   */
  class DeviceImage {
  private:
    bool _valid;
    bool _gpu;
    Blink::Image _hostImage;
    Blink::Image _deviceImage;

  public:

    inline DeviceImage(nuke::ImagePlane& plane, Device const& device, bool input) :
      _valid(false), _gpu(device.isGpu()), _hostImage(), _deviceImage()
    {
      _valid = nuke::Blink::ImagePlaneAsBlinkImage(plane, _hostImage);
      if (input) {
        _deviceImage = _hostImage.distributeTo(device);
      }
      if (!input && _gpu) {
        _deviceImage = _hostImage.makeLike(device);
      }
    }

    inline bool valid() const { return _valid; }
    inline bool isGpu() const { return _gpu; }
    inline bool isCpu() const { return !_gpu; }

    inline Blink::Image& hostImage() { return _hostImage; }
    inline Blink::Image const& hostImage() const { return _hostImage; }

    inline Blink::Image& deviceImage() { return _gpu ? _deviceImage : _hostImage; }
    inline Blink::Image const& deviceImage() const { return _gpu ? _deviceImage : _hostImage; }

    inline operator Blink::Image& () { return deviceImage(); }
    inline operator Blink::Image const& () const { return deviceImage(); }

    inline void copyToHost() {
      if (_gpu) {
        _hostImage.copyFrom(_deviceImage);
      }
    }
  };

} // namespace utils
