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

  module Styles = {
    open Style;

    let container = [flexDirection(`Row), justifyContent(`SpaceBetween)];
  };

  let%component make = (~ctx: Context.t, ~onSubmit, ()) => {
    let%hook (url, setValue) = Hooks.state(ctx.state.apiUrl);

    printf("url: %s\n%!", url);

    <View style=Styles.container>
      <Input2
        style=Style.[
          flexGrow(2),
          width(500),
          color(Colors.white),
          border(~width=1, ~color=Colors.white),
        ]
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

let%component make = (~graphQLConfig, ~ctx: Core.Context.t, ()) => {
  let Core.Context.{state, dispatch} = ctx;
  let variables =
    `Assoc([("type_", `String("ALL")), ("state", `String("ALL"))]);
  let%hook flags =
    Core.FeatureFlags.QueryHook.use(~config=graphQLConfig, ~variables, ());

  printf("render FeatureFlagList\n%!");

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
    switch (flags) {
    | Idle => <Text style=Styles.text text="Nothing Loaded Yet" />
    | Loading => <Text style=Styles.text text="Loading..." />
    | Error => <Text style=Styles.text text={Printf.sprintf("Error: ")} />
    | Data(query) =>
      let featureFlags = query#featureFlags;
      <ScrollView style=[] bounce=true>
        <Text
          style=Styles.text
          text={Printf.sprintf("Total: %i", Array.length(featureFlags))}
        />
        {featureFlags
         |> ArrayLabels.map(~f=featureFlag => {
              <FeatureFlag featureFlag toggle />
            })
         |> ArrayLabels.to_list
         |> React.listToElement}
      </ScrollView>;
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
