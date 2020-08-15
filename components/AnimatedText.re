open Core;
open Revery;
open Revery.UI;
open Revery.UI.Components;

module AnimatedText = {
  module Styles = {
    open Style;

    let text = (~yOffset) => [
      color(Colors.white),
      transform([Transform.TranslateY(yOffset)]),
    ];
  };

  let%component make = (~delay: Time.t, ~text: string, ()) => {
    let%hook (yOffset, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.ms(500))
        |> Animation.delay(delay)
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(50., 0.),
      );

    let%hook (animatedOpacity, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.seconds(1))
        |> Animation.delay(delay)
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(0., 1.),
      );

    <Opacity opacity=animatedOpacity>
      <Padding padding=8>
        <Text
          style={Styles.text(~yOffset)}
          fontFamily=Constants.fontFamily
          fontSize=24.
          text
        />
      </Padding>
    </Opacity>;
  };
};
