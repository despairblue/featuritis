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

  let query = Core.FeatureFlags.Query.make(~type_=`ALL, ~state=`ALL, ());
  let increment = () => {
    Lib.HTTP.sendQuery(
      ~apiUrl=state.apiUrl,
      ~query=query#query,
      ~variables=query#variables,
      ~authToken,
    )
    |> Lwt.map(
         fun
         | Some(data) => {
             let data = Core.FeatureFlags.Query.parse(data);
             let ffs = data#featureFlags;

             printf("Found Flags: %i\n%!", Array.length(ffs));

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
        ~apiUrl=state.apiUrl,
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
                 printf("Mutation failed: %s%!", data#message)
               | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
                 let featureFlags = data#featureFlags;
                 dispatch(ReplaceFeatureFlags(featureFlags));
               };
             }
           | None => {
               printf("Found nothing");
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
        ~apiUrl=state.apiUrl,
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
                 printf("Mutation failed: %s%!", data#message)
               | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
                 let featureFlags = data#featureFlags;
                 dispatch(ReplaceFeatureFlags(featureFlags));
               };
             }
           | None => {
               printf("Found nothing");
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
  let features =
    React.listToElement([
      <Padding padding=4> <Text style=Styles.text text /> </Padding>,
      ...children,
    ]);

  <View>
    <ApiUrlInput ctx onSubmit={url => dispatch(ChangeApiUrl(url))} />
    <Clickable onClick=increment>
      <View style=Styles.button />
      features
    </Clickable>
  </View>;
};
