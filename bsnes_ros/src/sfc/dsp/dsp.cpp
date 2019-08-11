#include <sfc/sfc.hpp>

namespace SuperFamicom {

DSP dsp;

#include "serialization.cpp"
#include "SPC_DSP.cpp"

auto DSP::main() -> void {
  if(!configuration.hacks.dsp.fast) {
    spc_dsp.run(1);
    clock += 2;
  } else {
    spc_dsp.run(32);
    clock += 2 * 32;
  }

  int count = spc_dsp.sample_count();
  if(count > 0) {
    for(uint n = 0; n < count; n += 2) {
      stream->sample(samplebuffer[n + 0] / 32768.0f, samplebuffer[n + 1] / 32768.0f);
    }
    spc_dsp.set_output(samplebuffer, 8192);
  }
}

auto DSP::read(uint8 address) -> uint8 {
  return spc_dsp.read(address);
}

auto DSP::write(uint8 address, uint8 data) -> void {
  spc_dsp.write(address, data);
}

auto DSP::load() -> bool {
  return true;
}

auto DSP::power(bool reset) -> void {
  clock = 0;
  stream = Emulator::audio.createStream(2, system.apuFrequency() / 768.0);

  if(!reset) {
    spc_dsp.init(apuram);
    spc_dsp.reset();
    spc_dsp.set_output(samplebuffer, 8192);
  } else {
    spc_dsp.soft_reset();
    spc_dsp.set_output(samplebuffer, 8192);
  }
}

auto DSP::mute() -> bool {
  return spc_dsp.mute();
}

}
