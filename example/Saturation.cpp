/**
 * Copyright (c) 2018  Niklas Rosenstein
 * Developed for AixSponza GmbH.
 */

#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4267)  // conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable: 4512)  // assignment operator could not be generated

#include <DDImage/PlanarIop.h>
#include <DDImage/Knobs.h>
#include <DDImage/NukeWrapper.h>
#include <DDImage/Blink.h>
#include <Blink/Blink.h>
#include <string>
#include <sstream>
#include "./utils.hpp"

#pragma warning(disable: 4481)  // nonstandard extension used: override specifier 'override'

namespace nuke = DD::Image;

static const char* const HELP = "";
static const char* const CLASS = "AixSponza_Saturation";
static const char* const GROUP = "AixSponza/Saturation";

extern "C" unsigned char SaturationKernel[];
extern "C" size_t SaturationKernel_size;


class SaturationIop : public nuke::PlanarIop {
protected:
  utils::Device _device;
  Blink::ProgramSource _kernelProgram;

private:

  void renderStripeInternal(nuke::ImagePlane& outputPlane) {
    // Bounding-box for our input plane.
    nuke::Box inputBox = outputPlane.bounds();
    inputBox.intersect(input0().info());

    // Create and fetch the image plane for the first input,
    // then create a Blink device image from it.
    nuke::ImagePlane inputPlane(inputBox, outputPlane.packed(), outputPlane.channels(), outputPlane.nComps());
    input0().fetchPlane(inputPlane);
    utils::DeviceImage inputImage(inputPlane, _device, true);

    if (aborted()) return;

    // Make the output plane writable and create a Blink device image.
    outputPlane.makeWritable();
    utils::DeviceImage outputImage(outputPlane, _device, false);

    // Check if successfully created the device images.
    if (!inputImage.valid() || !outputImage.valid()) {
      error("Could not fetch Blink image for input/output plane.");
      return;
    }

    // Bind the compute device to the current thread.
    Blink::ComputeDeviceBinder binder(_device);

    // Input/output images for the kernel.
    std::vector<Blink::Image> images;
    images.push_back(inputImage);
    images.push_back(outputImage);

    Blink::Kernel kernel(_kernelProgram, _device, images, kBlinkCodegenDefault);
    kernel.setParamValue("Saturation", 2.0f);
    kernel.iterate();

    outputImage.copyToHost();
  }

public:

  SaturationIop(Node* node) :
      nuke::PlanarIop(node),
      _device(true),
      _kernelProgram(std::string((char*)SaturationKernel, SaturationKernel_size))
  { }

  // nuke::PlanarIop Overrides

  virtual void renderStripe(nuke::ImagePlane& outputPlane) override {
    try {
      renderStripeInternal(outputPlane);
    }
    catch (...) {
      std::string message;
      try { throw; }
      catch (std::exception e) {
        message = e.what();
      }
      catch (Blink::ParseException e) {
        std::ostringstream s;
        s << e.userMessage() << ": " << e.parseError() << "(at line " << e.lineNumber() << ")";
        message = s.str();
      }
      catch (Blink::Exception e) {
        message = e.userMessage();
      }
      catch (...) {
        message = "Unknown error";
      }
      std::cout << "[" << CLASS << "]: ERROR: " << message << std::endl;
      error(message.c_str());
    }
  }

  // nuke::Iop Overrides

  virtual const char* Class() const override {
    return CLASS;
  }

  virtual const char* node_help() const override {
    return HELP;
  }

  virtual void in_channels(int, nuke::ChannelSet& mask) const override {
    // No changes to *mask* -- indicates that channels only depend on themselves.
  }

  virtual void knobs(nuke::Knob_Callback f) override {
    // Show information about the Compute Device.
    Newline(f, "Compute Device:");
    Named_Text_knob(f, "computeDeviceName", _device->name().c_str());
    Newline(f);
    Divider(f);
  }

  virtual void _validate(bool for_real) override {
    // Copy bbox channels etc from input0, which will validate it.
    copy_info();
  }

  // nuke::Op Overrides

  virtual const char* input_label(int i, char* buffer) const override {
    switch (i) {
      case 0: return "in";
      default: return "??";
    }
  }

};

static nuke::Iop* build(Node* node) {
  return (new nuke::NukeWrapper(new SaturationIop(node)))->channelsRGBoptionalAlpha();
}

static const nuke::Iop::Description description(CLASS, GROUP, build);
