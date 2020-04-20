open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let row = [alignItems(`Center)];

  let text = [
    color(Colors.white),
    fontFamily("Poppins-Regular.ttf"),
    fontSize(20.),
  ];

  let checkboxStyle = [
    width(25),
    height(25),
    border(~width=2, ~color=Colors.dodgerBlue),
  ];
};

let%component make =
              (
                ~debug=false,
                ~graphQLConfig,
                ~ctx: Core.Context.t,
                ~featureFlag: Core.FeatureFlags.featureFlags,
                (),
              ) => {
  let Core.Context.{dispatch} = ctx;

  let%hook (enableFeatureFlag, _state) =
    ReveryGraphqlHooks.useMutation(
      ~config=graphQLConfig,
      ~definition=Core.FeatureFlags.EnableFeatureFlag.definition,
      (),
    );

  let%hook (disableFeatureFlag, _state) =
    ReveryGraphqlHooks.useMutation(
      ~config=graphQLConfig,
      ~definition=Core.FeatureFlags.DisableFeatureFlag.definition,
      (),
    );

  let toggle = () => {
    let variables =
      `Assoc([
        ("type_", `String("ALL")),
        ("state", `String("ALL")),
        ("name", `String(featureFlag.name)),
      ]);

    if (featureFlag.enabled) {
      dispatch(DisableFeatureFlag(featureFlag.name));
      disableFeatureFlag(
        ~onCompleted=
          result => {
            switch (result) {
            | Ok(data) =>
              switch (data#deactivateGlobalFeatureFlags) {
              | `DeactivateGlobalFeatureFlagsPayloadError(data) =>
                Printf.printf("Mutation failed: %s%!", data#message)
              | `DeactivateGlobalFeatureFlagsPayloadSuccess(data) =>
                dispatch(UpdateFeatureFlags(data#featureFlags))
              }
            | Error(_error) => Printf.printf("Mutation failed")
            }
          },
        ~variables,
        (),
      );
    } else {
      dispatch(EnableFeatureFlag(featureFlag.name));
      enableFeatureFlag(
        ~onCompleted=
          result => {
            switch (result) {
            | Ok(data) =>
              switch (data#activateGlobalFeatureFlags) {
              | `ActivateGlobalFeatureFlagsPayloadError(data) =>
                Printf.printf("Mutation failed: %s%!", data#message)
              | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
                dispatch(UpdateFeatureFlags(data#featureFlags))
              }
            | Error(_error) => Printf.printf("Mutation failed")
            }
          },
        ~variables,
        (),
      );
    };
  };

  <MyRow debug style=Styles.row>
    <Text style=Styles.text text={featureFlag.name} />
    <Checkbox
      style=Styles.checkboxStyle
      checked={featureFlag.enabled}
      onChange=toggle
    />
  </MyRow>;
};
