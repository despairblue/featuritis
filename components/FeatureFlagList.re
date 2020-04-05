open Stdio;
open Revery;
open Revery.UI;
open Revery.UI.Components;

module State = Core.State;

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
};

module ApiUrlInput = {
  open Core;

  let%component make = (~ctx: Context.t, ~onSubmit, ()) => {
    let%hook (url, setValue) = Hooks.state(ctx.state.apiUrl);

    printf("url: %s\n%!", url);

    <View style=[]>
      <Input2
        style=[]
        placeholder="API URL"
        value=url
        onChange={(value, _) => setValue(state => value)}
      />
      <Button
        height=50
        width=100
        fontSize=15.
        title="Submit"
        onClick={() => {
          printf("onClick\n%!");
          onSubmit(url);
        }}
      />
    </View>;
  };
};

let make = (~authToken, ~ctx: Core.Context.t, ()) => {
  let Core.Context.{state, dispatch} = ctx;

  let loadFeatureFlags = () => {
    dispatch(LoadFeatureFlags(dispatch));
  };
  let toggle = (name, enabled) =>
    if (enabled) {
      dispatch(DisableFeatureFlag(dispatch, name));
    } else {
      dispatch(EnableFeatureFlag(dispatch, name));
    };

  let featureFlags =
    switch (state.featureFlags) {
    | Idle => <Text style=Styles.text text="Nothing Loaded Yet" />
    | Loading => <Text style=Styles.text text="Loading..." />
    | Error(error) =>
      <Text style=Styles.text text={Printf.sprintf("Error: %s", error)} />
    | Data(featureFlags) =>
      <View>
        <Padding padding=4>
          <Text
            style=Styles.text
            text={Printf.sprintf("Total: %i", Array.length(featureFlags))}
          />
        </Padding>
        {featureFlags
         |> ArrayLabels.map(~f=featureFlag => {
              <FeatureFlag featureFlag toggle />
            })
         |> ArrayLabels.to_list
         |> React.listToElement}
      </View>
    };

  <View>
    <ApiUrlInput ctx onSubmit={url => dispatch(ChangeApiUrl(url))} />
    <Button
      height=50
      width=200
      fontSize=15.
      title="Load Feature Flags"
      onClick=loadFeatureFlags
    />
    featureFlags
  </View>;
};
