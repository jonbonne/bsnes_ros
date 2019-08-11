#include "bsnes.hpp"
#include <sfc/interface/interface.hpp>
Video video;
Audio audio;
Input input;
unique_pointer<Emulator::Interface> emulator;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::userData(), "bsnes/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::userSettings(), "bsnes/"});
  return {Path::userSettings(), "bsnes/", name};
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  settings.location = locate("settings.bml");

  for(auto argument : arguments) {
    if(argument == "--fullscreen") {
      program.startFullScreen = true;
    } else if(argument.beginsWith("--locale=")) {
      Application::locale().scan(locate("Locale/"));
      Application::locale().select(argument.trimLeft("--locale=", 1L));
    } else if(argument.beginsWith("--settings=")) {
      settings.location = argument.trimLeft("--settings=", 1L);
    } else if(inode::exists(argument)) {
      //game without option
      program.gameQueue.append({"Auto;", argument});
    } else if(argument.find(";")) {
      //game with option
      auto game = argument.split(";", 1L);
      if(inode::exists(game.last())) program.gameQueue.append(argument);
    }
  }

  settings.load();
  Application::setName("bsnes");
  Application::setScreenSaver(settings.general.screenSaver);
  Application::setToolTips(settings.general.toolTips);

  Instances::presentation.construct();
  Instances::settingsWindow.construct();
  Instances::cheatDatabase.construct();
  Instances::cheatWindow.construct();
  Instances::stateWindow.construct();
  Instances::toolsWindow.construct();
  emulator = new SuperFamicom::Interface;
  program.create();

  if(Emulator::Version.find(".") && settings.general.betaWarning) {
    MessageDialog dialog;
    dialog.setTitle(Emulator::Name);
    dialog.setText(
      "This is a nightly release. Bugs and regressions are possible!\n"
      "If you experience issues, please report them to me.\n"
      "If stability is required, please use a stable release.\n"
    );
    dialog.setOption("Don't show this message again");
    dialog.information();
    if(dialog.checked()) {
      settings.general.betaWarning = false;
      settings.save();
    }
  }

  Application::run();
  Instances::presentation.destruct();
  Instances::settingsWindow.destruct();
  Instances::cheatDatabase.destruct();
  Instances::cheatWindow.destruct();
  Instances::stateWindow.destruct();
  Instances::toolsWindow.destruct();
}
