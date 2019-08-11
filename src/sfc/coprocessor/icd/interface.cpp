auto ICD::ppuHreset() -> void {
  hcounter = 0;
  vcounter++;
  if((uint3)vcounter == 0) writeBank++;
}

auto ICD::ppuVreset() -> void {
  hcounter = 0;
  vcounter = 0;
}

auto ICD::ppuWrite(uint2 color) -> void {
  auto x = (uint8)hcounter++;
  auto y = (uint3)vcounter;
  if(x >= 160) return;  //unverified behavior

  uint11 address = writeBank * 512 + y * 2 + x / 8 * 16;
  output[address + 0] = (output[address + 0] << 1) | !!(color & 1);
  output[address + 1] = (output[address + 1] << 1) | !!(color & 2);
}

auto ICD::apuWrite(float left, float right) -> void {
  float samples[] = {left, right};
  stream->write(samples);
}

auto ICD::joypWrite(bool p14, bool p15) -> void {
  //joypad handling
  if(p14 == 1 && p15 == 1) {
    if(joyp14Lock == 0 && joyp15Lock == 0) {
      joyp14Lock = 1;
      joyp15Lock = 1;
      joypID++;
      if(mltReq == 0) joypID &= 0;  //1-player mode
      if(mltReq == 1) joypID &= 1;  //2-player mode
      if(mltReq == 2) joypID &= 3;  //4-player mode (unverified; but the most likely behavior)
      if(mltReq == 3) joypID &= 3;  //4-player mode
    }
  }

  uint8 joypad;
  if(joypID == 0) joypad = r6004;
  if(joypID == 1) joypad = r6005;
  if(joypID == 2) joypad = r6006;
  if(joypID == 3) joypad = r6007;

  uint4 input = 0xf;
  if(p14 == 1 && p15 == 1) input = 0xf - joypID;
  if(p14 == 0) input &= (joypad >> 0 & 15);  //d-pad
  if(p15 == 0) input &= (joypad >> 4 & 15);  //buttons

  GB_icd_set_joyp(&sameboy, input);

  if(p14 == 0 && p15 == 1) joyp14Lock = 0;
  if(p14 == 1 && p15 == 0) joyp15Lock = 0;

  //packet handling
  if(p14 == 0 && p15 == 0) {  //pulse
    pulseLock = false;
    packetOffset = 0;
    bitOffset = 0;
    strobeLock = true;
    packetLock = false;
    return;
  }

  if(pulseLock) return;

  if(p14 == 1 && p15 == 1) {
    strobeLock = false;
    return;
  }

  if(strobeLock) {
    if(p14 == 1 || p15 == 1) {  //malformed packet
      packetLock = false;
      pulseLock = true;
      bitOffset = 0;
      packetOffset = 0;
    } else {
      return;
    }
  }

  //p14:0, p15:1 = 0
  //p14:1, p15:0 = 1
  bool bit = p15 == 0;
  strobeLock = true;

  if(packetLock) {
    if(p14 == 0 && p15 == 1) {
      if((joypPacket[0] >> 3) == 0x11) {
        mltReq = joypPacket[1] & 3;
        joypID = 3;  //required: the next time P14==1 && P15==1; increment and start from ID=0 (Joypad 1)
      }

      if(packetSize < 64) packet[packetSize++] = joypPacket;
      packetLock = false;
      pulseLock = true;
    }
    return;
  }

  bitData = bit << 7 | bitData >> 1;
  if(++bitOffset) return;

  joypPacket[packetOffset] = bitData;
  if(++packetOffset) return;

  packetLock = true;
}
