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
    printf("keycode: %i\tkeymod: %i\n%!", e.keycode, e.keymod);
    printf(
      "isLeftControlDown: %b\tisControlDown: %b\tisRightControlDown: %b\n%!",
      Sdl2.Keymod.isLeftControlDown(e.keymod),
      Sdl2.Keymod.isControlDown(e.keymod),
      Sdl2.Keymod.isRightControlDown(e.keymod),
    );
    printf(
      "isLeftGuiDown: %b\tisGuiDown: %b\tisRightGuiDown: %b\n%!",
      Sdl2.Keymod.isLeftGuiDown(e.keymod),
      Sdl2.Keymod.isGuiDown(e.keymod),
      Sdl2.Keymod.isRightGuiDown(e.keymod),
    );

    let paste = () => {
      printf("somebody tried to paste\n%!");
      Sdl2.Clipboard.getText()
      |> Base.Option.iter(~f=text => onChange(text, 0));
    };

    switch (e) {
    | e when e.keycode === 118 && Sdl2.Keymod.isGuiDown(e.keymod) => paste()
    | e when e.keycode === 118 && Sdl2.Keymod.isControlDown(e.keymod) =>
      paste()
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
