open Revery;
open Revery.UI;
open Revery.UI.Components;

module FeatureFlag = FeatureFlag.FeatureFlag;

module SimpleButton = {
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

  module State = Core.State;

  let mapValue = (~f, promise) =>
    Lwt.map(
      fun
      | Some(data) => {
          Printf.printf("Got some%!");
          Some(f(data));
        }
      | None => {
          Printf.printf("Got none%!");
          None;
        },
    );

  let%component make = () => {
    let%hook (state, dispatch) =
      Hooks.reducer(~initialState=State.make(), State.reducer);

    // Printf.printf("in callback, count: %i\n%!", state);
    // Printf.printf("in callback, bodyLength: %i\n%!", bodyLength);
    let authToken = "fba8ebc3d0827bafb46def017f9a98d7b061bed9a671ede53de69171ed6683ca29cf36cff4a7718ae6d4bc5b7e9f701d8f643cf0ff0d542aea9df4187898d9d7LhOFXNL6T";
    let query = Core.FeatureFlags.Query.make(~type_=`ALL, ~state=`ALL, ());
    let increment = () => {
      Lib.HTTP.sendQuery(
        ~query=query#query,
        ~variables=query#variables,
        ~authToken,
      )
      |> Lwt.map(
           fun
           | Some(data) => {
               let data = Core.FeatureFlags.Query.parse(data);
               let ffs = data#featureFlags;

               Printf.printf("Found Flags: %i\n%!", Array.length(ffs));

               dispatch(ReplaceFeatureFlags(ffs));
             }
           | None => (),
         )
      |> ignore;
    };
    let toggle = (name, enabled) =>
      if (enabled) {
        dispatch(DisableFeatureFlag(name));
        let disableFeatureFlag =
          Core.FeatureFlags.DisableFeatureFlag.make(
            ~type_=`ALL,
            ~state=`ALL,
            ~name=[|name|],
            (),
          );
        Lib.HTTP.sendQuery(
          ~query=disableFeatureFlag#query,
          ~variables=disableFeatureFlag#variables,
          ~authToken,
        )
        |> Lwt.map(
             fun
             | Some(data) => {
                 let data = Core.FeatureFlags.EnableFeatureFlag.parse(data);
                 let activateGlobalFeatureFlags =
                   data#activateGlobalFeatureFlags;

                 switch (activateGlobalFeatureFlags) {
                 | `ActivateGlobalFeatureFlagsPayloadError(data) =>
                   Printf.printf("Mutation failed: %s%!", data#message)
                 | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
                   let featureFlags = data#featureFlags;
                   dispatch(ReplaceFeatureFlags(featureFlags));
                 };
               }
             | None => {
                 Printf.printf("Found nothing");
               },
           )
        |> ignore;
      } else {
        dispatch(EnableFeatureFlag(name));
        let enableFeatureFlag =
          Core.FeatureFlags.EnableFeatureFlag.make(
            ~type_=`ALL,
            ~state=`ALL,
            ~name=[|name|],
            (),
          );
        Lib.HTTP.sendQuery(
          ~query=enableFeatureFlag#query,
          ~variables=enableFeatureFlag#variables,
          ~authToken,
        )
        |> Lwt.map(
             fun
             | Some(data) => {
                 let data = Core.FeatureFlags.EnableFeatureFlag.parse(data);
                 let activateGlobalFeatureFlags =
                   data#activateGlobalFeatureFlags;

                 switch (activateGlobalFeatureFlags) {
                 | `ActivateGlobalFeatureFlagsPayloadError(data) =>
                   Printf.printf("Mutation failed: %s%!", data#message)
                 | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
                   let featureFlags = data#featureFlags;
                   dispatch(ReplaceFeatureFlags(featureFlags));
                 };
               }
             | None => {
                 Printf.printf("Found nothing");
               },
           )
        |> ignore;
      };

    let children =
      ArrayLabels.map(state.featureFlags, ~f=featureFlag => {
        <FeatureFlag featureFlag toggle />
      })
      |> Array.to_list;

    let text =
      "Click me: " ++ string_of_int(state.featureFlags |> Array.length);
    let huh =
      React.listToElement([
        <Padding padding=4> <Text style=Styles.text text /> </Padding>,
        ...children,
      ]);

    <Clickable onClick=increment>
      <View style=Styles.button />
      huh
    </Clickable>;
  };
};
