open Stdio;
open Core;
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

  let text = [color(Colors.white)];
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

  let%component make = (~debug=false, ~ctx: Context.t, ~onSubmit, ()) => {
    let%hook (url, setValue) = Hooks.state(ctx.state.apiUrl);

    printf("url: %s\n%!", url);

    <MyRow debug>
      <MyContainer debug>
        <Input
          style=Styles.input
          placeholder="API URL"
          value=url
          onChange={(value, _) => setValue(state => value)}
        />
      </MyContainer>
      <MyContainer debug style=Style.[flexGrow(0)]>
        <MyButton
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

let renderFeatureFlags =
    (~debug=false, ~graphQLConfig, ~ctx: Core.Context.t, featureFlags) => {
  module Styles = {
    open Style;

    let scrollview = [flexGrow(1)];
  };

  let featureFlagList =
    featureFlags
    |> ArrayLabels.map(~f=featureFlag => {
         <FeatureFlag debug graphQLConfig ctx featureFlag />
       })
    |> ArrayLabels.to_list
    |> React.listToElement;

  <MyContainer debug>
    <ScrollView style=Styles.scrollview bounce=false>
      featureFlagList
    </ScrollView>
  </MyContainer>;
};

let%component make = (~graphQLConfig, ~ctx: Core.Context.t, ()) => {
  module Styles = {
    open Style;

    let container = [flexGrow(1)];

    let text = [color(Colors.white)];
  };

  let Core.Context.{state, dispatch} = ctx;
  let debug = state.debug;
  let variables =
    `Assoc([("type_", `String("ALL")), ("state", `String("ALL"))]);
  // let%hook flags =
  //   Core.FeatureFlags.QueryHook.use(~config=graphQLConfig, ~variables, ());

  let%hook flags =
    ReveryGraphqlHooks.useQuery(
      ~config=graphQLConfig,
      ~definition=Core.FeatureFlags.Query.definition,
      ~onCompleted=
        result => {
          switch (result) {
          | Ok(data) => dispatch(UpdateFeatureFlags(data#featureFlags))
          | Error(_error) => Printf.printf("Mutation failed")
          }
        },
      ~variables,
      (),
    );

  printf("render FeatureFlagList\n%!");

  let featureFlags =
    switch (flags) {
    | Idle =>
      <Text
        style=Styles.text
        fontFamily=Constants.fontFamily
        fontSize=20.
        text="Nothing Loaded Yet"
      />
    | Loading =>
      <Text
        style=Styles.text
        fontFamily=Constants.fontFamily
        fontSize=20.
        text="Loading..."
      />
    | Error =>
      <Text
        style=Styles.text
        fontFamily=Constants.fontFamily
        fontSize=20.
        text={Printf.sprintf("Error: ")}
      />
    | Data(_query) =>
      renderFeatureFlags(~debug, ~graphQLConfig, ~ctx, state.featureFlags)
    };

  let reloadButtonAndTotal =
    <MyRow
      debug style=[Style.alignItems(`Center), Style.paddingVertical(10)]>
      <MyButton
        width=200
        fontSize=15.
        title="Reload Feature Flags"
        // onClick=loadFeatureFlags
      />
      <Text
        style=Styles.text
        fontFamily=Constants.fontFamily
        fontSize=20.
        text={Printf.sprintf("Total: %i", Array.length([||]))}
      />
      <MyRow debug style=Style.[alignSelf(`FlexEnd)]>
        <MyButton
          color=Colors.crimson
          width=100
          fontSize=15.
          title="Logout"
          onClick={() => dispatch(Logout)}
        />
        <MyButton
          width=200
          fontSize=15.
          title={ctx.state.debug ? "Disable Debug Mode" : "Enable Debug Mode"}
          onClick={() => dispatch(ToggleDebug)}
        />
      </MyRow>
    </MyRow>;

  <MyContainer debug>
    <ApiUrlInput debug ctx onSubmit={url => dispatch(ChangeApiUrl(url))} />
    <MyContainer debug> reloadButtonAndTotal featureFlags </MyContainer>
  </MyContainer>;
};
