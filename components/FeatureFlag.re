open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let button = [
    backgroundColor(Color.rgba(1., 1., 1., 0.1)),
    border(~width=2, ~color=Colors.white),
    margin(16),
  ];

  let text = [
    color(Colors.white),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20.),
  ];

  let inner = [
    flexDirection(`Row),
    alignItems(`Center),
    justifyContent(`SpaceBetween),
  ];

  let checkboxStyle = [
    width(25),
    height(25),
    border(~width=2, ~color=Colors.dodgerBlue),
  ];
};

let make = (~featureFlag: Core.FeatureFlags.featureFlags, ~toggle, ()) => {
  <View style=Styles.inner>
    <Text style=Styles.text text={featureFlag.name} />
    <Checkbox
      style=Styles.checkboxStyle
      checked={featureFlag.enabled}
      onChange={_ => toggle(featureFlag.name, featureFlag.enabled)}
    />
  </View>;
};
