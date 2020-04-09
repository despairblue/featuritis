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

  let%component make = (~config: Core.Config.t, ()) => {
    let%hook (state, dispatch) =
      Hooks.reducer(
        ~initialState=
          State.make(~apiUrl=config.apiUrl, ~authToken=config.authToken),
        State.reducer,
      );

    let ctx = Core.Context.make(state, dispatch);

    // TODO: move to state
    let login = (~email, ~password) => {
      printf("login\n%!");
      Lib.HTTP.sendPost(~email, ~password)
      |> Lwt.map(
           fun
           | Some(authToken) => {
               printf("authToken: %s\n%!", authToken);
               dispatch(Login(authToken));
             }
           | None => {
               printf("nothing :(\n%!");
             },
         )
      |> ignore;
    };

    switch (state.authToken) {
    | Some(authToken) => <FeatureFlagList authToken ctx />

    | None =>
      <View>
        <Login
          onSubmit={(~username, ~password) =>
            login(~email=username, ~password)
          }
        />
      </View>
    };
  };
};
