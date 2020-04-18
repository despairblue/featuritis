open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let row = [height(50), alignItems(`Center)];

  let text = [
    color(Colors.white),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20.),
  ];

  let checkboxStyle = [
    width(25),
    height(25),
    border(~width=2, ~color=Colors.dodgerBlue),
  ];
};

let make = (~featureFlag: Core.FeatureFlags.featureFlags, ~toggle, ()) => {
  <MyRow style=Styles.row>
    <Text style=Styles.text text={featureFlag.name} />
    <Checkbox
      style=Styles.checkboxStyle
      checked={featureFlag.enabled}
      onChange={_ => toggle(featureFlag.name, featureFlag.enabled)}
    />
  </MyRow>;
};
