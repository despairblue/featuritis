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

    let input = [
      width(500),
      color(Colors.white),
      border(~width=1, ~color=Colors.white),
    ];
    // TODO: fork Button
    let button = [];
  };

  let%component make = (~ctx: Context.t, ~onSubmit, ()) => {
    let%hook (url, setValue) = Hooks.state(ctx.state.apiUrl);

    printf("url: %s\n%!", url);

    <MyRow>
      <MyContainer>
        <Input2
          style=Styles.input
          placeholder="API URL"
          value=url
          onChange={(value, _) => setValue(state => value)}
        />
      </MyContainer>
      <MyContainer style=Style.[flexGrow(0)]>
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
      </MyContainer>
    </MyRow>;
  };
};

let renderFeatureFlags = (featureFlags, toggle) => {
  module Styles = {
    open Style;

    let scrollview = [flexGrow(1)];
  };

  let featureFlagList =
    featureFlags
    |> ArrayLabels.map(~f=featureFlag => {<FeatureFlag featureFlag toggle />})
    |> ArrayLabels.to_list
    |> React.listToElement;

  <MyContainer>
    <ScrollView style=Styles.scrollview bounce=false>
      featureFlagList
    </ScrollView>
  </MyContainer>;
};

let%component make = (~graphQLConfig, ~ctx: Core.Context.t, ()) => {
  module Styles = {
    open Style;

    let container = [flexGrow(1)];

    let text = [
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(20.),
    ];
  };

  let Core.Context.{state, dispatch} = ctx;
  let variables =
    `Assoc([("type_", `String("ALL")), ("state", `String("ALL"))]);
  // let%hook flags =
  //   Core.FeatureFlags.QueryHook.use(~config=graphQLConfig, ~variables, ());

  let%hook flags =
    Core.FeatureFlags.Graphql.newQuery(
      ~config=graphQLConfig,
      ~definition=Core.FeatureFlags.Query.definition,
      ~variables,
      (),
    );

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
    | Data(query) => renderFeatureFlags(query#featureFlags, toggle)
    };

  let reloadButtonAndTotal =
    <MyRow style=[Style.alignItems(`Center), Style.paddingVertical(10)]>
      <Button
        height=50
        width=200
        fontSize=15.
        title="Reload Feature Flags"
        onClick=loadFeatureFlags
      />
      <Text
        style=Styles.text
        text={Printf.sprintf("Total: %i", Array.length([||]))}
      />
    </MyRow>;

  <MyContainer>
    <ApiUrlInput ctx onSubmit={url => dispatch(ChangeApiUrl(url))} />
    <MyContainer> reloadButtonAndTotal featureFlags </MyContainer>
  </MyContainer>;
};
