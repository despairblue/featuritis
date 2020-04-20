open Stdio;
open Revery;
open Revery.UI;
open Revery.UI.Components;
open Input;

let make =
    (
      ~style=Styles.default,
      ~placeholderColor=Styles.defaultPlaceholderColor,
      ~cursorColor=Styles.defaultCursorColor,
      ~autofocus=false,
      ~placeholder="",
      ~onFocus=() => (),
      ~onBlur=() => (),
      ~onKeyDown=_ => (),
      ~onChange=(_, _) => (),
      ~value=?,
      ~cursorPosition=?,
      (),
    ) => {
  let ownOnKeyDown = (e: NodeEvents.keyEventParams) => {
    printf("keycode: %i\n%!", e.keycode);

    switch (e.keycode) {
    | v when v === 118 && e.ctrlKey =>
      printf("somebody tried to paste\n%!");
      Sdl2.Clipboard.getText()
      |> Base.Option.iter(~f=text => onChange(text, 0));
    | _ => ()
    };

    onKeyDown(e);
  };

  <Input
    style
    placeholderColor
    cursorColor
    autofocus
    placeholder
    onFocus
    onBlur
    onKeyDown=ownOnKeyDown
    onChange
    ?value
    ?cursorPosition
  />;
};
