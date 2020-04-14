open Stdio;
open Revery;
open Revery.UI;
open Revery.UI.Components;

let mapValue = (~f, promise) =>
  Lwt.map(
    fun
    | Some(data) => {
        printf("Got some%!");
        Some(f(data));
      }
    | None => {
        printf("Got none%!");
        None;
      },
  );

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

let%component make = (~config: Core.Config.t, ()) => {
  let variables =
    `Assoc([("type_", `String("ALL")), ("state", `String("ALL"))]);
  let%hook flags = Core.FeatureFlags.QueryHook.use(~variables, ());

  let featureFlags =
    switch (flags) {
    | Idle => <Text style=Styles.text text="Nothing Loaded Yet" />
    | Loading => <Text style=Styles.text text="Loading..." />
    | Error => <Text style=Styles.text text={Printf.sprintf("Error: ")} />
    | Data(query) =>
      let featureFlags = query#featureFlags;
      <View>
        <Padding padding=4>
          <Text
            style=Styles.text
            text={Printf.sprintf("Total: %i", Array.length(featureFlags))}
          />
        </Padding>
        {featureFlags
         |> ArrayLabels.map(~f=featureFlag => {
              <FeatureFlag featureFlag toggle={(_, _) => ()} />
            })
         |> ArrayLabels.to_list
         |> React.listToElement}
      </View>;
    };

  <View> featureFlags </View>;
};
