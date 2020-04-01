open Stdio;
open Core;
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
      fontFamily(Constants.fontFamily),
      fontSize(20.),
    ];
  };

  module State = Core.State;

  let%component make = (~config: Core.Config.t, ()) => {
    open MyHooks;

    let%hook (state, dispatch) =
      Hooks.reducer(
        ~initialState=
          State.make(~apiUrl=config.apiUrl, ~authToken=config.authToken, ()),
        State.reducer,
      );

    let%hook (sendRequest, loginState) =
      UsePost.use(
        ~headers=UsePost.defaultJSONHeaders,
        ~bodyParser=
          body =>
            body
            |> Yojson.Basic.from_string
            |> Yojson.Basic.Util.member("authToken")
            |> Yojson.Basic.Util.to_string,
        ~onCompleted=
          fun
          | Data(authToken) => dispatch(Login(authToken))
          | _ => (),
        "http://localhost:3000/api/v0.0.1/users/authenticate",
      );

    let ctx = Core.Context.make(state, dispatch);

    let login = (~email, ~password) => {
      let body =
        `Assoc([
          ("email", `String(email)),
          ("password", `String(password)),
        ]);

      sendRequest(~body, ());
    };

    switch (state.graphQLConfig) {
    | Some(graphQLConfig) => <FeatureFlagList graphQLConfig ctx />

    | None =>
      <View>
        <Login onSubmit=login />
        {switch (loginState) {
         | Idle => <Text style=Styles.text text="idle" />
         | Loading => <Text style=Styles.text text="loading" />
         | ServerError(Fetch.Response.{status, body}) =>
           <Text
             style=Styles.text
             text={Printf.sprintf(
               "ServerError: %i\n%s",
               Fetch.Status.toCode(status),
               Fetch.Body.toString(body),
             )}
           />
         | NetworkError(error) =>
           <Text
             style=Styles.text
             text={Printf.sprintf("NetworkError: %s", error)}
           />
         | Data(body) => <Text style=Styles.text text=body />
         }}
      </View>
    };
  };
};
