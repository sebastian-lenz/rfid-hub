#include "ui/EmptyScreen.h"
#include "ui/HomeScreen.h"
#include "ui/SettingsScreen.h"
#include "ui/UI.h"
#include "globals.h"

#define FRAMES_DISPLAY_DIMMED 100
#define FRAMES_DISPLAY_OFF 400

UI::UI() {
  _screen = new HomeScreen();

  dispatcher.subscribe(ScreenEvent::descriptor,
    std::bind(&UI::onScreenEvent, this, std::placeholders::_1)
  );
}

void UI::onLoop() {
  for (int index = 0; index < NUM_BUTTONS; index++) {
    Button* button = &buttons[index];
    ButtonSignal signal = button->update();

    if (signal != ButtonSignal::NONE) {
      if (_lastButtonPress > FRAMES_DISPLAY_DIMMED) {
        u8g2.setContrast(100);
      }

      _lastButtonPress = 0;
      _screen->onButton(signal, index);
    }
  }

  _lastButtonPress += 1;
  if (_lastButtonPress == FRAMES_DISPLAY_DIMMED) {
    u8g2.setContrast(1);
  } else if (_lastButtonPress == FRAMES_DISPLAY_OFF) {
    setScreen(ScreenType::Empty);
  }

  _screen->update();
  _screen->render();
};

void UI::onSetup() {
  u8g2.begin();

  for (auto& button : buttons) {
    button.begin();
  }
};

void UI::onScreenEvent(const Event& event) {
  if (event.type() != ScreenEvent::descriptor) return;
  const auto& screenEvent = static_cast<const ScreenEvent&>(event);

  setScreen(screenEvent.screenType);
}

void UI::setScreen(ScreenType type) {
  delete _screen;

  switch (type) {
    case ScreenType::Empty:
      _screen = new EmptyScreen();
      break;
    case ScreenType::Home:
      _screen = new HomeScreen();
      break;
    case ScreenType::Settings:
      _screen = new SettingsScreen();
      break;
  }
}
