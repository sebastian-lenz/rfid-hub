#include "ui/ScreenEvent.h"
#include "ui/SettingsScreen.h"
#include "ui/Sprites.h"
#include "globals.h"

class Option {
private:
  const String _title;

public:
  Option(const String title)
  : _title(title) { }

  virtual uint8_t* getIcon() {
    return sprites::edit;
  };

  virtual void onButtonPress() { };

  virtual void reset() { };

  virtual void render() {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawUTF8(0, 11, _title.c_str());
  }
};


class ChoiceOption : public Option {
private:
  const std::vector<String> _choices;

public:
  ChoiceOption(const String title, const std::vector<String> choices)
  : Option(title), _choices(choices) { }

  virtual int getCurrentIndex() {
    return -1;
  };

  void render() override {
    Option::render();

    int currentIndex = getCurrentIndex();
    u8g2.setFont(u8g2_font_helvR08_tf);

    for (int index = 0; index < _choices.size(); index++) {
      int y = 27 + index * 16;
      u8g2.drawUTF8(13, y, _choices[index].c_str());
      u8g2.drawXBM(0, y - 10, 11, 11, index == currentIndex ? sprites::radioOn : sprites::radioOff);
    }
  }
};


class ModeOption : public ChoiceOption {
public:
  ModeOption()
  : ChoiceOption("Ausgabemodus", {"Tasten halten", "Tasten tippen", "Nachricht"}) { }

  int getCurrentIndex() override {
    return static_cast<int>(settings.getKeyboardMode());
  };

  void onButtonPress() override {
    KeyboardMode mode = settings.getKeyboardMode();
    settings.setKeyboardMode(
      static_cast<KeyboardMode>((static_cast<int>(mode) + 1) % 4)
    );
  };
};


class SensorOption : public ChoiceOption {
public:
  SensorOption()
  : ChoiceOption("Zuordnung Sensoren", {"Individuell", "Gemeinsam"}) { }

  int getCurrentIndex() override {
    return settings.useIndividualSensors() ? 0 : 1;
  };

  void onButtonPress() override {
    settings.useIndividualSensors(!settings.useIndividualSensors());
  };
};


class ResetOption : public Option {
private:
  bool _didReset = false;

public:
  ResetOption()
  : Option("Zurücksetzen") { }

  uint8_t* getIcon() override {
    return sprites::trash;
  };

  void render() override {
    Option::render();
    u8g2.setFont(u8g2_font_helvR08_tf);
    if (_didReset) {
      u8g2.drawXBM(0, 17, 11, 11, sprites::info);
      u8g2.drawUTF8(13, 27, "Einstellungen");
      u8g2.drawUTF8(0, 43, "zurückgesetzt");
    } else {
      u8g2.drawUTF8(0, 27, "Alle Einstellungen");
      u8g2.drawUTF8(0, 43, "auf Standard zurück-");
      u8g2.drawUTF8(0, 59, "setzen?");
    }
  }

  void reset() override {
    _didReset = false;
  };

  void onButtonPress() override {
    if (_didReset) return;
    settings.reset();
    _didReset = true;
  };
};


Option* options[] = {
  new ModeOption(),
  new SensorOption(),
  new ResetOption(),
};


void SettingsScreen::render() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  auto currentOption = options[_currentSetting];
  renderActions(Label(sprites::more), Label(currentOption->getIcon()));
  currentOption->render();

  u8g2.sendBuffer();
}

void SettingsScreen::onButton(ButtonSignal signal, int index) {
  if (signal != ButtonSignal::PRESS) {
    return;
  }

  if (index == 0) {
    _currentSetting += 1;
    if (_currentSetting >= 3) {
      dispatcher.trigger(ScreenEvent(ScreenType::Home));
    } else {
      options[_currentSetting]->reset();
    }
  } else {
    options[_currentSetting]->onButtonPress();
  }
}
