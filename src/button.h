#include "raylib.h"

#ifndef BUTTON_H
#define BUTTON_H

typedef enum ButtonState {
  BUTTON_IDLE,
  BUTTON_HOVERED,
  BUTTON_PRESSED,
  BUTTON_ACTIVATED,
} ButtonState;

ButtonState GetButtonState(Rectangle rect, MouseButton mouseButton);

#endif // !BUTTON_H

#ifdef BUTTON_IMPLEMENTATION
ButtonState GetButtonState(Rectangle button, MouseButton mouseButton) {
  ButtonState state;
  Vector2 mousePos = GetMousePosition();

  if (CheckCollisionPointRec(mousePos, button)) {
    state = BUTTON_HOVERED;
    if (IsMouseButtonDown(mouseButton)) {
      state = BUTTON_PRESSED;
    }
    if (IsMouseButtonReleased(mouseButton)) {
      state = BUTTON_ACTIVATED;
    }
  } else {
    state = BUTTON_IDLE;
  }

  return state;
}

#endif // !BUTTON_IMPLEMENTATION
