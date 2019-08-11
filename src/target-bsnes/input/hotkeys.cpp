auto InputHotkey::logic() const -> Logic {
  return inputManager.hotkeyLogic;
}

//

auto InputManager::bindHotkeys() -> void {
  static int stateSlot = 1;
  static double frequency = 48000.0;
  static double volume = 0.0;
  static bool fastForwarding = false;
  static bool rewinding = false;

  hotkeys.append(InputHotkey("Toggle Fullscreen Mode").onPress([] {
    presentation.toggleFullscreenMode();
  }));

  hotkeys.append(InputHotkey("Toggle Mouse Capture").onPress([] {
    input.acquired() ? input.release() : input.acquire();
  }));

  hotkeys.append(InputHotkey("Toggle Cheat Codes").onPress([] {
    cheatEditor.enableCheats.setChecked(!cheatEditor.enableCheats.checked()).doToggle();
  }));

  hotkeys.append(InputHotkey("Rewind").onPress([&] {
    if(!emulator->loaded() || fastForwarding) return;
    rewinding = true;
    if(program.rewind.frequency == 0) {
      program.showMessage("Please enable rewind support in Settings->Emulator first");
    } else {
      program.rewindMode(Program::Rewind::Mode::Rewinding);
    }
    volume = Emulator::audio.volume();
    if(settings.rewind.mute) {
      program.mute |= Program::Mute::Rewind;
    } else {
      Emulator::audio.setVolume(volume * 0.65);
    }
  }).onRelease([&] {
    rewinding = false;
    if(!emulator->loaded()) return;
    program.rewindMode(Program::Rewind::Mode::Playing);
    program.mute &= ~Program::Mute::Rewind;
    Emulator::audio.setVolume(volume);
  }));

  hotkeys.append(InputHotkey("Save State").onPress([&] {
    program.saveState({"Quick/Slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Load State").onPress([&] {
    program.loadState({"Quick/Slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Load Undo State").onPress([&] {
    program.loadState("Quick/Undo");
  }));

  hotkeys.append(InputHotkey("Load Redo State").onPress([&] {
    program.loadState("Quick/Redo");
  }));

  hotkeys.append(InputHotkey("Increment State Slot").onPress([&] {
    if(--stateSlot < 1) stateSlot = 9;
    program.showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Decrement State Slot").onPress([&] {
    if(++stateSlot > 9) stateSlot = 1;
    program.showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Capture Screenshot").onPress([] {
    program.captureScreenshot();
  }));

  hotkeys.append(InputHotkey("Fast Forward").onPress([] {
    if(!emulator->loaded() || rewinding) return;
    fastForwarding = true;
    emulator->setFrameSkip(emulator->configuration("Hacks/PPU/Fast") == "true" ? settings.fastForward.frameSkip : 0);
    video.setBlocking(false);
    audio.setBlocking(settings.fastForward.limiter != 0);
    audio.setDynamic(false);
    frequency = Emulator::audio.frequency();
    volume = Emulator::audio.volume();
    if(settings.fastForward.limiter) {
      Emulator::audio.setFrequency(frequency / settings.fastForward.limiter);
    }
    if(settings.fastForward.mute) {
      program.mute |= Program::Mute::FastForward;
    } else if(settings.fastForward.limiter) {
      Emulator::audio.setVolume(volume * 0.65);
    }
  }).onRelease([] {
    fastForwarding = false;
    if(!emulator->loaded()) return;
    emulator->setFrameSkip(0);
    video.setBlocking(settings.video.blocking);
    audio.setBlocking(settings.audio.blocking);
    audio.setDynamic(settings.audio.dynamic);
    if(settings.fastForward.limiter) {
      Emulator::audio.setFrequency(frequency);
    }
    program.mute &= ~Program::Mute::FastForward;
    Emulator::audio.setVolume(volume);
  }));

  hotkeys.append(InputHotkey("Pause Emulation").onPress([] {
    presentation.pauseEmulation.setChecked(!presentation.pauseEmulation.checked());
  }));

  hotkeys.append(InputHotkey("Frame Advance").onPress([] {
    presentation.frameAdvance.doActivate();
  }));

  hotkeys.append(InputHotkey("Reset Emulation").onPress([] {
    program.reset();
  }));

  hotkeys.append(InputHotkey("Quit Emulator").onPress([] {
    program.quit();
  }));

  for(auto& hotkey : hotkeys) {
    hotkey.path = string{"Hotkey/", hotkey.name}.replace(" ", "");
    auto assignments = settings(hotkey.path).text().split(";");
    for(uint index : range(BindingLimit)) hotkey.assignments[index] = assignments(index);
    hotkey.bind();
  }
}

auto InputManager::pollHotkeys() -> void {
  if(Application::modal() || !program.focused()) return;

  for(auto& hotkey : hotkeys) {
    auto state = hotkey.poll();
    if(hotkey.state == 0 && state == 1 && hotkey.press) hotkey.press();
    if(hotkey.state == 1 && state == 0 && hotkey.release) hotkey.release();
    hotkey.state = state;
  }
}
