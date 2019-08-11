auto Program::hackCompatibility() -> void {
  bool fastPPU = emulatorSettings.fastPPU.checked();
  bool fastPPUNoSpriteLimit = emulatorSettings.noSpriteLimit.checked();
  bool fastDSP = emulatorSettings.fastDSP.checked();
  bool coprocessorDelayedSync = emulatorSettings.coprocessorDelayedSyncOption.checked();

  auto title = superFamicom.title;
  if(title == "AIR STRIKE PATROL" || title == "DESERT FIGHTER") fastPPU = false;
  if(title == "KOUSHIEN_2") fastDSP = false;
  if(title == "RENDERING RANGER R2") fastDSP = false;

  emulator->configure("Hacks/PPU/Fast", fastPPU);
  emulator->configure("Hacks/PPU/NoSpriteLimit", fastPPUNoSpriteLimit);
  emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
  emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  emulator->configure("Hacks/DSP/Fast", fastDSP);
  emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorDelayedSync);
}

auto Program::hackPatchMemory(vector<uint8_t>& data) -> void {
  auto title = superFamicom.title;

  if(title == "Satellaview BS-X" && data.size() >= 0x100000) {
    //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN) (1.1)
    //disable limited play check for BS Memory flash cartridges
    //benefit: allow locked out BS Memory flash games to play without manual header patching
    //detriment: BS Memory ROM cartridges will cause the game to hang in the load menu
    if(data[0x4a9b] == 0x10) data[0x4a9b] = 0x80;
    if(data[0x4d6d] == 0x10) data[0x4d6d] = 0x80;
    if(data[0x4ded] == 0x10) data[0x4ded] = 0x80;
    if(data[0x4e9a] == 0x10) data[0x4e9a] = 0x80;
  }
}
