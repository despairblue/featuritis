open Stdio;
open Revery;
open Revery.UI;
open Revery.UI.Components;

let defaultStyle = Style.[flexGrow(1)];
let debugStyle =
  Style.[
    padding(20),
    backgroundColor(Colors.white),
    border(~width=2, ~color=Colors.black),
  ];

let make =
    (
      ~children=React.empty,
      ~debug=false,
      ~style: list(Style.viewStyleProps)=Style.emptyViewStyle,
      (),
    ) =>
  if (debug) {
    <View style={List.concat([defaultStyle, style, debugStyle])}>
      children
    </View>;
  } else {
    <View style={List.concat([defaultStyle, style])}> children </View>;
  };
