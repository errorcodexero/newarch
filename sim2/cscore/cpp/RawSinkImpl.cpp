/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RawSinkImpl.h"

#include "Instance.h"
#include "cscore.h"
#include "cscore_raw.h"

using namespace cs;

RawSinkImpl::RawSinkImpl(const wpi::Twine& name, wpi::Logger& logger,
                         Notifier& notifier, Telemetry& telemetry)
    : SinkImpl{name, logger, notifier, telemetry} {
  m_active = true;
  // m_thread = std::thread(&RawSinkImpl::ThreadMain, this);
}

RawSinkImpl::RawSinkImpl(const wpi::Twine& name, wpi::Logger& logger,
                         Notifier& notifier, Telemetry& telemetry,
                         std::function<void(uint64_t time)> processFrame)
    : SinkImpl{name, logger, notifier, telemetry} {}

RawSinkImpl::~RawSinkImpl() { Stop(); }

void RawSinkImpl::Stop() {
  m_active = false;

  // wake up any waiters by forcing an empty frame to be sent
  if (auto source = GetSource()) source->Wakeup();

  // join thread
  if (m_thread.joinable()) m_thread.join();
}

uint64_t RawSinkImpl::GrabFrame(CS_RawFrame& image) {
  SetEnabled(true);

  auto source = GetSource();
  if (!source) {
    // Source disconnected; sleep for one second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
  }

  auto frame = source->GetNextFrame();  // blocks
  if (!frame) {
    // Bad frame; sleep for 20 ms so we don't consume all processor time.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return 0;  // signal error
  }

  return GrabFrameImpl(image, frame);
}

uint64_t RawSinkImpl::GrabFrame(CS_RawFrame& image, double timeout) {
  SetEnabled(true);

  auto source = GetSource();
  if (!source) {
    // Source disconnected; sleep for one second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
  }

  auto frame = source->GetNextFrame(timeout);  // blocks
  if (!frame) {
    // Bad frame; sleep for 20 ms so we don't consume all processor time.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return 0;  // signal error
  }

  return GrabFrameImpl(image, frame);
}

uint64_t RawSinkImpl::GrabFrameImpl(CS_RawFrame& rawFrame,
                                    Frame& incomingFrame) {
  Image* newImage = nullptr;

  if (rawFrame.pixelFormat == CS_PixelFormat::CS_PIXFMT_UNKNOWN) {
    // Always get incoming image directly on unknown
    newImage = incomingFrame.GetExistingImage(0);
  } else {
    // Format is known, ask for it
    auto width = rawFrame.width;
    auto height = rawFrame.height;
    auto pixelFormat =
        static_cast<VideoMode::PixelFormat>(rawFrame.pixelFormat);
    if (width <= 0 || height <= 0) {
      width = incomingFrame.GetOriginalWidth();
      height = incomingFrame.GetOriginalHeight();
    }
    newImage = incomingFrame.GetImage(width, height, pixelFormat);
  }

  if (!newImage) {
    // Shouldn't happen, but just in case...
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return 0;
  }

  CS_AllocateRawFrameData(&rawFrame, newImage->size());
  rawFrame.height = newImage->height;
  rawFrame.width = newImage->width;
  rawFrame.pixelFormat = newImage->pixelFormat;
  rawFrame.totalData = newImage->size();
  std::copy(newImage->data(), newImage->data() + rawFrame.totalData,
            rawFrame.data);

  return incomingFrame.GetTime();
}

// Send HTTP response and a stream of JPG-frames
void RawSinkImpl::ThreadMain() {
  Enable();
  while (m_active) {
    auto source = GetSource();
    if (!source) {
      // Source disconnected; sleep for one second
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }
    SDEBUG4("waiting for frame");
    Frame frame = source->GetNextFrame();  // blocks
    if (!m_active) break;
    if (!frame) {
      // Bad frame; sleep for 10 ms so we don't consume all processor time.
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }
    // TODO m_processFrame();
  }
  Disable();
}

namespace cs {
CS_Sink CreateRawSink(const wpi::Twine& name, CS_Status* status) {
  auto& inst = Instance::GetInstance();
  return inst.CreateSink(CS_SINK_RAW,
                         std::make_shared<RawSinkImpl>(
                             name, inst.logger, inst.notifier, inst.telemetry));
}

CS_Sink CreateRawSinkCallback(const wpi::Twine& name,
                              std::function<void(uint64_t time)> processFrame,
                              CS_Status* status) {
  auto& inst = Instance::GetInstance();
  return inst.CreateSink(CS_SINK_RAW, std::make_shared<RawSinkImpl>(
                                          name, inst.logger, inst.notifier,
                                          inst.telemetry, processFrame));
}

uint64_t GrabSinkFrame(CS_Sink sink, CS_RawFrame& image, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data || data->kind != CS_SINK_RAW) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return static_cast<RawSinkImpl&>(*data->sink).GrabFrame(image);
}

uint64_t GrabSinkFrameTimeout(CS_Sink sink, CS_RawFrame& image, double timeout,
                              CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data || data->kind != CS_SINK_RAW) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return static_cast<RawSinkImpl&>(*data->sink).GrabFrame(image, timeout);
}
}  // namespace cs

extern "C" {
CS_Sink CS_CreateRawSink(const char* name, CS_Status* status) {
  return cs::CreateRawSink(name, status);
}

CS_Sink CS_CreateRawSinkCallback(const char* name, void* data,
                                 void (*processFrame)(void* data,
                                                      uint64_t time),
                                 CS_Status* status) {
  return cs::CreateRawSinkCallback(
      name, [=](uint64_t time) { processFrame(data, time); }, status);
}

uint64_t CS_GrabRawSinkFrame(CS_Sink sink, struct CS_RawFrame* image,
                             CS_Status* status) {
  return cs::GrabSinkFrame(sink, *image, status);
}

uint64_t CS_GrabRawSinkFrameTimeout(CS_Sink sink, struct CS_RawFrame* image,
                                    double timeout, CS_Status* status) {
  return cs::GrabSinkFrameTimeout(sink, *image, timeout, status);
}
}  // extern "C"
